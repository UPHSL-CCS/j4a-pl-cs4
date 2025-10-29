class PokemonGame:
    def __init__(self):
        self.lives = 3
        self.current_pokemon= None
        self.caught_pokemon = []
        self.got_away= []
        self.game_active= False

    def start_game(self, difficulty):
        self.lives = 3
        self.caught_pokemon = []
        self.got_away = []
        self.game_active = True

    
    def load_new_pokemon_data(self,data):
        self.current_pokemon = data #Loads new pokemon into the round

    def check_guess(self, guess):
        if not self.current_pokemon:
            return None
        
        if guess.strip().lower()==self.current_pokemon["name"].lower():
            self.caught_pokemon.append(self.current_pokemon["name"])
            return "correct"
        else:
            self.lives -= 1
            self.got_away.append(self.current_pokemon["name"])
            return "wrong"
        
    def handle_timeout(self):  #Handles when the player runs out of time.
        if self.current_pokemon and self.current_pokemon["name"] not in self.got_away:
            self.got_away.append(self.current_pokemon["name"])
        self.lives -= 1


    def get_score(self): #Final score calculation
        return {
            "score": len(self.caught_pokemon),
            "caught": self.caught_pokemon,
            "got_away": self.got_away
        }
