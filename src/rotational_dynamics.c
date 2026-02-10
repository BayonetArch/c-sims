/* rotational_dynamics.c */

#include "rotational_dynamics.h"

#define TEXT_RECT_W 100
#define TEXT_RECT_H 100

typedef struct {
    SDL_Texture *texture_rect;
} RotDynState;

static RotDynState state = {0};

void rotdyn_cleanup()
{
    if (state.texture_rect) SDL_DestroyTexture(state.texture_rect);
}

void rotdyn_draw_body_axis(Vec2f p1, Vec2f p2, float thickness)
{
    UNUSED(p1);

    UNUSED(p2);
    UNUSED(thickness);
}

bool rotdyn_sim_start(SDL_Renderer *renderer, WinCfg wincfg)
{
    static float theta  = 0.0f;
    float        dest_w = 100.0f;
    float        dest_h = 100.0f;

    SDL_FRect dest = {(float)wincfg.win_w / 2 - dest_w / 2,
                      (float)wincfg.win_h / 2 - dest_h / 2, dest_w, dest_h};

    if (!state.texture_rect) {
        state.texture_rect = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                               SDL_TEXTUREACCESS_TARGET,
                                               TEXT_RECT_W, TEXT_RECT_H);
        if (!state.texture_rect) APP_FATAL("Could not create texture.");

        SDL_SetRenderTarget(renderer, state.texture_rect);
        SDL_SetRenderDrawColor(renderer, 30, 30, 46, 255);
        SDL_RenderClear(renderer);

        Vec2f circle_center = {(float)TEXT_RECT_W / 2, (float)TEXT_RECT_H / 2};

        SDL_SetRenderDrawColor(renderer, RED);
        render_filled_circlev(renderer, circle_center, 30.0f);

        SDL_SetRenderTarget(renderer, NULL);
    }

    SDL_RenderTextureRotated(renderer, state.texture_rect, NULL, &dest, theta,
                             NULL, SDL_FLIP_NONE);
    theta += 2.0f;
    if (theta >= 360.0f) theta -= 360.0f;
    return true;
}
