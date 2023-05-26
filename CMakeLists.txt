cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "clang")

project(ray-casting C)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
                    -D_GNU_SOURCE \
                    -std=c99 \
                    -g \
                    -Ofast \
                    -masm=intel \
                    -march=native \
                    -mtune=native \
                    -fverbose-asm \
                    -fstack-protector-strong \
                    -ffast-math \
                    -funroll-loops \
                    -fomit-frame-pointer \
                    -fstrict-aliasing \
                    -pedantic \
                    -pedantic-errors \
                    -Wall \
                    -Wextra \
                    -Werror \
                    -Wfloat-equal \
                    -Wundef \
                    -Wshadow \
                    -Wwrite-strings \
                    -Wcast-qual \
                    -Wswitch-default \
                    -Wswitch-enum \
                    -Wconversion \
                    -Wunreachable-code \
                    -Wformat=2 \
                    -Wreturn-type \
                    -Wcomment \
                    -Wchar-subscripts \
                    -Wparentheses \
                    -Wpointer-arith")


find_package(SDL2 REQUIRED)


find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx)

if (NOT SDL2_GFX_LIBRARY)
    message(FATAL_ERROR " SDL2_gfx library not found ")
endif ()


add_executable(${PROJECT_NAME} main.c math.c vector.c world.c ray.c font.c game.c util.c logger.c)


target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY} m)
