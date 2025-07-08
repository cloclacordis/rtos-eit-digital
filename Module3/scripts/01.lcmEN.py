import math
from functools import reduce

# Function to compute LCM of two numbers
def lcm(a, b):
    return abs(a * b) // math.gcd(a, b)

# Function to compute LCM of a list of numbers
def lcm_multiple(numbers):
    return reduce(lcm, numbers)

# Function to get user input and validate it
def get_numbers():
    while True:
        user_input = input("Enter numbers separated by spaces to find LCM: ")
        parts = user_input.strip().split()
        try:
            numbers = [int(p) for p in parts]
            if not numbers:
                raise ValueError("The list is empty.")
            if any(n <= 0 for n in numbers):
                raise ValueError("All numbers must be positive integers.")
            return numbers
        except ValueError as e:
            print(f"Input error: {e}. Please try again.\n")

# Main function
def main():
    numbers = get_numbers()
    result = lcm_multiple(numbers)
    print(f"LCM of {numbers} = {result}")

if __name__ == "__main__":
    main()
