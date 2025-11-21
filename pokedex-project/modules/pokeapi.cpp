#include "pokeapi.h"

// CRITICAL: These defines must be at the very top to fix the "ambiguous byte" error
#define WIN32_LEAN_AND_MEAN

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <future>
#include <vector>

using json = nlohmann::json;

// Callback for libcurl to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
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
            try {
                json data = json::parse(response);
                // Check if key exists to avoid crash
                if (data.contains("flavor_text_entries")) {
                    for (const auto& flavor : data["flavor_text_entries"]) {
                        if (flavor["language"]["name"] == "en") {
                            // Clean up the text (remove newlines/form feeds)
                            std::string desc = flavor["flavor_text"].get<std::string>();
                            return desc; 
                        }
                    }
                }
            } catch (...) {
                return "Error parsing description.";
            }
        }
    }
    return "Description not available.";
}

// Parse JSON response into Pokemon struct
// Note: This matches the return type in the header now (Pokemon, not string)
Pokemon PokeAPI::parsePokemonData(const std::string& json_str) {
    json data = json::parse(json_str);
    Pokemon p;
    
    // Safely get values with defaults if missing
    p.id = data.value("id", 0);
    p.name = data.value("name", "Unknown");
    p.height = data.value("height", 0);
    p.weight = data.value("weight", 0);
    
    if (data.contains("sprites") && data["sprites"].contains("front_default") && !data["sprites"]["front_default"].is_null()) {
        p.sprite_url = data["sprites"]["front_default"].get<std::string>();
    } else {
        p.sprite_url = "";
    }

    if (data.contains("types")) {
        for (const auto& type : data["types"]) {
            p.types.push_back(type["type"]["name"].get<std::string>());
        }
    }

    if (data.contains("stats")) {
        for (const auto& stat : data["stats"]) {
            Stat s;
            s.name = stat["stat"]["name"].get<std::string>();
            s.base_stat = stat["base_stat"].get<int>();
            p.stats.push_back(s);
        }
    }

    return p;
}

// Async fetch a single Pokémon
std::future<Pokemon> PokeAPI::fetchPokemon(const std::string& query) {
    // std::launch::async forces a new thread
    return std::async(std::launch::async, [query]() {
        CURL* curl = curl_easy_init();
        std::string url = "https://pokeapi.co/api/v2/pokemon/" + query;
        std::string response;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            
            // IMPORTANT: Set User-Agent, some APIs reject requests without it
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                return parsePokemonData(response);
            } else {
                // Return a dummy pokemon on error or throw
                Pokemon errorPoke;
                errorPoke.name = "Error: " + std::string(curl_easy_strerror(res));
                return errorPoke; 
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
        // First fetch the basic data
        Pokemon p = fetchPokemon(query).get();
        // Then fetch the species description using the ID we just got
        if (p.id != 0) {
            p.description = fetchSpeciesDescription(p.id);
        }
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
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == CURLE_OK) {
                json data = json::parse(response);
                std::vector<Region> regions;
                
                for (const auto& reg : data["results"]) {
                    Region r;
                    r.id = (int)regions.size() + 1;
                    r.name = reg["name"].get<std::string>();
                    
                    // NOTE: Fetching details for EVERY region inside this loop 
                    // will be very slow. Consider doing this separately or lazily.
                    
                    regions.push_back(r);
                }
                return regions;
            }
        }
        return std::vector<Region>{}; // Return empty vector on failure
    });
}

std::future<std::vector<std::string>> PokeAPI::fetchPokemonByRegion(const std::string& regionName) {
    return std::async(std::launch::async, [regionName]() {
        CURL* curl = curl_easy_init();
        std::string url = "https://pokeapi.co/api/v2/region/" + regionName;
        std::string response;
        
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == CURLE_OK) {
                json data = json::parse(response);
                std::vector<std::string> pokemonNames;
                
                // Get the pokedexes for this region
                if (data.contains("pokedexes") && !data["pokedexes"].empty()) {
                    // Fetch the first pokedex for this region
                    std::string pokedexUrl = data["pokedexes"][0]["url"].get<std::string>();
                    
                    // Fetch pokedex details
                    CURL* curl2 = curl_easy_init();
                    std::string pokedexResponse;
                    
                    if (curl2) {
                        curl_easy_setopt(curl2, CURLOPT_URL, pokedexUrl.c_str());
                        curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, WriteCallback);
                        curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &pokedexResponse);
                        curl_easy_setopt(curl2, CURLOPT_USERAGENT, "libcurl-agent/1.0");
                        
                        CURLcode res2 = curl_easy_perform(curl2);
                        curl_easy_cleanup(curl2);
                        
                        if (res2 == CURLE_OK) {
                            json pokedexData = json::parse(pokedexResponse);
                            if (pokedexData.contains("pokemon_entries")) {
                                for (const auto& entry : pokedexData["pokemon_entries"]) {
                                    pokemonNames.push_back(entry["pokemon_species"]["name"].get<std::string>());
                                }
                            }
                        }
                    }
                }
                return pokemonNames;
            }
        }
        return std::vector<std::string>{};
    });
}