cmake_minimum_required(VERSION 3.6)
project(synth_firmware_game_project C CXX)
include(FetchContent)
include(opengl.cmake.in)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_library(machinemodel SHARED STATIC shared/model/machinemodel.cpp)

# ********* x86 teensy cores :: feature/shared_serial branch
set(BUILD_SHARED_LIBRARY ON)
FetchContent_Declare(teensy_x86_stubs
        GIT_REPOSITORY https://github.com/newdigate/teensy-x86-stubs.git
        GIT_TAG        feature/shared_serial
)
FetchContent_MakeAvailable(teensy_x86_stubs)
set(teensy_x86_stubs_local_include "${FETCHCONTENT_BASE_DIR}/teensy_x86_stubs-src/src")
message(INFO " ${teensy_x86_stubs_local_include}")
include_directories(${teensy_x86_stubs_local_include})
#***********


# ********* assimp model loading for importing .obj files
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        master
)

FetchContent_MakeAvailable(assimp)
#*********** // end assimp




add_subdirectory(examples/blink)


add_subdirectory(src)

