cmake_minimum_required(VERSION 3.26)
project(sfml_flecs_demo)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(AutoPas)

find_package(flecs REQUIRED)
find_package(SFML COMPONENTS window graphics REQUIRED)

# find_package(box2d CONFIG REQUIRED)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h")
add_executable(sfml_flecs_demo ${SRC})

target_link_libraries(sfml_flecs_demo PRIVATE sfml-window sfml-graphics)
target_link_libraries(sfml_flecs_demo PRIVATE flecs::flecs flecs::flecs_static)

target_link_libraries(sfml_flecs_demo PRIVATE autopas autopasTools SPHLibrary)

# target_link_libraries(sfml_flecs_demo PRIVATE box2d::box2d)
