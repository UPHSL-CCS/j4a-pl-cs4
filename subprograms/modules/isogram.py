def isIsogram(text):
    """Isogram is a word, phrase, or sentence where each letter of the alphabet appears only once, or where letters repeat an equal number of times."""
    
    cleaned_text = "".join(char for char in text if char.isalpha()).lower()
    # this part cleans the text in a way that it removes all characters that are not an alphabet and turns it to lowercase.

    return len(cleaned_text) == len(set(cleaned_text))
    # this checks if the original length of text is similar to the length/count of the unique characters.