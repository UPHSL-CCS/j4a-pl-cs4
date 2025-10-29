# Who’s That Pokémon? 🎮

A simple Python game made by **Jornales**, **Aninang**, and **Cabanes**.  
This project combines **Concurrency**, **Modularity / Subprograms**, and **Abstraction** concepts.

---

## 🧠 Project Description
“Who’s That Pokémon?” is a guessing game where the player tries to name a Pokémon from its shadow image.  
The player chooses a **difficulty level** (Easy, Medium, Hard) that sets the **time limit** per round.

If the player types the correct Pokémon name before time runs out, they earn a point.  
If the answer is wrong, they lose a life (3 lives total).  
If time runs out, the Pokémon “gets away.”  
The game ends when all lives are gone, and the final results show how many Pokémon were caught and how many got away.

---

## ⚙️ Features
- **Concurrency:** The timer runs in real-time using `after()` and threads.  
- **Modularity:** The code is organized into multiple files (`main_ui.py`, `game_logic.py`, `pokemon_api.py`).  
- **Abstraction:** Each module hides its internal details (e.g., Pokémon data fetching, game rules, UI display).
