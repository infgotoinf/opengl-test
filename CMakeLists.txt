cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_GENERATOR "Ninja") # Doesn't work
set(CMAKE_BUILD_TYPE "Debug")

project(LyckyBallRace C CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(SYSTEM
  external/glad/include
)

add_executable(game
  main/main.cpp
  external/glad/src/gl.c
)

target_sources(game
  PRIVATE
    main/main.cpp

  PUBLIC
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS
      main/include
    FILES
)

target_link_libraries(game
  PRIVATE
    GL
    glfw
    # m
    # pthread
    # dl
    # rt
    # X11
)
