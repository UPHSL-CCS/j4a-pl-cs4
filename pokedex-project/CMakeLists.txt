cmake_minimum_required(VERSION 3.5)
project(pokedex-project)

# 1. Find vcpkg libraries
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 2. Find Qt libraries 
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_executable(pokedex-project 
    main.cpp
    modules/pokeapi.cpp
    # Add other cpp files here if needed
)

# 3. Link everything together
target_link_libraries(pokedex-project PRIVATE 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    Qt6::Widgets 
)