#include <iostream>
#include <string>
using namespace std;
#include "modules/pokeapi.h"

// Testing code for api fetches (hoping it works)
int main() {
    try {

        auto future_pokemon = PokeAPI::fetchPokemon("Lucario");
        Pokemon p = future_pokemon.get();

        cout << "ID: " << p.id << endl;
        cout << "Name: " << p.name << endl;
        cout << "Height: " << p.height << endl;
        cout << "Weight: " << p.weight << endl;
        cout << "Types: ";
        for (const auto& type : p.types) {
            cout << type << " ";
        }
        cout << endl;
        cout << "Stats: " << endl;
        for (const auto& stat : p.stats) {
            cout << "  " << stat.name << ": " << stat.base_stat << endl;
        }

        cout << "Sprite URL: " << p.sprite_url << endl;
        
        cout << "Description: " << p.description << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cout << "Failed to fetch Pokemon data." << endl;
    }
    return 0;
}

