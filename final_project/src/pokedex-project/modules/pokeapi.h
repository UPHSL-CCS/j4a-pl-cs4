#ifndef pokeapi_h
#define pokeapi_h

#include <string>
#include <vector>
#include <future>
#include "pokedata.h"

// REMOVED: using namespace std; (This fixes the "ambiguous byte" error)

class PokeAPI {
public:
    static std::future<Pokemon> fetchPokemon(const std::string& query);
    
    static std::future<Pokemon> fetchPokemonWithDescription(const std::string& query);

    static std::future<std::vector<RegionWithPokedex>> fetchRegionsWithPokedexUrls();

    static std::future<std::vector<std::string>> fetchPokemonByRegion(const std::string& regionName);

    static std::future<std::vector<std::string>> fetchPokedexPokemon(const std::string& pokedexUrl);
private:
    // Fixed return type: Returns 'Pokemon' object, not 'string'
    static Pokemon parsePokemonData(const std::string& json); 

    static std::string fetchSpeciesDescription(int id); 
};

#endif
