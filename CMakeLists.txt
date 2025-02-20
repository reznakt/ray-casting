cmake_minimum_required(VERSION 3.5)

set(STRICT OFF CACHE BOOL "Promote warnings to errors")

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_DIR} LANGUAGES C DESCRIPTION "A simple ray casting project using SDL2")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
        -fstrict-aliasing
        -g
        -O3
        -pedantic
        -Wall
        -Walloc-zero
        -Walloca
        -Warith-conversion
        -Warray-bounds=2
        -Wcast-qual
        -Wchar-subscripts
        -Wconversion
        -Wdisabled-optimization
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra
        -Wfloat-equal
        -Wformat-nonliteral
        -Wformat-overflow=2
        -Wformat-security
        -Wformat-signedness
        -Wformat-truncation=2
        -Wformat-y2k
        -Wformat=2
        -Winline
        -Wjump-misses-init
        -Wlogical-op
        -Wmissing-format-attribute
        -Wmissing-noreturn
        -Wmissing-prototypes
        -Wnormalized
        -Wnull-dereference
        -Wold-style-definition
        -Wpacked
        -Wpedantic
        -Wredundant-decls
        -Wshadow
        -Wshift-overflow=2
        -Wstack-protector
        -Wstrict-overflow=5
        -Wstrict-prototypes
        -Wstringop-overflow=4
        -Wswitch-enum
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wunsuffixed-float-constants
        -Wunused-const-variable=2
        -Wunused-macros
        -Wunused-parameter
        -Wvla
        -Wwrite-strings
)

if (NOT CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wno-unknown-warning-option)
endif ()

if (CMAKE_C_COMPILER_ID MATCHES "TinyCC")
    # See https://www.mail-archive.com/tinycc-devel@nongnu.org/msg08821.html.
    add_compile_options(-DSDL_DISABLE_IMMINTRIN_H)
endif ()

function(notice message)
    string(ASCII 27 ESCAPE)
    message(NOTICE "-- ${ESCAPE}[36mNotice: ${message}${ESCAPE}[0m")
endfunction()

if (STRICT)
    notice("Strict mode enabled, promoting warnings to errors.")
    add_compile_options(-Werror -pedantic-errors)
else ()
    notice("Strict mode disabled. Use -DSTRICT=ON to promote warnings to errors.")
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
                        -s USE_SDL=2 \
                        -s USE_SDL_GFX=2 \
                        --embed-file=assets/")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
    find_package(SDL2 REQUIRED)
    find_library(SDL2_GFX NAMES SDL2_gfx REQUIRED)
endif ()

find_program(GIT_EXECUTABLE git)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" branch --show-current
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_BRANCH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --always --abbrev=40
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --always
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_HASH_SHORT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%cd --date=iso
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_TIME
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%s
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_MESSAGE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        whoami
        OUTPUT_VARIABLE BUILD_USER
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
        hostname
        OUTPUT_VARIABLE BUILD_HOST
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S %z")

configure_file("src/version.c.in" "${CMAKE_SOURCE_DIR}/src/.version.c" @ONLY)

file(GLOB SOURCES "src/*.c")
file(GLOB TEST_SOURCES "tests/*.c")
list(REMOVE_ITEM SOURCES ${TEST_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(test ${TEST_SOURCES} "src/math.c" "src/vector.c" "src/util.c")

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
set(LIBS ${SDL2_LIBRARIES} ${SDL2_GFX} ${SDL2_IMG} m)

target_link_libraries(${PROJECT_NAME} ${LIBS})
target_link_libraries(test ${LIBS})

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/"
)

file(GLOB ASSETS "assets/*")

foreach (FILE ${ASSETS})
    add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${FILE} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/"
    )
endforeach ()
