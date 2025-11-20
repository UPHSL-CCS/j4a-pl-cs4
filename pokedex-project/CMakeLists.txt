# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(pokedex-project)

# Find the installed packages
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED) # nlohmann_json uses the CONFIG mode

# Add your executable target (replace main.cpp with your source file)
add_executable(pokedex-project
    main.cpp 
    modules/pokeapi.cpp)

# Link the libraries to your executable target
target_link_libraries(pokedex-project PRIVATE 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    Qt6::Widgets
)