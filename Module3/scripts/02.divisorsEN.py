def get_divisors(n):
    if not isinstance(n, int) or n <= 0:
        raise ValueError("Input must be a positive integer")

    divisors = []
    for i in range(1, int(n**0.5) + 1):
        if n % i == 0:
            divisors.append(i)
            if i != n // i:
                divisors.append(n // i)
    return sorted(divisors)

# Example usage:
number = int(input("Enter a positive integer: "))
print("Divisors:", get_divisors(number))
