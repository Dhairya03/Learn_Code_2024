import random

main()

def main():
    correct_number = random.randint(1,100)
    is_correct_number_guessed = False
    number_of_guesses = 0
    while not is_correct_number_guessed:
        user_guessed_number = get_user_guess()
        number_of_guesses += 1
        is_correct_number_guessed = check_user_guess(user_guessed_number, correct_number)
    print("You guessed it in ", number_of_guesses, " guesses!")

def get_user_guess():
    user_input_number = input("Guess a number between 1 and 100:")
    while not is_valid_guess(user_input_number):
        user_input_number = input("I wont count this one Please enter a number between 1 to 100")
    return int(user_input_number)

def is_valid_guess(guessed_number):
    return guessed_number.isdigit() and 1<=int(guessed_number)<=100

def check_user_guess(user_input_number, correct_number):
    is_correct_user_guess = false
    if user_input_number < correct_number:
        user_input_number = input("Too low. Guess again")
    elif user_input_number > correct_number:
        user_input_number = input("Too High. Guess again")
    else:
        is_correct_user_guess = true
    return is_correct_user_guess
        