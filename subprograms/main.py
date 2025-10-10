from modules.palindrome import isPalindrome
from modules.isogram import isIsogram
from modules.tautonym import isTautonym


def wordPlay():
    while True:
        text = input("\nEnter a word to check (or press Enter to quit): ").strip()

        if text == "":
            print("\nજ⁀➴Thank you for playing with words! ₍₍⚞(˶˃ ꒳ ˂˶)⚟⁾⁾♡\n")
            break

        print("")  # spacing

        # Palindrome
        if isPalindrome(text):
            print(f"• '{text}' is a palindrome because it reads the same backward and forward.")
        else:
            print(f"• '{text}' is not a palindrome because it changes when reversed.")

        # Isogram
        if isIsogram(text):
            print(f"• '{text}' is an isogram because all its letters are unique.")
        else:
            print(f"• '{text}' is not an isogram because it contains repeated letters.")

print("———————————————————————————♡———————————————————————————\n꒰  ⊹  ˚ . [ Word Play Subprogram ] !  ⁺  𓈒 ꒱\n-this program checks if the word you entered is either an isogram, palindrome, or tautonym.\nPress Enter without typing anything to exit.\n———————————————————————————♡———————————————————————————")

wordPlay()