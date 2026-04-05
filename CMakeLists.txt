cmake_minimum_required(VERSION 3.10)
project(lanterns)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(src/
                    src/utilities/
                    lib/glad/include/
                    ${GLFW_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
file(GLOB         VENDOR_SOURCES  lib/glad/src/glad.c)
file(GLOB_RECURSE PROJECT_SHADERS res/shaders/*.vert res/shaders/*.frag)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${VENDOR_SOURCES}
    ${PROJECT_SHADERS}
)

target_link_libraries(${PROJECT_NAME}
    glfw
    glm::glm
    dl
)