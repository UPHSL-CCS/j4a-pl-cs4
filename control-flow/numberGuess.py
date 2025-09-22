import random

uwu = random.randint(1,10)
guess = 0
count = 0

print("⋆.𐙚 ̊ Lets play a game! 𐔌՞. .՞𐦯\n\nGuess the number I am thinking between 1 to 10! (⸝⸝> ᴗ•⸝⸝)")

while guess!= uwu:
    try:
        guess = int(input("My guess is number "))
        count += 1

        if guess < uwu:
            print("Oops, too low! (⸝⸝๑﹏๑⸝⸝)")
        elif guess > uwu:
            print("Woah, too high! ( ◡̀_◡́)ᕤ")
        elif guess == uwu:
            print(f"Congratulations! You guessed it right! ♡〜٩( ˃▿˂ )۶〜♡\nIt took you {count} time/s!")
        elif guess > 10 | guess <= 0:
            print("Please enter 1 to 10 ONLY!! ( ｡ •̀ ᴖ •́ ｡)")

    except ValueError:
        print("Please enter digits 1 to 10 ONLY!! ( ｡ •̀ ᴖ •́ ｡)")