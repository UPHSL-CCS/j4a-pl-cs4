#ifndef pokedata_h
#define pokedata_h

#include <string>
#include <vector>

struct Stat {
    std::string name;
    int base_stat;
};

struct Pokemon {
    int id;
    std::string name;
    std::vector<std::string> types;
    int height;
    int weight;
    std::vector<Stat> stats;
    std::string description;
    std::string sprite_url;
};

struct Region {
    int id;
    std::string name;
    std::vector<std::string> pokemon_names;
};

#endif