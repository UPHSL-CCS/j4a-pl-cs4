#include <iostream>
#include <string>
using namespace std;
#include "modules/pokeapi.h"

// Testing code for api fetches (hoping it works)
int main() {
    try {

        auto future_pokemon = PokeAPI::fetchPokemonWithDescription("Lucario");
        Pokemon p = future_pokemon.get();
        cout << "Fetched: " << p.name << std::endl;
        cout << "Description: " << p.description << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cout << "Failed to fetch Pokemon data." << endl;
    }
    return 0;
}

