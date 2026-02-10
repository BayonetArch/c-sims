/* common.c */

#include "common.h"

Vec2f vec2f_scale(Vec2f v, float scale)
{
    return (Vec2f){.x = v.x * scale, .y = v.y * scale};
}

Vec2f vec2f_add(Vec2f v1, Vec2f v2)
{
    return (Vec2f){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

Vec2f vec2f_sub(Vec2f v1, Vec2f v2)
{

    return (Vec2f){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

SDL_FPoint vec2f_to_sdlfp(Vec2f src) { return (SDL_FPoint){src.x, src.y}; }
Vec2f      sdlfp_to_vec2f(SDL_FPoint src) { return (Vec2f){src.x, src.y}; }

void render_circlev(SDL_Renderer *renderer, Vec2f center, float r)
{
    int cx     = (int)center.x;
    int cy     = (int)center.y;
    int radius = (int)r;

    int x                 = radius;
    int y                 = 0;
    int decisionParameter = 1 - radius;

    while (x >= y) {
        // draw all 8 octants of the circle
        SDL_RenderPoint(renderer, cx + x, cy + y);
        SDL_RenderPoint(renderer, cx - x, cy + y);
        SDL_RenderPoint(renderer, cx + x, cy - y);
        SDL_RenderPoint(renderer, cx - x, cy - y);
        SDL_RenderPoint(renderer, cx + y, cy + x);
        SDL_RenderPoint(renderer, cx - y, cy + x);
        SDL_RenderPoint(renderer, cx + y, cy - x);
        SDL_RenderPoint(renderer, cx - y, cy - x);

        y++;

        if (decisionParameter <= 0) {
            // midpoint is inside or on the circle perimeter
            decisionParameter += 2 * y + 1;
        } else {
            // midpoint is outside the circle perimeter
            x--;
            decisionParameter += 2 * (y - x) + 1;
        }
    }
}

void render_filled_circlev(SDL_Renderer *renderer, Vec2f center, float r)
{
    int cx     = (int)center.x;
    int cy     = (int)center.y;
    int radius = (int)r;

    int x                 = radius;
    int y                 = 0;
    int decisionParameter = 1 - radius;

    while (x >= y) {
        // Top and bottom halves
        SDL_RenderLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderLine(renderer, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderLine(renderer, cx - y, cy + x, cx + y, cy + x);
        SDL_RenderLine(renderer, cx - y, cy - x, cx + y, cy - x);

        y++;

        if (decisionParameter <= 0) {
            decisionParameter += 2 * y + 1;
        } else {
            x--;
            decisionParameter += 2 * (y - x) + 1;
        }
    }
}
