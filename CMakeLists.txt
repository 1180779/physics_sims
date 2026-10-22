cmake_minimum_required(VERSION 4.3)
project(physics_sims)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# fetch qt and openGL
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

find_package(OpenGL REQUIRED)

include(FetchContent)

enable_testing()

# distributable archives need Qt's runtime deployment API (Qt >= 6.5 on Linux)
# this is opt-in so development builds work on older Qt versions (ex. with ubuntu
# 24.04 LTS); 
# pass -DENABLE_BUNDLE=ON to build the release archives
option(ENABLE_BUNDLE "Build the distributable bundle target" OFF)

set(TBB_TEST OFF CACHE BOOL "Don't build TBB tests" FORCE)
set(TBB_STRICT OFF CACHE BOOL "Don't treat TBB compiler warnings as errors" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "Don't build TBB examples" FORCE)

# fetch tbb for multithreading stuff from std::execution
FetchContent_Declare(
        oneTBB
        GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
        GIT_TAG v2021.13.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(oneTBB)

# fetch the custom math library
FetchContent_Declare(
        cad_math
        GIT_REPOSITORY https://github.com/1180779/cad_math.git
        GIT_TAG v1.0.1
)
FetchContent_MakeAvailable(cad_math)

FetchContent_Declare(
        cad_graphics
        GIT_REPOSITORY https://github.com/1180779/cad_graphics.git
        GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(cad_graphics)

# fetch the custom qt library with common stuff
FetchContent_Declare(
        cad_common
        GIT_REPOSITORY https://github.com/1180779/cad_common.git
        GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(cad_common)

# add individual targets (more precisely their subdirectories)
add_subdirectory(src/spring)

