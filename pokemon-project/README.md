# 🚀 Who's That Pokémon? - Concurrently Optimized Trivia System

## 🌟 Project Overview

The **"Who's That Pokémon?"** application is a desktop-based interactive trivia game designed to test a user's visual memory and trivia knowledge of the original 151 Pokémon. The system has been significantly improved and extended from its midterm iteration to demonstrate mastery of **modularity, concurrency, and professional software engineering practices**.

The application challenges users to identify Pokémon from their silhouettes under time pressure, utilizing real-time data fetched from the **PokéAPI**. A key focus of this final project was to ensure a responsive and smooth user experience by implementing robust threading for all external data operations.

## ✨ Key Features and Improvements (Final Project Deliverables)

This section highlights the required new elements and improvements implemented since the Midterm Presentation:

### 1. Improved Structure & Modularity (Refactoring)

The project adheres to modern software design principles, ensuring a clear separation of concerns (SoC) for enhanced maintainability and scalability.

* **File Separation:** The system logic is cleanly separated into a dedicated `/modules` directory:
    * `/modules/game_logic.py`: Manages game state (score, lives, timer rules).
    * `/modules/pokemon_api.py`: Handles fetching random Pokémon data and performing the **CPU-intensive silhouette image creation**.
    * `/modules/catalogue_viewer.py`: Manages the concurrent fetching of data for all 151 Pokémon for the catalogue feature.
* **Abstraction and Naming:** Functions and variables use clear, descriptive naming conventions (e.g., `_fetch_pokemon_thread`, `display_result`).

### 2. Concurrency & Performance Optimization

All heavy I/O and processing tasks are executed off the main thread, ensuring the GUI remains fast and responsive.

* **Asynchronous API Handling:** **Python's `threading` module** is used in `main.py` to fetch random Pokémon data from the PokéAPI in the background (`_fetch_pokemon_thread`).
* **Catalogue Concurrency:** The **Catalogue Viewer** utilizes threading to fetch and process data for **all 151 Pokémon concurrently**, preventing the UI from freezing during the initial heavy data load.
* **Optimized Image Processing (Improvement):** The CPU-intensive **silhouette creation** process is executed within the background thread (`pokemon_api.py`), rather than on the main UI thread, ensuring optimal performance when loading new rounds.

### 3. Added Features to Show System Flexibility

* **Catalogue Viewer (Expanded Functionality):** A dedicated screen that allows users to view the complete list of Pokémon, including their **sprite image**, **ID**, and **Type(s)**, loaded concurrently for reference.
* **Refined Start Menu (New Process Flow):** The application now starts with a simple menu offering two distinct paths: **"Start Game"** (leading to difficulty selection) and **"View Catalogue"** (leading to the new reference tool).
* **Multi-tiered Difficulty:** Configurable behavior across **Easy, Medium, and Hard** modes, modifying the available time limit.

### 4. Usability and Error Handling Enhancements

* **Cleaner UI/UX (Feedback):** The disruptive `tkinter.messagebox` alerts for correct/wrong guesses have been eliminated. Results are now displayed using a **smooth, temporary in-app message** (e.g., "Correct!" in green) with a 2-second delay before seamlessly advancing to the next round.
* **Improved Menu Navigation:** Clear **"Back to Menu"** buttons are provided across all non-game screens for easy flow control.
* **Fixed Layout:** The Catalogue Viewer utilizes Tkinter's canvas and frame management to ensure a fixed window size with a dedicated, scrollable area, maintaining a clean and professional appearance.

## 🚀 How to Run the Application

1.  **Prerequisites:** Ensure you have **Python 3.10+** installed.
2.  **Dependencies:** Install the required external libraries:
    ```bash
    pip install requests Pillow
    ```
3.  **Execution:** Run the main application file:
    ```bash
    python main.py
    ```

## 👤 Developers & Ethics

The code adheres to professional ethical standards. All data is sourced exclusively from the public, community-maintained PokéAPI. Source code management was conducted via **GitHub Classroom**, ensuring ethical conduct in collaboration and version control.

- **Angela Cabanes**
- **Kelvin Adam Aninang**
- **Michaela Jornales**
---

