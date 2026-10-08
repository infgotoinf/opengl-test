cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

project(game C CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_X11      OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_WIN32    OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_COCOA    OFF CACHE BOOL "" FORCE)

# https://www.glfw.org/docs/3.3/compile.html#compile_options_win32
# set(GLFW_USE_HYBRID_HPG OFF CACHE BOOL "" FORCE)

# GLAD
add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

# Final exacutable
add_executable(${PROJECT_NAME} WIN32
    main/main.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${GLFW_SOURCE_DIR}/deps
    main/include
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    OpenGL::GL
    glfw
)

add_custom_command(
  TARGET game POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:game>/resources
)
