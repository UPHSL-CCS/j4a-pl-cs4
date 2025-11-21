#ifndef pokeapi_h
#define pokeapi_h

#include <string>
#include <vector>
#include <future>
#include "pokedata.h"

// REMOVED: using namespace std; (This fixes the "ambiguous byte" error)

class PokeAPI {
public:
    // Fixed name to match .cpp: fetchMultiplePokemon (Singular)
    static std::vector<std::future<Pokemon>> fetchMultiplePokemon(int count); 

    static std::future<Pokemon> fetchPokemon(const std::string& query);
    
    // Fixed name to match .cpp: fetchPokemonWithDescription
    static std::future<Pokemon> fetchPokemonWithDescription(const std::string& query);

    static std::future<std::vector<Region>> fetchRegions();

private:
    // Fixed return type: Returns 'Pokemon' object, not 'string'
    static Pokemon parsePokemonData(const std::string& json); 

    static std::string fetchSpeciesDescription(int id); 
};

#endif