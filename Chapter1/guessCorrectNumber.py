import random
def isNumberInRange(number):
    if number.isdigit() and 1 <= int(number) <= 100:
        return True
    else:
        return False

def main():
    correct_number = random.randint(1,100)
    is_guessed = False
    input_number = input("Guess a number between 1 and 100:")
    number_of_guesses = 0
    while not is_guessed:
        if not isNumberInRange(input_number):
            input_number = input("I wont count this one Please enter a number between 1 to 100")
            continue
        else:
            number_of_guesses += 1
            input_number = int(input_number)

        if input_number < correct_number:
            input_number = input("Too low. Guess again")
        elif input_number > correct_number:
            input_number = input("Too High. Guess again")
        else:
            print("You guessed it in",number_of_guesses,"guesses!")
            is_guessed = True

main()
        