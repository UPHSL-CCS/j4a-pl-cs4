import requests
from modules.pokemon_api import BASE_URL, MAX_POKEMON_ID # Import constants
import threading
# NEW IMPORTS
from PIL import Image
from io import BytesIO

def fetch_catalogue_data(callback, error_callback):
    """
    Fetches the name, ID, types, and image data for all Pokémon (1-151) 
    in a separate thread to prevent the UI from freezing.
    
    Args:
        callback (function): Function to call when data is successfully loaded.
        error_callback (function): Function to call if an error occurs.
    """
    
    def _fetch_thread_target():
        catalogue_list = []
        try:
            for i in range(1, MAX_POKEMON_ID + 1):
                url = f"{BASE_URL}{i}"
                response = requests.get(url)
                response.raise_for_status()
                data = response.json()
                
                # --- NEW DATA EXTRACTION ---
                # Get the list of types (e.g., ["electric", "flying"])
                types = [t["type"]["name"].capitalize() for t in data["types"]]
                # Get the URL for the Pokémon's front-facing picture (sprite)
                sprite_url = data["sprites"]["front_default"]
                
                # --- IMAGE FETCHING & RESIZING (concurrently) ---
                # 1. Download the image data
                image_data = requests.get(sprite_url).content
                # 2. Open image from bytes
                sprite_image = Image.open(BytesIO(image_data))
                # 3. Resize to a small size for the catalogue (e.g., 50x50 pixels)
                resized_sprite = sprite_image.resize((50, 50), Image.Resampling.LANCZOS)
                
                # Extract essential data for the catalogue
                pokemon_entry = {
                    "id": i,
                    "name": data["name"].capitalize(),
                    "types": types, # NEW
                    "sprite_image": resized_sprite # NEW (PIL Image object)
                }
                catalogue_list.append(pokemon_entry)
                
        except Exception as e:
            # If any API call fails, call the error callback
            error_callback(f"Error fetching catalogue data: {e}")
            return

        # If successful, call the main callback with the fetched list
        callback(catalogue_list)

    # Start the fetching process in a dedicated thread
    threading.Thread(target=_fetch_thread_target, daemon=True).start()