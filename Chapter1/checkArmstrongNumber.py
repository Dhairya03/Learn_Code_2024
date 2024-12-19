def calculate_sum_of_digits_raised_to_number_of_digits(input_number):
    #Initializing Sum and Number of Digits
    sum_of_digits=0
    total_number_of_digits=0

    #Calculating Number of individual digits
    temporary_number = input_number
    while temporary_number > 0:
        total_number_of_digits = total_number_of_digits + 1
        temporary_number = temporary_number // 10

    #Finding Armstrong Number
    temporary_number = input_number
    for loop_counter in range(1, temporary_number + 1):
        remainder = temporary_number % 10
        sum_of_digits = sum_of_digits + (remainder ** total_number_of_digits)
        temporary_number //= 10
    return sum_of_digits

#End of Function

#User Input
user_input_number = int(input("\nPlease Enter the Number to Check for Armstrong: "))

if (user_input_number == calculate_sum_of_digits_raised_to_number_of_digits(user_input_number)):
    print("\n %d is Armstrong Number.\n" % user_input_number)
else:
    print("\n %d is Not a Armstrong Number.\n" % user_input_number)