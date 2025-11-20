#ifndef pokeapi_h
#define pokeapi_h

#include <string>
#include <vector>
#include <future>
#include "pokedata.h"
using namespace std;

class PokeAPI {
public:
    static future<Pokemon> fetchPokemon(const string& query);
    static vector<future<Pokemon>> fetchMultiplePokemons(int count);
    static future<Pokemon> fetchPokemonDescription(const string& query);
    static future<vector<Region>> fetchRegions();
private:
    static string parsePokemonData(const string& json);
    static string fetchSpeciesDescription(int id);
};

#endif