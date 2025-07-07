**Set first**

| **Task** | **P** | **e** | **D** |
| -------- | ----- | ----- | ----- |
| T₁       | 15    | 1     | 14    |
| T₂       | 20    | 2     | 26    |
| T₃       | 22    | 3     | 22    |

**Set second**

| **Task** | **P** | **e** | **D** |
| -------- | ----- | ----- | ----- |
| T₁       | 4     | 1     | 4     |
| T₂       | 5     | 2     | 7     |
| T₃       | 20    | 5     | 20    |

**Set third**

| **Task** | **P** | **e** | **D** |
| -------- | ----- | ----- | ----- |
| T₁       | 5     | 0.1   | 5     |
| T₂       | 7     | 1     | 7     |
| T₃       | 12    | 6     | 12    |
| T₄       | 45    | 9     | 45    |

---

#### Automation

The solution can be automated:

```python
import math

def lcm(a, b):
    """Compute least common multiple (LCM) of two numbers a and b."""
    return a * b // math.gcd(a, b)

def compute_hyperperiod(periods):
    """Compute the hyperperiod (LCM) of a list of periods."""
    H = periods[0]
    for p in periods[1:]:
        H = lcm(H, p)
    return H

def get_divisors(n):
    """Return a sorted list of all positive divisors of integer n."""
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
    Tasks - list of dicts {'P': period, 'e': exec_time, 'D': deadline}.
    """
    periods = [t['P'] for t in tasks]
    exec_times = [t['e'] for t in tasks]
    deadlines = [t['D'] for t in tasks]

    # Calculation header
    print("\n=== Calculating Maximum Frame Size ===\n")

    # Step 1: hyperperiod and minimum frame
    H = compute_hyperperiod(periods)
    e_max = max(exec_times)
    print(f"Hyperperiod H = {H}")
    print(f"Minimum frame size f ≥ max(e_i) = {e_max}\n")

    # Step 2: candidates
    divisors = get_divisors(H)
    candidates = [f for f in divisors if f >= e_max]
    candidates.sort(reverse=True)
    print("Candidates (divisors of hyperperiod) after f ≥ max(e_i) in descending order:", candidates, "\n")

    # Step 3: check the third requirement
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
            print(f"\n✓ Largest feasible frame size f = {f}\n")
            return
        print()  # separator

    print("✗ No feasible frame size exists\n")

def main():
    print("=== Find Maximum Frame Size for Cyclic Scheduler ===")
    try:
        n = int(input("Enter the number of tasks in the set: "))
        if n <= 0:
            raise ValueError
    except ValueError:
        print("Error: please enter a unsigned integer for the number of tasks.")
        return

    tasks = []
    for i in range(1, n+1):
        try:
            P = int(input(f"For task T{i} enter period P{i}: "))
            if P <= 0:
                raise ValueError
        except ValueError:
            print("Error: period must be a unsigned integer.")
            return

        try:
            e = float(input(f"For task T{i} enter execution time e{i}: "))
            if e <= 0:
                raise ValueError
        except ValueError:
            print("Error: execution time must be a unsigned integer or floating point.")
            return

        D_input = input(f"For task T{i} enter deadline D{i} or press Enter if D{i} = P{i}: ")
        if D_input.strip() == "":
            D = P
        else:
            try:
                D = float(D_input)
                if D <= 0:
                    raise ValueError
            except ValueError:
                print("Error: deadline must be a unsigned integer or floating point.")
                return

        tasks.append({'P': P, 'e': e, 'D': D})

    find_max_frame(tasks)

if __name__ == "__main__":
    main()
```

Run, input, output:

![Freehand Drawing.svg](assets/06.cyclicSchedulerEN1.png)

![Freehand Drawing.svg](assets/07.cyclicSchedulerEN2.png)

![Freehand Drawing.svg](assets/08.cyclicSchedulerEN3.png)
