#include "pokeapi.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <future>

using json = nlohmann::json;

// Callback for libcurl to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

// Async fetch a single Pokémon
std::future<Pokemon> PokeAPI::fetchPokemon(const std::string& query) {
    return std::async(std::launch::async, [query]() {
        CURL* curl = curl_easy_init();
        std::string url = "https://pokeapi.co/api/v2/pokemon/" + query;
        std::string response;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                return parsePokemonData(response);
            } else {
                throw std::runtime_error("Failed to fetch data from PokeAPI");
            }
        }
        throw std::runtime_error("CURL initialization failed");
    });
}

// Fetch multiple Pokémon concurrently
std::vector<std::future<Pokemon>> PokeAPI::fetchMultiplePokemon(int count) {
    std::vector<std::future<Pokemon>> futures;
    for (int i = 1; i <= count; ++i) {
        futures.push_back(fetchPokemon(std::to_string(i)));
    }
    return futures;
}

// Async fetch Pokemon with description
std::future<Pokemon> PokeAPI::fetchPokemonWithDescription(const std::string& query) {
    return std::async(std::launch::async, [query]() {
        Pokemon p = fetchPokemon(query).get();
        p.description = fetchSpeciesDescription(p.id);
        return p;
    });
}

// Fetch regions
std::future<std::vector<Region>> PokeAPI::fetchRegions() {
    return std::async(std::launch::async, []() {
        CURL* curl = curl_easy_init();
        std::string url = "https://pokeapi.co/api/v2/region";
        std::string response;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if (res == CURLE_OK) {
                json data = json::parse(response);
                std::vector<Region> regions;
                for (const auto& reg : data["results"]) {
                    Region r;
                    r.id = regions.size() + 1;  // Simple ID
                    r.name = reg["name"];
                    // Fetch Pokedex for this region to get Pokemon list
                    std::string pokedex_url = "https://pokeapi.co/api/v2/pokedex/" + r.name;
                    std::string pokedex_response;
                    curl = curl_easy_init();
                    curl_easy_setopt(curl, CURLOPT_URL, pokedex_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pokedex_response);
                    curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                    json pokedex_data = json::parse(pokedex_response);
                    for (const auto& entry : pokedex_data["pokemon_entries"]) {
                        r.pokemon_names.push_back(entry["pokemon_species"]["name"]);
                    }
                    regions.push_back(r);
                }
                return regions;
            }
        }
        throw std::runtime_error("Failed to fetch regions");
    });
}

// Helper to fetch description from species
std::string PokeAPI::fetchSpeciesDescription(int id) {
    CURL* curl = curl_easy_init();
    std::string url = "https://pokeapi.co/api/v2/pokemon-species/" + std::to_string(id);
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK) {
            json data = json::parse(response);
            for (const auto& flavor : data["flavor_text_entries"]) {
                if (flavor["language"]["name"] == "en") {
                    return flavor["flavor_text"];
                }
            }
        }
    }
    return "Description not available.";
}

// Parse JSON response into Pokemon struct
Pokemon PokeAPI::parsePokemonData(const std::string& json_str) {
    json data = json::parse(json_str);
    Pokemon p;
    p.id = data["id"];
    p.name = data["name"];
    p.height = data["height"];
    p.weight = data["weight"];
    p.sprite_url = data["sprites"]["front_default"];

    for (const auto& type : data["types"]) {
        p.types.push_back(type["type"]["name"]);
    }

    for (const auto& stat : data["stats"]) {
        Stat s;
        s.name = stat["stat"]["name"];
        s.base_stat = stat["base_stat"];
        p.stats.push_back(s);
    }

    return p;
}