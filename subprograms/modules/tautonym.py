def isTautonym(text):
    """Tautonyms are words or names consisting of two identical parts."""
    
    cleaned_text = "".join(char for char in text if char.isalpha()).lower()
    # this part cleans the text in a way that it removes all characters that are not an alphabet and turns it to lowercase.
    length = len(cleaned_text)
    if length % 2 != 0:
        return False
    
    return cleaned_text[:length//2] == cleaned_text[length//2:]
    # compares the first half of the word to its other half

