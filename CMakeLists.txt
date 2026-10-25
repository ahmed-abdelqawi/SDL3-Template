cmake_minimum_required(VERSION 3.24)
project(sdl3-template LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c src/*.cpp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for Zed autocomplete

include(FetchContent)

set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_TESTS   OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.0   # bump to the latest release-3.x.y tag
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
