cmake_minimum_required(VERSION 3.29)
project(GraphicsEngine2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PROJECT_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(app ${PROJECT_SRC})

find_program(GLSLC_EXECUTABLE NAMES glslc
        HINTS "$ENV{VULKAN_SDK}/Bin"
)

if (NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found! Make sure VULKAN_SDK and PATH are set.")
endif()

message(STATUS "Found glslc: ${GLSLC_EXECUTABLE}")

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)

file(GLOB SHADERS
        ${SHADER_DIR}/*.vert
        ${SHADER_DIR}/*.frag
)

set(SPIRV_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)

foreach(SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV_FILE ${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv)

    add_custom_command(
            OUTPUT ${SPIRV_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SPIRV_OUTPUT_DIR}
            COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${SPIRV_FILE}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILE_NAME} -> ${SPIRV_FILE}"
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach()

add_custom_target(compile_shaders ALL DEPENDS ${SPIRV_BINARY_FILES})

add_dependencies(app compile_shaders)

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

