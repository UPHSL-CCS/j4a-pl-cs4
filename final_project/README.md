# 👾 Pokémon Application Suite

**University:** UPHSL College of Computer Studies  
**Course:** Programming Languages Laboratory (CS4)  
**Date:** November 2025  
**Instructor:** Prof. Grachel Eliza Greta  

## 👥 Contributors
* **Kelvin Adam Aninang**
* **Angela Cabanes**
* **Michaela Jornaes**

---

## 📖 Executive Summary
This repository contains two comprehensive desktop applications demonstrating advanced concepts in **Python** and **C++**. Both projects utilize the [PokéAPI](https://pokeapi.co/) to provide real-time data fetching, featuring concurrent programming models to ensure responsive User Interfaces (UI).

1.  **Who's That Pokémon?** (Python) - An interactive trivia game testing visual memory using silhouette identification.
2.  **PokeDex** (C++) - A robust encyclopedia application for cataloging and searching Pokémon data.

---

## 🎮 Project 1: Who's That Pokémon? (Python)

### Overview
A desktop-based interactive trivia system inspired by the classic commercial segment. It challenges users to identify Pokémon based on their silhouettes. The application focuses on gamification, dynamic data fetching, and a responsive UI using threading.

### Key Features
* **Game Modes**: Difficulty selection (Easy, Medium, Hard) that dynamically adjusts countdown timers.
* **Gameplay Mechanics**: Lives tracking, real-time scoring, and a hint mechanism.
* **Catalogue Viewer**: A standalone screen to review the complete list of Generation 1 Pokémon (sprites, IDs, types).
* **Non-Blocking UI**: Uses background worker threads for heavy I/O operations (API calls, image processing) to prevent freezing.

### Technical Stack
| Category | Tool / Library | Purpose |
| :--- | :--- | :--- |
| **Language** | Python 3.11+ | Core application logic |
| **GUI** | Tkinter | Modular graphical user interface |
| **Image Processing** | Pillow (PIL) | Silhouette creation and sprite resizing |
| **Networking** | Requests | Asynchronous API communication |
| **Concurrency** | Threading | Background task management |

### Architecture
The system follows a modular design:
* `main.py`: Entry point and UI controller.
* `game_logic.py`: Manages rules, scoring, and lives.
* `pokemon_api.py`: Handles API fetching and image manipulation.
* `catalogue_viewer.py`: Manages concurrent fetching for the catalogue list.

---

## 📕 Project 2: PokeDex (C++)

### Overview
A comprehensive encyclopedia designed to educate users about Pokémon species. It addresses the challenge of navigating vast datasets by centralizing stats, physical attributes, and species data into an organized, searchable format.

### Key Features
* **Regional Filtering**: Filter and view Pokémon lists based on specific regions.
* **Detailed Information**: View base stats, species characteristics, height, and weight for selected Pokémon.
* **Search Functionality**: Lookup Pokémon by Name or ID Number.
* **High Performance**: Utilizes C++ concurrency to fetch 1000+ entries without UI lag.

### Technical Stack
| Category | Tool / Library | Purpose |
| :--- | :--- | :--- |
| **Language** | C++ (23) | Main implementation language |
| **Framework** | Qt 6.10 | Frontend development and concurrency |
| **Networking** | Libcurl | HTTP response handling |
| **Data Handling** | nlohmann-json | JSON data manipulation |

### Architecture
The software is structured into three primary layers:
1.  **Data Layer**: Blueprints (`pokedata.h`) structuring API and UI data.
2.  **API Layer**: Communicates with PokeAPI using Libcurl to fetch and convert data via JSON.
3.  **UI Layer**: Visualizes data using Qt Widgets and manages concurrency via `QtConcurrent`.

---

## ⚖️ Ethical & Professional Reflection
**How did your team ensure ethical collaboration (no plagiarism, fair contribution)?**
We utilized Git and GitHub to maintain a transparent history of contributions. Each member’s work is verifiable through commit logs, ensuring accountability. To avoid plagiarism, we strictly adhered to Open Source licensing by properly importing and attributing external libraries rather than copying source code directly. We also divided the project using a Modular Architecture UI, Logic,  API, which naturally enforced fair division of labor without overlapping code. We also contributed to each other's documentation and user interface development, balancing the workload across the different languages (Python and C++) used by the group.

**How does your system ensure data privacy (if applicable) and responsible programming?**
While our system does not store personal information, we practiced Data Minimization by keeping game states in ephemeral memory rather than writing to persistent storage. Regarding responsible programming, we implemented API stewardship. Instead of spamming the PokéAPI with requests on every frame update, we fetch data only when necessary and cache results where possible to respect the provider's server resources and rate limits

**What lessons can you apply from professional practice and version control ethics?**
We applied the industry standard of making small, descriptive changes rather than bulk, vague uploads and making each draft branches (e.g. draft-angela, draft-aninang, and draft-mickz). This ensures that if a bug is introduced, we can easily rollback without breaking the entire build.
