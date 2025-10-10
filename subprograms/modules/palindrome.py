def isPalindrome(text):
    """Palindrome is a word, phrase, or sequence that reads the same backward as forward."""
    
    cleaned_text = "".join(char for char in text if char.isalpha()).lower()
    # this part cleans the text in a way that it removes all characters that are not an alphabet and turns it to lowercase.

    return cleaned_text == cleaned_text[::-1]
    # this compares the cleaned original text to its reversed version.