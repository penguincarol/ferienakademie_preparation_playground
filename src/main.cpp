#include <SFML/Graphics.hpp>
#include <iostream>
#include <flecs.h>
#include <memory>
#include <box2d/box2d.h>
#include "window_module.h"

using namespace std;
using namespace sf;

const String WINDOW_TITLE = "Game tests";
const VideoMode DEFAULT_VIDEOMODE = VideoMode(1600, 900);

class App {
public:
    App() {
        cout << "create App" << endl;
        window = make_shared<RenderWindow>(DEFAULT_VIDEOMODE, WINDOW_TITLE);
        view = make_shared<View>(FloatRect(0, 0, 1000, 1000));
        running = true;
        fullscreen = false;
        f11_pressed = false;
    }

    shared_ptr<RenderWindow> window;
    shared_ptr<View> view;
    bool running;
    bool fullscreen;
    bool f11_pressed;
};

void window_prepare_drawing_system(flecs::iter, App *app) {
    app->window->clear();
}

void toggle_fullscreen_system(flecs::iter, App *app) {
    auto window = app->window;
    if (!app->f11_pressed && Keyboard::isKeyPressed(Keyboard::Key::F11)) {
        if (app->fullscreen) {
            window->create(DEFAULT_VIDEOMODE, WINDOW_TITLE, Style::Default);
        } else {
            window->create(VideoMode::getDesktopMode(), WINDOW_TITLE, Style::Fullscreen);
        }
        app->f11_pressed = true;
        app->fullscreen = !app->fullscreen;
    } else if (app->f11_pressed && !Keyboard::isKeyPressed(Keyboard::Key::F11)) {
        app->f11_pressed = false;
    }
}

void rotate_system(flecs::iter it, RectangleShape *rects) {
    for (auto i: it) {
        rects[i].rotate(100.0f * it.delta_time());
    }
}

void draw_rect_system(flecs::iter it, RectangleShape *rects, App *app) {
    auto window = app->window;
    for (auto i: it) {
        window->draw(rects[i]);
    }
}

void move_system(flecs::iter it, RectangleShape *rects) {
    Vector2f delta;
    if (Keyboard::isKeyPressed(Keyboard::Key::W)) delta.y -= 400;
    if (Keyboard::isKeyPressed(Keyboard::Key::S)) delta.y += 400;
    if (Keyboard::isKeyPressed(Keyboard::Key::A)) delta.x -= 400;
    if (Keyboard::isKeyPressed(Keyboard::Key::D)) delta.x += 400;
    for (auto i: it) {
        rects[i].move(delta * it.delta_time());
    }
}

void window_update_system(flecs::iter, App *app) {
    auto window = app->window;

    Event event{};
    while (window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            app->running = false;
            window->close();
        } else if (event.type == Event::Resized) {
            float ratio = (float) event.size.height / (float) event.size.width;
            app->view->setSize(1000.f, 1000.f * ratio);
        }
    }

    window->setView(*app->view);
    window->display();
}

class Rotating {
};

class Movable {
};

int main() {
    flecs::world world;

    world.add<App>();

    world.import<window_module>();

    auto rotating_tag = world.entity().add<Rotating>();
    auto movable_tag = world.entity().add<Movable>();

    auto rect_e = world.entity().add<RectangleShape>();
    rect_e.add(rotating_tag).add(movable_tag);

    auto rect = rect_e.get_mut<RectangleShape>();
    rect->setSize(Vector2f(100, 100));
    rect->setFillColor(Color::Red);
    rect->setOrigin(50, 50);
    rect->setPosition(500, 500);

    world.system<App>().term_at(1).singleton().kind(flecs::PreUpdate).iter(window_prepare_drawing_system);
    world.system<RectangleShape>().with(rotating_tag).kind(flecs::PreUpdate).iter(rotate_system);
    world.system<RectangleShape>().with(movable_tag).kind(flecs::PreUpdate).iter(move_system);
    world.system<App>().term_at(1).singleton().kind(flecs::PreUpdate).iter(toggle_fullscreen_system);

    world.system<RectangleShape, App>().term_at(2).singleton().kind(flecs::OnUpdate).iter(draw_rect_system);

    world.system<App>().term_at(1).singleton().kind(flecs::PostUpdate).iter(window_update_system);

    Clock clk;
    while (world.get<App>()->running) {
        world.progress(clk.restart().asSeconds());
    }

    return 0;
}
