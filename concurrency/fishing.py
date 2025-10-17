import threading # run multiple tasks
import time # for counting seconds
import random # random fish type

caught = False # default na false kasi wala pang nahuhuli
time_up = False

def timer(seconds):
    global caught, time_up
    for i in range(seconds, 0, -1):
        if caught:
            return  # stop counting if fish is caught
        print(f"{i}... ", end="", flush=True)
        time.sleep(1)
    if not caught:
        time_up = True  # tell main thread that time is up
        print("\nThe fish got away... (｡•́︿•̀｡)")

def fishing_game():
    global caught, time_up

    print("⋆｡ﾟ☁︎｡⋆｡ 𓇼 Let's go fishing! 𓆝⋆｡ﾟ☁︎｡⋆｡\n")
    level = input("Choose difficulty — Easy (E), Medium (M), Hard (H): ").lower()
    # set timer based on difficulty

    if level == "e":
        seconds = 5
    elif level == "m":
        seconds = 3
    elif level == "h":
        seconds = 1
    else:
        print("Invalid choice! Defaulting to Easy. ( ˶ˊᵕˋ˵ )")
        seconds = 5

    print(f"\nWaiting for a fish... You have {seconds} seconds to catch it! 🎣")

    # start the timer thread
    t = threading.Thread(target=timer, args=(seconds,))
    t.start()

    # while timer is running, keep waiting for Enter
    while not time_up and not caught:
        user_input = input("Press ENTER to catch the fish!! (≧▽≦)\n")
        if not time_up:
            caught = True  # stop the timer
            fish_types = ["Salmon", "Tuna", "Goldfish", "Shark", "Clownfish", "Koi", "Betta", "Catfish"]
            fish = random.choice(fish_types) 
            print(f"\nYou caught a {fish}! ₍˶ˆ꒳ˆ˵₎♡")
        else:
            print("\nToo late... the fish swam away! (╥﹏╥)")
        break

fishing_game()
