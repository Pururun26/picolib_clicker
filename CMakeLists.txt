cmake_minimum_required(VERSION 3.10)
project(picolib_clicker)   # ← имя проекта изменено

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ==========================================================
# --- Оптимизация по размеру ---
# ==========================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -s")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
endif()
# ==========================================================

# Пути к библиотеке raylib
set(RAYLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(RAYLIB_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)

include_directories(${RAYLIB_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/headers)

# Ищем ТОЛЬКО статическую библиотеку (.a), чтобы exe был автономным
find_library(RAYLIB_LIBRARY
    NAMES libraylib.a raylib 
    PATHS ${RAYLIB_LIBRARY_DIR}
    NO_DEFAULT_PATH
)
if(NOT RAYLIB_LIBRARY)
    find_library(RAYLIB_LIBRARY raylib)
endif()

if(NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "Не удалось найти библиотеку raylib!")
endif()

# Собираем исполняемый файл (добавьте сюда все ваши .c файлы)
add_executable(picolib_clicker
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/picolib.c
    # при необходимости добавьте другие файлы, например:
    # ${CMAKE_SOURCE_DIR}/src/other.c
)

# Линкуем библиотеки
target_link_libraries(picolib_clicker
    ${RAYLIB_LIBRARY}
    gdi32
    winmm
    shell32
    opengl32
)

# ==========================================================
# --- Полная статическая линковка ---
# ==========================================================
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(picolib_clicker PRIVATE -static)
endif()

# ==========================================================
# --- СКРЫВАЕМ ОКНО КОНСОЛИ ---
# ==========================================================
set_target_properties(picolib_clicker PROPERTIES WIN32_EXECUTABLE TRUE)

# Для компиляторов GCC/MinGW добавляем флаг -mwindows
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(picolib_clicker PRIVATE -mwindows)
endif()
