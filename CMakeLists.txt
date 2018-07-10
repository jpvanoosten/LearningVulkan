cmake_minimum_required(VERSION 3.12.0)

# Enable parallel compilation
if (MSVC)
    add_compile_options(/MP)
elseif (MINGW)
    include(ProcessorCount)
    ProcessorCount(N)
    add_compile_options(-j${N})
endif ()

# Make sure DLL and EXE targets go to the same directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

project(LearningVulkan LANGUAGES C)

add_subdirectory(Tutorial1)

# Set the startup project.
set_directory_properties(PROPERTIES
        VS_STARTUP_PROJECT Tutorial1
        )

