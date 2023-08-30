#include "window_module.h"
#include "iostream"
#include "SFML/Graphics.hpp"
#include <flecs.h>
#include <memory>

using namespace std;
using namespace sf;

const String WINDOW_TITLE = "Game tests";
const VideoMode DEFAULT_VIDEOMODE = VideoMode(1600, 900);

struct WindowContext {
    shared_ptr<RenderWindow> window = make_shared<RenderWindow>(DEFAULT_VIDEOMODE, WINDOW_TITLE);
    shared_ptr<View> view = make_shared<View>(FloatRect(0, 0, 1000, 1000));
    bool running = true;
    bool fullscreen = false;
    bool f11_pressed = false;
};

void window_update_system(flecs::iter, WindowContext *ctx) {
    auto window = ctx->window;

    Event event{};
    while (window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            ctx->running = false;
            window->close();
        } else if (event.type == Event::Resized) {
            float ratio = (float) event.size.height / (float) event.size.width;
            ctx->view->setSize(1000.f, 1000.f * ratio);
        }
    }

    window->setView(*ctx->view);
    window->display();
}

void draw_rect_system(flecs::iter it, RectangleShape *rects, WindowContext *ctx) {
    auto window = ctx->window;
    for (auto i: it) {
        window->draw(rects[i]);
    }
}

void window_prepare_drawing_system(flecs::iter, WindowContext *ctx) {
    ctx->window->clear();
}

void toggle_fullscreen_system(flecs::iter, WindowContext *ctx) {
    auto window = ctx->window;
    if (!ctx->f11_pressed && Keyboard::isKeyPressed(Keyboard::Key::F11)) {
        if (ctx->fullscreen) {
            window->create(DEFAULT_VIDEOMODE, WINDOW_TITLE, Style::Default);
        } else {
            window->create(VideoMode::getDesktopMode(), WINDOW_TITLE, Style::Fullscreen);
        }
        ctx->f11_pressed = true;
        ctx->fullscreen = !ctx->fullscreen;
    } else if (ctx->f11_pressed && !Keyboard::isKeyPressed(Keyboard::Key::F11)) {
        ctx->f11_pressed = false;
    }
}

void main_loop(flecs::world & world) {
    Clock clk;
    while (world.get<WindowContext>()->running) {
        world.progress(clk.restart().asSeconds());
    }
}

window_module::window_module(flecs::world &world) {
    world.module<window_module>();
    cout << "load window_module" << endl;

    world.add<WindowContext>();

    world.system<WindowContext>().term_at(1).singleton().kind(flecs::PreUpdate).iter(window_prepare_drawing_system);
    world.system<WindowContext>().term_at(1).singleton().kind(flecs::PreUpdate).iter(toggle_fullscreen_system);

    world.system<RectangleShape, WindowContext>().term_at(2).singleton().kind(flecs::OnUpdate).iter(draw_rect_system);

    world.system<WindowContext>().term_at(1).singleton().kind(flecs::PostUpdate).iter(window_update_system);
}
