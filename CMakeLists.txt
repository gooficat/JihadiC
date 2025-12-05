cmake_minimum_required(VERSION 3.15)
    project(jihadi-c LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    # Include directories
    include_directories(include)
    
    # Add executable
    add_executable(jihadi-c 
        src/main.c
        src/data.c
        src/specification.c
        src/token.c
    )
    
    # Compiler-specific options
    if(MSVC)
        target_compile_options(jihadi-c PRIVATE /W4 /fsanitize=address)
    else()
        target_compile_options(jihadi-c PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address)
        target_link_options(jihadi-c PRIVATE -fsanitize=address)
    endif()
    
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(jihadi-c PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(jihadi-c PRIVATE -g -O0)
        endif()
    endif()
    
    # Enable testing
    enable_testing()
    
    # Install target
    #install(TARGETS jihadi-c  RUNTIME DESTINATION bin)
        