# === IMPORTS ===
# Tkinter - para sa UI
# PIL (Pillow) - para sa images (Pokémon silhouettes)
# Threading - para makapag-load ng API data nang hindi nagha-hang yung app
# Custom modules - logic (rules ng game) at API fetching
# Scrollbar/Canvas - special widgets for scrollable content and drawing
# requests - Allows code to connect to websites or APIs and download data.
import tkinter as tk
from tkinter import ttk, messagebox, Scrollbar, Canvas 
from PIL import ImageTk, Image
import threading
from modules.game_logic import PokemonGame
from modules.pokemon_api import get_random_pokemon
from modules.catalogue_viewer import fetch_catalogue_data
import requests
from io import BytesIO # Image downloads come as raw bytes, not files (PIL is dependent here)

# === MAIN APP WINDOW ===
class PokemonApp(tk.Tk):
    def __init__(self):
        # super() is for parent class (tk.Tk) para ma-inherit yung window features
        super().__init__()
        self.title("Who's That Pokémon?")
        self.geometry("430x550") # Keep the fixed window size

        # self is mismong instance ng class (PokemonApp)
        # parang sinasabi natin: "itong specific window na to"
        self.game = PokemonGame()  # game logic instance galing sa ibang module
        self.catalogue_data = None # Store catalogue data once fetched
        self.catalogue_photo_refs = [] # to prevent garbage collection of images

        # Timer variables para sa sabay-sabay na processes (concurrency)
        self.timer_id = None
        self.remaining_time = 0 # coundown
        self.pokemon_silhouette_tk = None #store the Tkinter image object kasi it will disappear if not

        # Container na mag-hohold ng multiple frames (screens)
        self.container = ttk.Frame(self)
        self.container.pack(fill="both", expand=True)

        # Dictionary ng screens (para madali magpalit)
        self.frames = {}
        # Changed the first screen to StartMenuScreen
        for F in (StartMenuScreen, DifficultyScreen, GameScreen, GameOverScreen, CatalogueScreen):
            # Gumagawa ng instance ng bawat screen
            frame = F(self.container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")
        # this code just places the screen on top of each other. parang overlap lang. 

        # Unang screen na lalabas
        self.show_frame(StartMenuScreen)

    # === Function para mag-switch ng screen ===
    def show_frame(self, frame_class):
        frame = self.frames[frame_class]
        frame.tkraise()  # parang “bring to front”

    # === START GAME FLOW ===
    # Start button now triggers the Difficulty Screen
    def show_difficulty_select(self):
        self.show_frame(DifficultyScreen)

    # Starts the actual game round
    def start_game(self, difficulty):
        # i-setup yung rules ng game (difficulty, timer, lives)
        self.game.start_game(difficulty)
        # lipat sa mismong game screen
        self.show_frame(GameScreen)
        # load ng first Pokémon
        self.frames[GameScreen].start_first_round()

    # === CATALOGUE FLOW ===
    def open_catalogue(self):
        # Pass the data to the CatalogueScreen if it's already loaded
        self.frames[CatalogueScreen].load_catalogue(self.catalogue_data)
        self.show_frame(CatalogueScreen)

    def set_catalogue_data(self, data):
        self.catalogue_data = data
        # Immediately open the catalogue once the data is ready
        self.open_catalogue()
        
    def handle_catalogue_error(self, message):
        messagebox.showerror("Catalogue Error", message)
        self.show_frame(StartMenuScreen) # Return to start screen

    # === END GAME ===
    def end_game(self):
        self.stop_timer()
        self.game.game_active = False
        # display results sa Game Over screen
        self.frames[GameOverScreen].display_results()
        self.show_frame(GameOverScreen)

    # === RESTART GAME ===
    def restart_game(self):
        self.show_frame(DifficultyScreen)

    # === TIMER SYSTEM ===
    def start_timer(self):
        # kukunin yung time depende sa difficulty
        self.remaining_time = self.game.timer_duration
        self.stop_timer()  # reset muna any old timer
        self.update_timer_display()
        self.run_timer()

    def run_timer(self):
        # recursive timer — tumatakbo kada 1 second
        if self.remaining_time > 0 and self.game.game_active:
            self.remaining_time -= 1
            self.update_timer_display()
            # self.after → parang built-in delay function ng Tkinter
            self.timer_id = self.after(1000, self.run_timer) # 1k milisecond to 1 second
        elif self.remaining_time == 0 and self.game.game_active:
            # kapag naubos yung time
            self.handle_time_out()

    def stop_timer(self):
        # i-cancel kung may existing timer (para hindi mag-overlap)
        if self.timer_id:
            self.after_cancel(self.timer_id)
            self.timer_id = None

    def update_timer_display(self):
        # update text ng timer sa screen
        self.frames[GameScreen].timer_label.config(
            text=f"Time: {self.remaining_time}s"
        )

    def handle_time_out(self):
        # kapag naubusan ng oras, move to next Pokémon
        
        self.frames[GameScreen].display_result(
            "Time's Up!", # The message to display
            "red", # Text color
            f"It was {self.game.current_pokemon['name']}!", # Full reveal message
            reveal=True # Flag to stop the round
        )
        
        # Game logic handles the life deduction and adding to got_away list
        self.game.handle_timeout()
        
        # Check for game end after the timeout is handled (now in _continue_after_result)
        pass 


# === NEW SCREEN 1: START MENU (Replaces old DifficultyScreen as 1st screen) ===
class StartMenuScreen(ttk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, padding=20)
        self.controller = controller

        ttk.Label(self, text="Who's That Pokémon?", font=("Arial", 24, "bold")).pack(pady=20)
        
        # 1. Start Button: Leads to Difficulty Selection
        ttk.Button(self, text="Start Game", command=controller.show_difficulty_select).pack(fill="x", ipady=15, pady=20)
        
        # 2. Catalogue Button: Leads to Catalogue Screen
        self.catalogue_button = ttk.Button(self, text="View Catalogue", command=self.on_view_catalogue)
        self.catalogue_button.pack(fill="x", ipady=15, pady=5)
        
    def on_view_catalogue(self):
        if self.controller.catalogue_data:
            # If data is already loaded, go straight to the screen
            self.controller.open_catalogue()
        else:
            # If data is not loaded, start the loading process
            self.catalogue_button.config(text="Loading Catalogue...", state="disabled") # para di makapindot
            # Use the concurrent module to fetch data
            fetch_catalogue_data(
                callback=lambda data: self.controller.after(0, self.on_catalogue_loaded_success, data),
                error_callback=lambda msg: self.controller.after(0, self.on_catalogue_loaded_error, msg)
            )
            
    def on_catalogue_loaded_success(self, data):
        self.controller.set_catalogue_data(data)
        self.catalogue_button.config(text="View Catalogue", state="normal") # Reset button
        
    def on_catalogue_loaded_error(self, message):
        self.catalogue_button.config(text="View Catalogue", state="normal") # Reset button
        self.controller.handle_catalogue_error(message)


# === SCREEN 2: DIFFICULTY SELECTION (Now triggered from Start Menu) ===
class DifficultyScreen(ttk.Frame):
# ... (Keep DifficultyScreen class unchanged) ...
    def __init__(self, parent, controller):
        # super() → tawag sa parent class (ttk.Frame)
        # ginagawa ito para ma-inherit yung mga UI methods ng Frame
        super().__init__(parent, padding=20)
        self.controller = controller  # access sa main app (PokemonApp)

        ttk.Label(self, text="Select a difficulty:", font=("Arial", 16)).pack(pady=30)

        # bawat button magtatawag ng start_game() na nasa controller (PokemonApp)
        ttk.Button(self, text="Easy (2:00)", command=lambda: controller.start_game("Easy")).pack(fill="x", ipady=10, pady=5)
        ttk.Button(self, text="Medium (1:00)", command=lambda: controller.start_game("Medium")).pack(fill="x", ipady=10, pady=5)
        ttk.Button(self, text="Hard (0:30)", command=lambda: controller.start_game("Hard")).pack(fill="x", ipady=10, pady=5)
        
        ttk.Separator(self, orient='horizontal').pack(fill='x', pady=20)
        ttk.Button(self, text="Back to Menu", command=lambda: controller.show_frame(StartMenuScreen)).pack(fill="x", ipady=5, pady=5)


# === SCREEN 3: MAIN GAME (Same as before) ===
class GameScreen(ttk.Frame):
# ... (Keep GameScreen class unchanged from previous update) ...
    def __init__(self, parent, controller):
        super().__init__(parent, padding=20)
        self.controller = controller  # para ma-access yung game logic at timer

        # --- Top Info Bar ---
        info_frame = ttk.Frame(self)
        info_frame.pack(fill="x", pady=10)

        self.score_label = ttk.Label(info_frame, text="Caught: 0", font=("Arial", 12))
        self.score_label.pack(side="left", expand=True)

        self.timer_label = ttk.Label(info_frame, text="Time: 0s", font=("Arial", 12, "bold"))
        self.timer_label.pack(side="left", expand=True)

        self.lives_label = ttk.Label(info_frame, text="Lives: 3", font=("Arial", 12))
        self.lives_label.pack(side="right", expand=True)

        # --- Pokémon Image ---
        self.silhouette_label = ttk.Label(self, text="Loading...")
        self.silhouette_label.pack(pady=10)
        
        # --- Result Display Label (NEW) ---
        self.result_label = ttk.Label(self, text="", font=("Arial", 16, "bold"))
        self.result_label.pack(pady=10)
        
        # --- Guess Area ---
        guess_frame = ttk.Frame(self)
        guess_frame.pack(pady=10)

        ttk.Label(guess_frame, text="It's:").pack(side="left", padx=5)
        self.guess_entry = ttk.Entry(guess_frame, font=("Arial", 14))
        self.guess_entry.pack(side="left", padx=5)

        self.guess_entry.bind("<Return>", self.submit_guess)
        self.guess_button = ttk.Button(guess_frame, text="Guess!", command=self.submit_guess)
        self.guess_button.pack(side="left", padx=5)

        # --- Hint & Quit Buttons ---
        button_frame = ttk.Frame(self)
        button_frame.pack(fill="x", pady=10)
        ttk.Button(button_frame, text="Get Hint", command=self.show_hint).pack(side="left", expand=True, padx=10)
        ttk.Button(button_frame, text="Quit Game", command=self.controller.end_game).pack(side="right", expand=True, padx=10)


    def start_first_round(self):
        # reset score/lives display sa simula ng game
        self.lives_label.config(text=f"Lives: {self.controller.game.lives}")
        self.score_label.config(text=f"Caught: 0")
        self.load_next_pokemon()

    def display_result(self, message, color, full_reveal_message=None, reveal=False):
        """
        Displays the result message on the screen for 2 seconds and handles the flow.
        """
        self.controller.stop_timer()

        # eto yung part na nagsshow kung ano yung tamang sagot
        # Update UI with the result
        self.result_label.config(text=message, foreground=color)
        self.guess_entry.config(state="disabled")
        self.guess_button.config(state="disabled")

        if reveal and full_reveal_message:
            # Display the full name if it was a timeout/wrong guess that ends the round
            self.result_label.config(text=f"{message}\n({full_reveal_message})")
            
        # 2000 ms (2 seconds) delay before clearing the message and loading the next round
        # or checking for game over
        self.controller.after(2000, self._continue_after_result)

    def _continue_after_result(self):
        # Clear the message
        self.result_label.config(text="")
        
        # Check for Game Over condition
        if self.controller.game.lives == 0:
            self.controller.end_game()
        else:
            # Load the next Pokémon
            self.load_next_pokemon()


    def submit_guess(self, event=None):
        guess = self.guess_entry.get()
        if not guess:
            return

        # check kung tama o mali ang hula
        result = self.controller.game.check_guess(guess)
        self.guess_entry.delete(0, tk.END)

        pokemon_name = self.controller.game.current_pokemon['name']

        if result == "correct":
            self.display_result(
                "Correct!", 
                "green", 
                full_reveal_message=f"It was {pokemon_name}!",
                reveal=True
            )
        elif result == "wrong":
            self.display_result(
                "Wrong! -1 Life", 
                "red", 
                full_reveal_message=f"It was {pokemon_name}!",
                reveal=True
            )
        # Note: self._continue_after_result handles calling load_next_pokemon() or end_game()


    def show_hint(self):
        hint = self.controller.game.get_hint()
        messagebox.showinfo("Hint", hint)


    def load_next_pokemon(self):
        # disable input habang naglo-load ng bagong Pokémon
        self.guess_entry.config(state="disabled")
        self.guess_button.config(state="disabled")
        self.silhouette_label.config(image="", text="Loading...")

        # threading → para hindi mag-freeze habang kinukuha ang API data
        threading.Thread(target=self._fetch_pokemon_thread, daemon=True).start()

    def _fetch_pokemon_thread(self):
        # kukunin ang random Pokémon sa API (sa background)
        pokemon_data = get_random_pokemon()
        # pagkatapos magload, update UI sa main thread
        self.controller.after(0, self.on_pokemon_loaded, pokemon_data)

    def on_pokemon_loaded(self, pokemon_data):
        if pokemon_data:
            # store data para sa game logic
            self.controller.game.load_new_pokemon_data(pokemon_data)

            # resize at ipakita ang silhouette
            img = pokemon_data["silhouette_image"].resize((250, 250), Image.Resampling.LANCZOS)
            self.controller.pokemon_silhouette_tk = ImageTk.PhotoImage(img)
            self.silhouette_label.config(image=self.controller.pokemon_silhouette_tk, text="")

            # update info labels
            self.lives_label.config(text=f"Lives: {self.controller.game.lives}")
            self.score_label.config(text=f"Caught: {len(self.controller.game.caught_pokemon)}")

            # enable guessing again
            self.guess_entry.config(state="normal")
            self.guess_button.config(state="normal")

            # start timer (sabay habang naglalaro)
            self.controller.start_timer()
        else:
            messagebox.showerror("Error", "Couldn't load Pokémon.")
            self.controller.end_game()


# === SCREEN 4: GAME OVER SCREEN (Same as before) ===
class GameOverScreen(ttk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, padding=20)
        self.controller = controller

        ttk.Label(self, text="Game Over!", font=("Arial", 24, "bold")).pack(pady=20)
        self.score_label = ttk.Label(self, text="Your Score: 0", font=("Arial", 18))
        self.score_label.pack(pady=10)
        self.results_label = ttk.Label(self, text="", font=("Arial", 12))
        self.results_label.pack(pady=20)

        ttk.Button(self, text="Play Again", command=self.controller.restart_game).pack(ipady=10, pady=20)

    def display_results(self):
        # kinukuha lahat ng score at list ng Pokémon caught/got away
        results = self.controller.game.get_score()
        self.score_label.config(text=f"Your Score: {results['score']}")

        caught = ", ".join(results['caught']) or "None"
        got_away = ", ".join(results['got_away']) or "None"

        self.results_label.config(text=f"You Caught:\n{caught}\n\nGot Away:\n{got_away}")

#Screen 5: CatalogueScreen
class CatalogueScreen(ttk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent, padding=10) 
        self.controller = controller
        
        # Title
        ttk.Label(self, text="Pokémon Catalogue (Gen 1)", font=("Arial", 20, "bold")).pack(pady=10)
        
        # --- Setup Scrollable Canvas ---
        scroll_container = ttk.Frame(self)
        scroll_container.pack(fill="both", expand=True, padx=5, pady=5) 
        
        # Create Canvas and Scrollbar
        self.canvas = tk.Canvas(scroll_container, borderwidth=0, highlightthickness=0, height=400)
        self.scrollbar = ttk.Scrollbar(scroll_container, orient="vertical", command=self.canvas.yview)
        
        # Inner frame to hold the catalogue items
        self.scrollable_frame = ttk.Frame(self.canvas)

        # 1. TELL THE CANVAS TO SCROLL THE FRAME
        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )

        #Create the window inside the canvas
        
        self.window_id = self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")

        # Force the inner frame to match the Canvas width
        # This ensures the list items shrink slightly to make room for the scrollbar
        self.canvas.bind(
            '<Configure>', 
            lambda e: self.canvas.itemconfig(self.window_id, width=e.width)
        )

        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        #Scrollbar on right, Canvas fills the rest
        self.scrollbar.pack(side="right", fill="y")
        self.canvas.pack(side="left", fill="both", expand=True)
        
        # Back Button
        ttk.Button(self, text="Back to Menu", command=lambda: controller.show_frame(StartMenuScreen)).pack(fill="x", ipady=5, pady=(5, 10))
        
    def load_catalogue(self, data):
        # Clear existing widgets
        for widget in self.scrollable_frame.winfo_children():
            widget.destroy()
        
        self.controller.catalogue_photo_refs = []

        if not data:
            ttk.Label(self.scrollable_frame, text="Catalogue data is not available.").pack(pady=20)
            return

        # Loop to create rows
        for entry in data:
            # Container frame for one Pokémon
            entry_frame = ttk.Frame(self.scrollable_frame, padding=5, relief="solid", borderwidth=1)
            entry_frame.pack(fill="x", padx=2, pady=2) # padx=2 keeps it clean inside the canvas
            
            # ID
            ttk.Label(entry_frame, text=f"#{entry['id']:03d}", font=("Arial", 10, "bold"), width=4).pack(side="left", padx=5)

            # Image
            sprite_tk = ImageTk.PhotoImage(entry['sprite_image'])
            self.controller.catalogue_photo_refs.append(sprite_tk)
            
            lbl_img = ttk.Label(entry_frame, image=sprite_tk)
            lbl_img.image = sprite_tk 
            lbl_img.pack(side="left", padx=10)
            
            # Name
            ttk.Label(entry_frame, text=entry['name'], font=("Arial", 12), width=12, anchor="w").pack(side="left")

            # Types,Right Aligned
            types_text = " / ".join(entry['types'])
            
            # FIX: Added extra padding on the right (padx=(0, 15)) so it doesn't touch the scrollbar
            ttk.Label(entry_frame, text=types_text, font=("Arial", 10), anchor="e").pack(side="right", padx=(0, 15))

        # Force update layout
        self.scrollable_frame.update_idletasks()


# === START APP ===
if __name__ == "__main__":
    app = PokemonApp()
    app.mainloop()
