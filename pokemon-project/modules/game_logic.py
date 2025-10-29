class PokemonGame:
    def __init__(self):
        self.lives = 3
        self.timer_duration = 60
        self.current_pokemon = None
        self.caught_pokemon = []
        self.got_away = []
        self.game_active = False

    def start_game(self, difficulty):
        self.lives = 3
        self.caught_pokemon = []
        self.got_away = []
        self.game_active = True

        if difficulty == "Easy":
            self.timer_duration = 120
        elif difficulty == "Medium":
            self.timer_duration = 60
        else:
            self.timer_duration = 30

    def load_new_pokemon_data(self, data):
        """Loads a new Pokémon into the current round."""
        self.current_pokemon = data

    def check_guess(self, guess):
        """Checks the user's guess against the current Pokémon."""
        if not self.current_pokemon:
            return None

        if guess.strip().lower() == self.current_pokemon["name"].lower():
            self.caught_pokemon.append(self.current_pokemon["name"])
            return "correct"
        else:
            self.lives -= 1
            # Even if wrong, we record that the Pokémon 'got away'
            # to show it in the final results.
            self.got_away.append(self.current_pokemon["name"])
            return "wrong"

    def handle_timeout(self):
        """Handles when the player runs out of time."""
        if self.current_pokemon and self.current_pokemon["name"] not in self.got_away:
            self.got_away.append(self.current_pokemon["name"])
        self.lives -= 1

    def get_hint(self):
        """Returns a hint for the current Pokémon."""
        if self.current_pokemon:
            return self.current_pokemon["hint"]
        return "No hint available."

    def get_score(self):
        """Returns final score summary."""
        return {
            "score": len(self.caught_pokemon),
            "caught": self.caught_pokemon,
            "got_away": self.got_away
        }
