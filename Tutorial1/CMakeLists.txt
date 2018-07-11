cmake_minimum_required(VERSION 3.12.0)

set(SRC_FILES
        src/main.c)

add_executable(Tutorial1
        ${SRC_FILES})

find_package(Vulkan REQUIRED)

target_include_directories(Tutorial1
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(Tutorial1
        PRIVATE Vulkan::Vulkan)

if (WIN32)
    target_compile_definitions(Tutorial1
            PUBLIC VK_USE_PLATFORM_WIN32_KHR)
endif ()
