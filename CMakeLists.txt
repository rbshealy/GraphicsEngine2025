cmake_minimum_required(VERSION 3.29)
project(GraphicsEngine2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PROJECT_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(app ${PROJECT_SRC})

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external/glm
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(app PRIVATE glm::glm)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(app PRIVATE glfw)

find_package(Vulkan REQUIRED)
target_link_libraries(app PRIVATE Vulkan::Vulkan)

