# SFML and Flecs Demo

## Setup with CLion
1. Open the vcpkg tool window with: View -> Tool Windows -> Vcpkg
2. If you do not already have a vcpkg directory, create a new one by clicking the "+" symbol and accept with Ok.
3. Open: File -> Settings -> Build, Execution, Deployment -> CMake
4. Put `-DCMAKE_TOOLCHAIN_FILE=<HOMEPATH>/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake` into the CMake options. 
Replace `<HOMEPATH>` with your home path, e.g. /home/kilian.
5. Open your run configurations (next to the run button) and click "Edit configurations..."
6. Click on "+" and add a "CMake Application" configuration.
7. Select `sfml_flecs_demo` as target and executable.
8. Click run!