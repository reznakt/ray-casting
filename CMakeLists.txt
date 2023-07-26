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
                    -fopenmp \
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
                    -Wmissing-prototypes \
                    -Wpointer-arith \
                    -Wno-keyword-macro")


find_package(SDL2 REQUIRED)


find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx)

if (NOT SDL2_GFX_LIBRARY)
    message(FATAL_ERROR " SDL2_gfx library not found ")
endif ()

find_library(SDL2_IMG_LIBRARY NAMES SDL2_image)

if (NOT SDL2_IMG_LIBRARY)
    message(FATAL_ERROR " SDL2_image library not found ")
endif ()

file(GLOB SOURCES "*.c")
file(GLOB TEST_SOURCES "tests/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(test ${TEST_SOURCES} "math.c" "vector.c")


target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY} ${SDL2_IMG_LIBRARY} m)
target_link_libraries(test ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY} ${SDL2_IMG_LIBRARY} m)
