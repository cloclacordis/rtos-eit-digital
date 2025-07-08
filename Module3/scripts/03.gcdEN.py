def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# User input
try:
    a = int(input("Enter the first number: "))
    b = int(input("Enter the second number: "))
    result = gcd(a, b)
    print(f"GCD({a},{b}) = {result}")
except ValueError:
    print("Error: please enter valid integers.")
