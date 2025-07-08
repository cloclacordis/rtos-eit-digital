# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Timofei Alekseenko

import math

def lcm(a, b):
    """Compute the least common multiple (LCM) of two numbers."""
    return a * b // math.gcd(a, b)

def compute_hyperperiod(periods):
    """Compute the hyperperiod (LCM) of a list of periods."""
    H = periods[0]
    for p in periods[1:]:
        H = lcm(H, p)
    return H

def get_divisors(n):
    """Return a sorted list of all divisors of integer n."""
    divisors = []
    for i in range(1, int(math.sqrt(n)) + 1):
        if n % i == 0:
            divisors.append(i)
            if i != n // i:
                divisors.append(n // i)
    return sorted(divisors)

def find_max_frame(tasks):
    """
    Find the largest feasible frame size for a set of tasks.
    Tasks is a list of dictionaries {'P': period, 'e': execution time, 'D': deadline}.
    """
    periods = [t['P'] for t in tasks]
    exec_times = [t['e'] for t in tasks]
    deadlines = [t['D'] for t in tasks]

    # Calculation header
    print("\n=== Finding the Largest Frame Size ===\n")

    # Step 1: compute hyperperiod and minimal feasible frame size
    H = compute_hyperperiod(periods)
    e_max = max(exec_times)
    print(f"Hyperperiod H = {H}")
    print(f"Minimum feasible frame size f ≥ max(e_i) = {e_max}\n")

    # Step 2: compute divisors of hyperperiod, filter those f ≥ max(e_i), print descending order
    divisors = get_divisors(H)
    candidates = [f for f in divisors if f >= e_max]
    candidates.sort(reverse=True)
    print("Divisors of hyperperiod satisfying f ≥ max(e_i), in descending order:", candidates, "\n")

    # Step 3: check the schedulability condition (2f – gcd ≤ D)
    for f in candidates:
        print(f"### Checking f = {f} ###")
        all_pass = True
        for idx, (P, e, D) in enumerate(zip(periods, exec_times, deadlines), start=1):
            gcd_val = math.gcd(P, f)
            lhs = 2 * f - gcd_val
            if lhs <= D:
                print(f"T{idx}: 2 * {f} - gcd({P},{f}) = {lhs} ≤ D{idx}({D}) ✓")
            else:
                print(f"T{idx}: 2 * {f} - gcd({P},{f}) = {lhs} > D{idx}({D}) ✗")
                all_pass = False
                break
        if all_pass:
            print(f"\n✓ Largest feasible frame size guaranteeing timely completion is f = {f}\n")
            return
        print()  # separator

    print("✗ No largest feasible frame size guaranteeing timely completion exists\n")

def main():
    print("=== Find the Largest Frame Size for a Cyclic Scheduler ===")
    try:
        n = int(input("Enter the number of tasks in the set: "))
        if n <= 0:
            raise ValueError
    except ValueError:
        print("Error: please enter a positive integer.")
        return

    tasks = []
    for i in range(1, n+1):
        try:
            P = int(input(f"For task T{i}, enter period P{i}: "))
            if P <= 0:
                raise ValueError
        except ValueError:
            print("Error: period must be a positive integer.")
            return

        try:
            e = float(input(f"For task T{i}, enter execution time e{i}: "))
            if e <= 0:
                raise ValueError
        except ValueError:
            print("Error: execution time must be a positive number.")
            return

        D_input = input(f"For task T{i}, enter deadline D{i} or press Enter if D{i} = P{i}: ")
        if D_input.strip() == "":
            D = P
        else:
            try:
                D = float(D_input)
                if D <= 0:
                    raise ValueError
            except ValueError:
                print("Error: deadline must be a positive number.")
                return

        tasks.append({'P': P, 'e': e, 'D': D})

    find_max_frame(tasks)

if __name__ == "__main__":
    main()

