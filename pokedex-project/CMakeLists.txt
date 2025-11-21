cmake_minimum_required(VERSION 3.5)

# =================================================================
# 1. VCPKG CONFIGURATION
# =================================================================
set(VCPKG_ROOT "C:/dev/vcpkg")

# CRITICAL FIX: Force VCPKG to use the MinGW Triplet
# This matches your Qt Kit (MinGW 64-bit)
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")

if(EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Loading VCPKG for MinGW from: ${VCPKG_ROOT}")
    include("${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "Could not find VCPKG. Check path: ${VCPKG_ROOT}")
endif()
# =================================================================

# 2. Initialize Project (Detects MinGW Compiler)
project(pokedex-project)

# 3. Find Libraries (Now looking for x64-mingw-dynamic versions)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 4. Find Qt libraries 
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Concurrent Network)

# 5. Add Source Files
add_executable(pokedex-project 
    main.cpp
    modules/pokeapi.cpp
    modules/pokeui.cpp  
)

# 6. Link Libraries
target_link_libraries(pokedex-project PRIVATE 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    Qt6::Core         
    Qt6::Gui          
    Qt6::Widgets      
    Qt6::Concurrent   
    Qt6::Network      
)