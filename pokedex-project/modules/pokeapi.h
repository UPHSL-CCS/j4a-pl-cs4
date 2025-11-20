#ifndef pokeapi_h
#define pokeapi_h

#include <string>
#include <vector>
#include <future>
#include "pokedata.h"
using namespace std;

class PokeAPI {
public:
    static future<Pokemon> fetchPokemon(const string& query); // async fetch for concurrency
    static vector<future<Pokemon>> fetchMultiplePokemons(int count); // fetch multiple pokemons concurrently
    static future<Pokemon> fetchPokemonDescription(const string& query);
    static future<vector<Region>> fetchRegions();
private:
    static string parsePokemonData(const string& json); // helper for parsing pokemon data from JSON
    static string fetchSpeciesDescription(int id); // helper for fetching species description by id
};

#endif