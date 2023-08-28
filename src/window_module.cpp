#include "window_module.h"
#include "iostream"

using namespace std;

window_module::window_module(flecs::world &w) {
    w.module<window_module>();
    cout<<"hi"<<endl;
}
