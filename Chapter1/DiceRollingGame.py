import random
def get_dice_outcome(sides_of_dice):
    dice_outcome=random.randint(1,sides_of_dice)
    return dice_outcome

def main():
    sides_of_dice=6
    is_playing=True
    while is_playing:
        user_input=input("Ready to roll? Enter Q to Quit")
        if user_input.lower() !="q":
            dice_outcome=get_dice_outcome(sides_of_dice)
            print("You have rolled a",dice_outcome)
        else:
            is_playing=False
