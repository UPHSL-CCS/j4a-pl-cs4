# Import necessary libraries
import requests  # Used to make HTTP requests (to get data from the API)
import random    # Used to pick a random Pokémon ID
from PIL import Image, ImageEnhance  # Used for opening and editing images (Pillow library)
from io import BytesIO  # Used to treat raw image data (in bytes) like a file

# The base URL for the PokéAPI
BASE_URL = "https://pokeapi.co/api/v2/pokemon/"
# We'll only pick Pokémon from the original 151 (Generation 1)
MAX_POKEMON_ID = 151
# --------------------------------------------------------------------------
# NOTE: The variables above are now at the module level for external access.
# --------------------------------------------------------------------------

def _create_silhouette(image_data):
    """Takes raw image data and turns it into a dark silhouette."""
    try:
        # Open the image data from memory
        image = Image.open(BytesIO(image_data))
        
        # Ensure the image has transparency (RGBA)
        # 'A' stands for Alpha (transparency), which helps make the background clear
        if image.mode != 'RGBA':
            image = image.convert('RGBA')
            
        # Select the brightness tool from the image enhancement library
        enhancer = ImageEnhance.Brightness(image)
        
        # Make the image very dark (0.09 is almost black)
        darkened = enhancer.enhance(0.09)
        
        return darkened
        
    except Exception as e:
        # If anything goes wrong during image processing, print an error
        print(f"Error making silhouette: {e}")
        return None

# --- Main Function ---

def get_random_pokemon():
    """Fetches a random Pokémon's data and creates a silhouette."""
    try:
        # 1. Get Pokémon Data from API
        
        # Pick a random number between 1 and 151
        pokemon_id = random.randint(1, MAX_POKEMON_ID)
        # Create the full API URL (e.g., ".../pokemon/25" for Pikachu)
        url = f"{BASE_URL}{pokemon_id}"
        
        # Make the API request
        response = requests.get(url)
        # Check if the request was successful (e.g., no 404 errors)
        response.raise_for_status() 
        
        # Parse the JSON response into a Python dictionary
        data = response.json()

        # 2. Extract Useful Information
        
        # Get the Pokémon's name and capitalize the first letter (e.g., "Pikachu")
        name = data["name"].capitalize()
        # Get the list of types (e.g., ["electric"])
        types = [t["type"]["name"] for t in data["types"]]
        # Get the URL for the Pokémon's front-facing picture (sprite)
        sprite_url = data["sprites"]["front_default"]

        # 3. Get and Process the Image
        
        # Download the image data from the sprite URL
        image_data = requests.get(sprite_url).content
        # Use our helper function to turn the image data into a silhouette
        silhouette = _create_silhouette(image_data)
        
        # 4. Create a Hint
        hint = f"It has {len(types)} type(s). One of them is '{types[0]}'."

        # 5. Return all the data
        # Send back a dictionary containing everything needed for the game
        return {"name": name, "hint": hint, "silhouette_image": silhouette}
        
    except Exception as e:
        # If the API request fails it will print an error
        print(f"API error: {e}")
        return None