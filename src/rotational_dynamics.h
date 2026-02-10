/* rotational_dynamics.h */

#ifndef ROT_DYN_H
#define ROT_DYN_H

#include "common.h"


void rotdyn_draw_body_axis(Vec2f p1, Vec2f p2, float thickness);
bool rotdyn_sim_start(SDL_Renderer *renderer, WinCfg wincfg);
void rotdyn_cleanup();

#endif // !ROT_DYN_H
