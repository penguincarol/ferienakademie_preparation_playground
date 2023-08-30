#include <SFML/Graphics.hpp>
#include <flecs.h>
#include "window_module.h"

using namespace std;
using namespace sf;

void rotate_system(flecs::iter it, RectangleShape *rects) {
    for (auto i: it) {
        rects[i].rotate(100.0f * it.delta_time());
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

struct Rotating {
};

struct Movable {
};

int main() {
    flecs::world world;

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

    world.system<RectangleShape>().with(rotating_tag).kind(flecs::PreUpdate).iter(rotate_system);
    world.system<RectangleShape>().with(movable_tag).kind(flecs::PreUpdate).iter(move_system);

    main_loop(world);
}
