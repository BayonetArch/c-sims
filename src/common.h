#ifndef COMMON_H
#define COMMON_H

#include <SDL3/SDL.h>

#define CX_STRIP_PREFIX
#include "../ext/include/cx.h"

static bool colored_log = true;

typedef struct {
    float x;
    float y;
} Vec2f;

typedef struct {
    float x1;
    float y1;

    float x2;
    float y2;
} LinePoints;

typedef struct {
    int win_h;
    int win_w;
} WinCfg;

Vec2f vec2f_scale(Vec2f v, float scale);
Vec2f vec2f_add(Vec2f v1, Vec2f v2);
Vec2f vec2f_sub(Vec2f v1, Vec2f v2);

SDL_FPoint vec2f_to_sdlfp(Vec2f src);
Vec2f      sdlfp_to_vec2f(SDL_FPoint src);

void render_circlev(SDL_Renderer *renderer, Vec2f center, float r);
void render_filled_circlev(SDL_Renderer *renderer, Vec2f center, float r);

#define percent_of(p, x) ((p) / (100.0f) * (x))
#define SQR(x)           ((x) * (x))
#define CUBE(x)          ((x) * (x) * (x))

#define RED    255, 0, 0, 255
#define GREEN  0, 255, 0, 255
#define YELLOW 255, 255, 0, 255
#define BLUE   0, 0, 255, 255
#define WHITE  255, 255, 255, 255
#define BLACK  0, 0, 0, 255

#define REDA(a)    255, 0, 0, (a)
#define GREENA(a)  0, 255, 0, (a)
#define YELLOWA(a) 255, 255, 0, (a)
#define BLUEA(a)   0, 0, 255, (a)
#define WHITEA(a)  255, 255, 255, (a)
#define BLACKA(a)  0, 0, 0, (a)

#define APP_LOG(level, fmt, ...)                                               \
    do {                                                                       \
        if (colored_log) CX_LOGC(level, fmt, ##__VA_ARGS__);                   \
        else CX_LOG(level, fmt, ##__VA_ARGS__);                                \
    } while (0)

#define APP_FATAL(fmt, ...)                                                    \
    do {                                                                       \
        if (colored_log) {                                                     \
            cx_fatalc(fmt "(%s)", ##__VA_ARGS__, SDL_GetError());              \
        } else {                                                               \
            cx_fatal(fmt "(%s)", ##__VA_ARGS__, SDL_GetError());               \
        }                                                                      \
    } while (0)

#endif // COMMON_H
