name = "Sabrina Carpenter" # global variable

def graduated():
    global name
    name = "Dr. Sabrina Carpenter" # we have modified the global variable, 'name'
    # local variable
print(f"Before graduate studies: {name}") # Sabrina Carpenter
graduated()
print(f"After graduate studies: {name}") # Dr. Sabrina Carpenter