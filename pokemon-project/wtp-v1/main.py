# === IMPORTS ===
# Tkinter - para sa UI
# PIL (Pillow) - para sa images (Pokémon silhouettes)
# Threading - para makapag-load ng API data nang hindi nagha-hang yung app
# Custom modules - logic (rules ng game) at API fetching
import tkinter as tk
from tkinter import ttk, messagebox
from PIL import ImageTk, Image
import threading
from modules.game_logic import PokemonGame
from modules.pokemon_api import get_random_pokemon


# === MAIN APP WINDOW ===
class PokemonApp(tk.Tk):
    def __init__(self):
        # super() is for parent class (tk.Tk) para ma-inherit yung window features
        super().__init__()
        self.title("Who's That Pokémon?")
        self.geometry("400x550")

        # self is mismong instance ng class (PokemonApp)
        # parang sinasabi natin: "itong specific window na to"
        self.game = PokemonGame()  # game logic instance galing sa ibang module

        # Timer variables para sa sabay-sabay na processes (concurrency)
        self.timer_id = None
        self.remaining_time = 0
        self.pokemon_silhouette_tk = None

        # Container na mag-hohold ng multiple frames (screens)
        self.container = ttk.Frame(self)
        self.container.pack(fill="both", expand=True)

        # Dictionary ng screens (para madali magpalit)
        self.frames = {}
        for F in (DifficultyScreen, GameScreen, GameOverScreen):
            # Gumagawa ng instance ng bawat screen
            frame = F(self.container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        # Unang screen na lalabas
        self.show_frame(DifficultyScreen)


    # === Function para mag-switch ng screen ===
    def show_frame(self, frame_class):
        frame = self.frames[frame_class]
        frame.tkraise()  # parang “bring to front”


    # === START GAME ===
    def start_game(self, difficulty):
        # i-setup yung rules ng game (difficulty, timer, lives)
        self.game.start_game(difficulty)
        # lipat sa mismong game screen
        self.show_frame(GameScreen)
        # load ng first Pokémon
        self.frames[GameScreen].start_first_round()


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
        messagebox.showwarning("Time's Up!", f"It was {self.game.current_pokemon['name']}!")
        self.game.handle_timeout()
        if self.game.lives == 0:
            self.end_game()
        else:
            self.frames[GameScreen].load_next_pokemon()


# === SCREEN 1: DIFFICULTY SELECTION ===
class DifficultyScreen(ttk.Frame):
    def __init__(self, parent, controller):
        # super() → tawag sa parent class (ttk.Frame)
        # ginagawa ito para ma-inherit yung mga UI methods ng Frame
        super().__init__(parent, padding=20)
        self.controller = controller  # access sa main app (PokemonApp)

        ttk.Label(self, text="Who's That Pokémon?", font=("Arial", 24, "bold")).pack(pady=20)
        ttk.Label(self, text="Select a difficulty:", font=("Arial", 16)).pack(pady=10)

        # bawat button magtatawag ng start_game() na nasa controller (PokemonApp)
        ttk.Button(self, text="Easy (2:00)", command=lambda: controller.start_game("Easy")).pack(fill="x", ipady=10, pady=5)
        ttk.Button(self, text="Medium (1:00)", command=lambda: controller.start_game("Medium")).pack(fill="x", ipady=10, pady=5)
        ttk.Button(self, text="Hard (0:30)", command=lambda: controller.start_game("Hard")).pack(fill="x", ipady=10, pady=5)


# === SCREEN 2: MAIN GAME ===
class GameScreen(ttk.Frame):
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
        self.silhouette_label.pack(pady=20, expand=True)

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


    def submit_guess(self, event=None):
        guess = self.guess_entry.get()
        if not guess:
            return

        # check kung tama o mali ang hula
        result = self.controller.game.check_guess(guess)
        self.guess_entry.delete(0, tk.END)

        if result == "correct":
            self.controller.stop_timer()
            messagebox.showinfo("Correct!", f"You got it! It was {self.controller.game.current_pokemon['name']}!")
            self.load_next_pokemon()
        elif result == "wrong":
            messagebox.showwarning("Incorrect!", "That's not it! -1 life")
            if self.controller.game.lives == 0:
                self.controller.end_game()
            else:
                self.load_next_pokemon()


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


# === SCREEN 3: GAME OVER SCREEN ===
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


# === START APP ===
if __name__ == "__main__":
    app = PokemonApp()
    app.mainloop()
