cmake_minimum_required(VERSION 3.18)

project(ray-casting C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
                    -fopenmp \
                    -fstack-protector \
                    -fstrict-aliasing \
                    -g \
                    -O3 \
                    -pedantic \
                    -Wall \
                    -Walloc-zero \
                    -Walloca \
                    -Warith-conversion \
                    -Warray-bounds=2 \
                    -Wcast-qual \
                    -Wchar-subscripts \
                    -Wconversion \
                    -Wdisabled-optimization \
                    -Wduplicated-branches \
                    -Wduplicated-cond \
                    -Wextra \
                    -Wfloat-equal \
                    -Wformat-nonliteral \
                    -Wformat-overflow=2 \
                    -Wformat-security \
                    -Wformat-signedness \
                    -Wformat-truncation=2 \
                    -Wformat-y2k \
                    -Wformat=2 \
                    -Winline \
                    -Wjump-misses-init \
                    -Wlogical-op \
                    -Wmissing-format-attribute \
                    -Wmissing-noreturn \
                    -Wmissing-prototypes \
                    -Wnormalized \
                    -Wnull-dereference \
                    -Wold-style-definition \
                    -Wpacked \
                    -Wpedantic \
                    -Wredundant-decls \
                    -Wshadow \
                    -Wshift-overflow=2 \
                    -Wstack-protector \
                    -Wstrict-overflow=5 \
                    -Wstrict-prototypes \
                    -Wstringop-overflow=4 \
                    -Wswitch-enum \
                    -Wundef \
                    -Wuninitialized \
                    -Wunreachable-code \
                    -Wunsuffixed-float-constants \
                    -Wunused-const-variable=2 \
                    -Wunused-macros \
                    -Wunused-parameter \
                    -Wvla \
                    -Wwrite-strings")

find_package(SDL2 REQUIRED)
find_library(SDL2_GFX NAMES SDL2_gfx REQUIRED)
find_library(SDL2_IMG NAMES SDL2_image REQUIRED)

file(GLOB SOURCES "src/*.c")
file(GLOB TEST_SOURCES "tests/*.c")
list(REMOVE_ITEM SOURCES ${TEST_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(test ${TEST_SOURCES} "src/math.c" "src/vector.c" "src/util.c")

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_GFX} ${SDL2_IMG} m)
target_link_libraries(test ${SDL2_LIBRARIES} ${SDL2_GFX} ${SDL2_IMG} m)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/"
)

file(GLOB ASSETS "assets/*")

foreach (FILE ${ASSETS})
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${FILE} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/"
    )
endforeach ()
