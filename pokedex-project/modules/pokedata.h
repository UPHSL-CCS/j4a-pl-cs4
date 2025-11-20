#ifndef pokedata_h
#define pokedata_h

#include <string>
#include <vector>
using namespace std;

struct Stat {
    string name;
    int base_stat;
};

struct Pokemon {
    int id;
    string name;
    vector<string> types;
    int height;
    int weight;
    vector<Stat> stats;
    string description;
    string sprite_url;
};

struct Region {
    int id;
    string name;
    string pokemon_names;
};

#endif