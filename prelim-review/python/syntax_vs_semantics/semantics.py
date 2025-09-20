def average_calculator():
    print("Let's calculate your total average score in English, Math, and Science! (,,>ヮ<,,)")
    english = int(input("English: "))
    math = int(input("Math: "))
    science = int(input("Science: "))

    total = english + math + science / 3
    # it should be (english + math + science) / 3
    # the logic is not correct
    
    print(f"\n────୨ৎ────\nYour average is {total}! Goodjob ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧")

average_calculator()