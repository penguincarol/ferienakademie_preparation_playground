#ifndef GAME_TESTS_WINDOW_UTIL_H
#define GAME_TESTS_WINDOW_UTIL_H

#include "flecs.h"

struct window_module {
    explicit window_module(flecs::world &);
};

void main_loop(flecs::world & );

#endif //GAME_TESTS_WINDOW_UTIL_H
