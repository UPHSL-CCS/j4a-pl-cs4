import requests
import random
from PIL import Image, ImageEnhance
from io import BytesIO

BASE_URL = "https://pokeapi.co/api/v2/pokemon/"
MAX_POKEMON_ID = 151

def _create_silhouette(image_data):
    try:
        image = Image.open(BytesIO(image_data))
        if image.mode != 'RGBA':
            image = image.convert('RGBA')
        enhancer = ImageEnhance.Brightness(image)
        darkened = enhancer.enhance(0.09)
        return darkened
    except Exception as e:
        print(f"Error making silhouette: {e}")
        return None

def get_random_pokemon():
    try:
        pokemon_id = random.randint(1, MAX_POKEMON_ID)
        url = f"{BASE_URL}{pokemon_id}"
        response = requests.get(url)
        response.raise_for_status()
        data = response.json()

        name = data["name"].capitalize()
        types = [t["type"]["name"] for t in data["types"]]
        sprite_url = data["sprites"]["front_default"]

        image_data = requests.get(sprite_url).content
        silhouette = _create_silhouette(image_data)
        hint = f"It has {len(types)} type(s). One of them is '{types[0]}'."

        return {"name": name, "hint": hint, "silhouette_image": silhouette}
    except Exception as e:
        print(f"API error: {e}")
        return None
