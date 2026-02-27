/* sims.c */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbcks instead of main() */
#include <SDL3/SDL_main.h>

#include <SDL3_ttf/SDL_ttf.h>

#include "common.h"

#define INIT_WINDOW_W 600
#define INIT_WINDOW_H 400
#define INIT_FONT_SZ  16.0f
#define FPS_FONT_SZ   12.0f

static bool enable_vsync = true;

static float  dt             = 0.0f;
static Uint64 previous_frame = 0;
static Uint64 freq           = 0;

typedef struct {
    SDL_Window   *win;
    SDL_Renderer *renderer;

    TTF_Font       *global_font;
    float           global_font_sz;
    TTF_TextEngine *text_engine;

    WinCfg win_cfg;
} AppState;

typedef struct {
    TTF_Text *ttf_text;

    TTF_Font *font;
    float     font_sz;

    int text_w;
    int text_h;

    char content[64];
} FpsText;

static FpsText fps_text = {0};

static void setup_fps_ttf(TTF_TextEngine *text_engine)
{
    fps_text.font = TTF_OpenFont("assets/fps-font.ttf", FPS_FONT_SZ);
    if (!fps_text.font) APP_FATAL("Failed to load font");
    fps_text.font_sz = FPS_FONT_SZ;

    fps_text.ttf_text = TTF_CreateText(text_engine, fps_text.font, NULL, 0);
    if (!fps_text.ttf_text) APP_FATAL("Failed to create text");
}

static void sdl_ttf_init(AppState *s)
{

    TTF_Init();

    s->global_font = TTF_OpenFont("assets/font.ttf", INIT_FONT_SZ);
    if (!s->global_font) APP_FATAL("Failed to load font");

    s->global_font_sz = INIT_FONT_SZ;

    s->text_engine = TTF_CreateRendererTextEngine(s->renderer);
    if (!s->text_engine) APP_FATAL("Failed to create text engine");
}

static AppState *get_app_state(void *appstate)
{
    if (!appstate) APP_FATAL("AppState was null");

    return (AppState *)(appstate);
}

static void display_debug_fps(float x, float y)
{

    static int c             = 0;
    Uint64     current_frame = SDL_GetPerformanceCounter();
    Uint64     frame_diff    = current_frame - previous_frame;
    dt                       = (float)frame_diff / (float)freq;
    float        fps         = 1.0f / dt;
    static float prev_fps    = 0.0;

    if (c == 60) {
        SDL_snprintf(fps_text.content, sizeof(fps_text.content),
                     "Avg fps(vsync: %s) = %.2f",
                     enable_vsync == true ? "enabled" : "disabled", fps);

        c        = 0;
        prev_fps = fps;

    } else {
        SDL_snprintf(fps_text.content, sizeof(fps_text.content),
                     "Avg fps(vsync: %s) = %.2f",
                     enable_vsync == true ? "enabled" : "disabled", prev_fps);
    }

    TTF_SetTextString(fps_text.ttf_text, fps_text.content,
                      SDL_strlen(fps_text.content));

    // NOTE: might not need this
    TTF_GetTextSize(fps_text.ttf_text, &fps_text.text_w, &fps_text.text_h);

    TTF_DrawRendererText(fps_text.ttf_text, x, y);

    previous_frame = current_frame;

    c++;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *s = get_app_state(appstate);

    SDL_SetRenderDrawColor(s->renderer, BLACK);
    SDL_RenderClear(s->renderer);

    TTF_SetTextColor(fps_text.ttf_text, GREEN);
    display_debug_fps(s->win_cfg.win_w - fps_text.text_w, 2);

    SDL_RenderPresent(s->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    AppState *s = malloc(sizeof(AppState));
    if (!s) APP_FATAL("Could not allocate memory for appstate.");

    SDL_SetAppMetadata("SDL3 simulations", "1.0", "...");

    if (!SDL_Init(SDL_INIT_VIDEO)) APP_FATAL("Couldn't initialize SDL");

    s->win = SDL_CreateWindow("simulations", INIT_WINDOW_W, INIT_WINDOW_H, 0);
    if (!s->win) APP_FATAL("Couldn't create window");

    APP_LOG(LOG_INFO, "Welcome to simulation!");

    s->renderer = SDL_CreateRenderer(s->win, NULL);
    if (!s->renderer) APP_FATAL("Could not Create renderer");

    s->win_cfg.win_w = INIT_WINDOW_W;
    s->win_cfg.win_h = INIT_WINDOW_H;

    if (enable_vsync) SDL_SetRenderVSync(s->renderer, 1);

    // tff
    sdl_ttf_init(s);
    setup_fps_ttf(s->text_engine);

    // performance
    freq           = SDL_GetPerformanceFrequency();
    previous_frame = SDL_GetPerformanceCounter();

    *appstate = (void *)s;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{

    AppState *s = get_app_state(appstate);

    switch (event->type) {

    case SDL_EVENT_QUIT    : return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN: {
        Uint32 key = event->key.key;

        switch (key) {
        case SDLK_E: return SDL_APP_SUCCESS;

        default    : break;
        }

    } break;

    case SDL_EVENT_WINDOW_RESIZED:
        SDL_GetRenderOutputSize(s->renderer, &s->win_cfg.win_w,
                                &s->win_cfg.win_h);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        float button_x = event->button.x;
        float button_y = event->button.y;

        UNUSED(button_x);
        UNUSED(button_y);

    } break;

    case SDL_EVENT_FINGER_DOWN: {
        float touch_x = event->tfinger.x * s->win_cfg.win_w;
        float touch_y = event->tfinger.y * s->win_cfg.win_h;

        UNUSED(touch_x);
        UNUSED(touch_y);

    } break;

    default: break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    UNUSED(result);
    AppState *s = get_app_state(appstate);

    SDL_DestroyRenderer(s->renderer);
    SDL_DestroyWindow(s->win);

    TTF_CloseFont(s->global_font);
    TTF_CloseFont(fps_text.font);

    TTF_DestroyText(fps_text.ttf_text);
    TTF_DestroyRendererTextEngine(s->text_engine);

    free(s);
    APP_LOG(LOG_INFO, "Good Bye.");
}
