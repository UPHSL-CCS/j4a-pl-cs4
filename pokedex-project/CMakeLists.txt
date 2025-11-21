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

# Enable Qt's automatic MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add modules directory to include path
include_directories(${CMAKE_SOURCE_DIR}/modules)

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
    modules/pokeui.h    
    modules/pokeapi.h
    modules/pokedata.h
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