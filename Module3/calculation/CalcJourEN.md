## Task Overview

Two tasks are given — one **computational**, for finding a feasible schedule for a cyclic scheduler, and the other — a **simulation** using a **real-time simulator** to verify feasibility. Below are the formulations and requirements for both tasks, conditions and necessary information for completion, along with a detailed solution log.

---

### First Task

#### Given

For the cyclic scheduler, **three task sets** are provided:

**First Task Set**

| **Task**         | **P** | **e** | **D** |
| ---------------- | ----- | ----- | ----- |
| T<sub>1</sub>    | 15    | 1     | 14    |
| T<sub>2</sub>    | 20    | 2     | 26    |
| T<sub>3</sub>    | 22    | 3     | 22    |

**Second Task Set**

| **Task**         | **P** | **e** | **D** |
| ---------------- | ----- | ----- | ----- |
| T<sub>1</sub>    | 4     | 1     | 4     |
| T<sub>2</sub>    | 5     | 2     | 7     |
| T<sub>3</sub>    | 20    | 5     | 20    |

**Third Task Set**

| **Task**         | **P** | **e** | **D** |
| ---------------- | ----- | ----- | ----- |
| T<sub>1</sub>    | 5     | 0.1   | 5     |
| T<sub>2</sub>    | 7     | 1     | 7     |
| T<sub>3</sub>    | 12    | 6     | 12    |
| T<sub>4</sub>    | 45    | 9     | 45    |

> Task structure: **T<sub>n</sub>(P,e,D)**, where **P** — *period*, or time between two consecutive task activations; **e** — *execution time*, or duration required for normal task completion; **D** — *deadline*, or maximum time by which the task must be completed.
> If the deadline **D** is not explicitly specified — as in **T<sub>3</sub>(22,3)**, then **D = P**, hence **T<sub>3</sub>(22,3,22)**.

---

#### To Find

For each of the three task sets, find:

**1**. *The largest frame size* that guarantees timely completion of all tasks (*largest feasible frame size*).

**2**. Such a size that satisfies the following *three requirements*:

**a**. *Task duration*. Each task **i** must start and complete within a single frame **f**.  
Hence, **f ≥ max<sub>1 ≤ i ≤ n</sub>(e<sub>i</sub>)**, where **e<sub>i</sub>** is the execution time of task **i**.
No frame can be smaller than the longest task.

**b**. *Hyperperiod divisibility*. The frame size must evenly divide the hyperperiod **H mod f = 0** (or **H % f == 0**).

> The **hyperperiod** is the minimum interval that encompasses all task periods. Mathematically, the hyperperiod is the **least common multiple (LCM)** of all periods. Practically, the hyperperiod is used to verify feasibility: if the schedule meets deadlines within one hyperperiod, it will always meet them.  
> To determine the *number of frames* **F** in the hyperperiod **H**, simple division is used: **F = H / f** (not to be confused with *divisibility check* **H mod f = 0**).

**c**. *Interval between release and deadline*. There must be at least one frame that fits between the task's *release time* and its *deadline*.  
In other words: **2f − GCD(P<sub>i</sub>, f) ≤ D<sub>i</sub>**, where **f** is the frame size; **P<sub>i</sub>** is the period of task **i**; **D<sub>i</sub>** is the deadline of task **i**; **GCD(P<sub>i</sub>, f)** is the *greatest common divisor (GCD)* of the period and frame values.

---

#### Conditions

The solution is performed using a sheet of paper and a pen. **Draft solution**:

![Freehand Drawing.svg](assets/01.mydraft.jpg)

> The logic of the solution will be described in detail below and an example of automation will be offered.

---

#### Solution

**Search Logic**

1. Find the hyperperiod **H = LCM(P<sub>1</sub>,P<sub>2</sub>,...,P<sub>n</sub>)**.  
2. Select candidate frames **f** (divisors of **H**) that:  
- **≥ max<sub>1 ≤ i ≤ n</sub>(e<sub>i</sub>)** — requirement 1,  
- **H mod f = 0** — requirement 2.  
3. Check selected **f** against the condition **2f − GCD(P<sub>i</sub>, f) ≤ D<sub>i</sub>** for all tasks — requirement 3.  
4. From the passing **f**, select the maximum value for each set.

---

**Step 1**

First, find the **LCM** of periods **P** for each set.

- First set: LCM(15,20,22) = **660**.  
- Second set: LCM(4,5,20) = **20**.  
- Third set: LCM(5,7,12,45) = **1260**.

> To find the LCM of several numbers manually: factor each number into its prime factors, take the highest power of each unique prime, multiply them together — that’s the LCM. For example, LCM(12,18): 12 = 2<sup>2</sup> * 3, 18 = 2 * 3<sup>2</sup> -> LCM = 2<sup>2</sup> * 3<sup>2</sup> = 36.

**Example of LCM search automation for positive integers**

```python
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
```

Run, input, output:

![Freehand Drawing.svg](assets/02.lcmEN.png)

---

**Step 2.1**

For each set, find the minimum frame size requirement **f**, i.e., the task with the longest execution time **e**.

- First set: task T<sub>3</sub>(22,3), hence **f ≥ 3**.  
- Second set: task T<sub>3</sub>(20,5), hence **f ≥ 5**.  
- Third set: task T<sub>4</sub>(45,9), hence **f ≥ 9**.

---

**Step 2.2**

Per the second requirement, the frame size must evenly divide the hyperperiod **H**.

**Manual divisor search example for 660**  
Factorize 660:

660 / 2 = 330,  
330 / 2 = 165, no longer divisible by 2, write 2<sup>2</sup>.  
165 / 3 = 55, no longer divisible by 3, write 3.  
55 / 5 = 11, write 5.  
Remainder is 11 — a prime number, write 11.  
Result: **660 = 2<sup>2</sup> * 3 * 5 * 11**.  

Each divisor of 660 is a number that can be "assembled" by taking **powers** from the factorization:

- 0 to 2 for 2,  
- 0 to 1 for 3,  
- 0 to 1 for 5,  
- 0 to 1 for 11.

Possible combinations:

**d = 2<sup>a</sup> * 3<sup>b</sup> * 5<sup>c</sup> * 11<sup>d</sup>, a ∈ {0,1,2},  b, c, d ∈ {0,1}**.

Number of (positive) divisors: 3 * 2 * 2 * 2 = **24**. To find **all divisors**, enumerate all combinations of the above powers and compute the value for each.

**Automated divisor search example for a positive integer**

```python
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
```

Run, input, output:

![Freehand Drawing.svg](assets/03.divisorsEN.png)

---

Thus, **all** hyperperiod **H** divisors for each task set:

- First set, **H = 660**: 1, 2, 3, 4, 5, 6, 10, 11, 12, 15, 20, 22, 30, 33, 44, 55, 60, 66, 110, 132, 165, 220, 330, 660.  
- Second set, **H = 20**: 1, 2, 4, 5, 10, 20.  
- Third set, **H = 1260**: 1, 2, 3, 4, 5, 6, 7, 9, 10, 12, 14, 15, 18, 20, 21, 28, 30, 35, 36, 42, 45, 60, 63, 70, 84, 90, 105, 126, 140, 180, 210, 252, 315, 420, 630, 1260.

Apply the minimum frame size requirement **f** and get:

- First set, **f ≥ 3**, **f = {3,4,5,6,10,11,12,15,20,22,30,33,44,55,60,66,110,132,165,220,330,660}**.  
- Second set, **f ≥ 5**, **f = {5,10,20}**.  
- Third set, **f ≥ 9**, **f = {9,10,12,14,15,18,20,21,28,30,35,36,42,45,60,63,70,84,90,105,126,140,180,210,252,315,420,630,1260}**.

Recall maximum period values **P** to limit candidate search "from above":

- First set **P<sub>max</sub> = 22**.  
- Second set, **P<sub>max</sub> = 20**.  
- Third set, **P<sub>max</sub> = 45**.

Sort frames in descending order to get the **candidate set** for requirement 3 verification:

- First set: **f<sub>rev</sub> = {22,20,15,12,11,10,6,5,4,3}**.  
- Second set: **f<sub>rev</sub> = {20,10,5}**.  
- Third set: **f<sub>rev</sub> = {45,42,36,35,30,28,21,20,18,15,14,12,10,9}**.

---

**Step 3**

Verify requirement 3 for the obtained sets: **2f − GCD(P<sub>i</sub>, f) ≤ D<sub>i</sub>**, where **f** is the frame size; **P<sub>i</sub>** is the period of task **i**; **D<sub>i</sub>** is the deadline of task **i**; **GCD(P<sub>i</sub>, f)** is the greatest common divisor of the period and frame.

Briefly, the **greatest common divisor (GCD)** of two or more numbers is the largest number that divides all given numbers without remainder.

The simplest algorithm for computing GCD is the **Euclidean algorithm** for two numbers `a` and `b`:

1. While `b ≠ 0`, execute:

```python
temp = b
b = a % b
a = temp
```

2. When `b = 0`, the value `a` is the GCD.
    
Or:

```python
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# User input
try:
    a = int(input("Enter first number: "))
    b = int(input("Enter second number: "))
    result = gcd(a, b)
    print(f"GCD({a},{b}) = {result}")
except ValueError:
    print("Error: enter integers.")
```

Run, input, output:

![Freehand Drawing.svg](assets/04.gcdEN.png)

---

Begin **requirement 3 verification**.

- First set:  
   * **T<sub>1</sub>(15,1,14)**:  
     2 * 22 − GCD(15,22) = 44 − 1  = 43 > D<sub>1</sub>(14). Candidate **f = 22 fails**.  
     2 * 20 − GCD(15,20) = 40 − 5  = 35 > D<sub>1</sub>(14). Candidate **f = 20 fails**.  
     2 * 15 − GCD(15,15) = 30 − 15 = 15 > D<sub>1</sub>(14). Candidate **f = 15 fails**.  
     2 * 12 − GCD(15,12) = 24 − 3  = 21 > D<sub>1</sub>(14). Candidate **f = 12 fails**.  
     2 * 11 − GCD(15,11) = 22 − 1  = 21 > D<sub>1</sub>(14). Candidate **f = 11 fails**.  
     2 * 10 − GCD(15,10) = 20 − 5  = 15 > D<sub>1</sub>(14). Candidate **f = 10 fails**.  
     2 * 6  − GCD(15,6)  = 12 − 3  = 9  < D<sub>1</sub>(14). Candidate **f = 6  passes**.
     
   * **T<sub>2</sub>(20,2,26)**:  
     2 * 6 − GCD(20,6) = 12 − 2 = 10 < D<sub>2</sub>(26). Candidate **f = 6 passes**.
     
   * **T<sub>3</sub>(22,3)**:  
     2 * 6 − GCD(22,6) = 12 − 2 = 10 < D<sub>3</sub>(22). Candidate **f = 6 passes**.

> Candidate **f = 6** passes all requirement 3 checks for the first set and does not violate other requirements.

- Second set:  
   * **T<sub>1</sub>(4,1)**:  
     2 * 20 − GCD(4,20) = 40 − 4 = 36 > D<sub>1</sub>(4). Candidate **f = 20 fails**.  
     2 * 10 − GCD(4,10) = 20 − 2 = 18 > D<sub>1</sub>(4). Candidate **f = 10 fails**.  
     2 * 5  − GCD(4,5)  = 10 − 1 = 9  > D<sub>1</sub>(4). Candidate **f = 5  fails**.  

> No candidate passes the first task check (*no feasible frame size exists*), checking **T<sub>2</sub>**, **T<sub>3</sub>** is unnecessary. **Cyclic scheduling is infeasible for this task set**, other methods should be considered (e.g., dynamic scheduling).

- Third set:
   * **T<sub>1</sub>(5,0.1)**:  
     2 * 45 − GCD(5,45) = 90 − 5 = 85 > D<sub>1</sub>(5). Candidate **f = 45 fails**.  
     2 * 42 − GCD(5,42) = 84 − 1 = 83 > D<sub>1</sub>(5). Candidate **f = 42 fails**.  
     2 * 36 − GCD(5,36) = 72 − 1 = 71 > D<sub>1</sub>(5). Candidate **f = 36 fails**.  
     2 * 35 − GCD(5,35) = 70 − 5 = 65 > D<sub>1</sub>(5). Candidate **f = 35 fails**.  
     2 * 30 − GCD(5,30) = 60 − 5 = 55 > D<sub>1</sub>(5). Candidate **f = 30 fails**.  
     2 * 28 − GCD(5,28) = 56 − 1 = 55 > D<sub>1</sub>(5). Candidate **f = 28 fails**.  
     2 * 21 − GCD(5,21) = 42 − 1 = 41 > D<sub>1</sub>(5). Candidate **f = 21 fails**.  
     2 * 20 − GCD(5,20) = 40 − 5 = 35 > D<sub>1</sub>(5). Candidate **f = 20 fails**.  
     2 * 18 − GCD(5,18) = 36 − 1 = 35 > D<sub>1</sub>(5). Candidate **f = 18 fails**.  
     2 * 15 − GCD(5,15) = 30 − 5 = 25 > D<sub>1</sub>(5). Candidate **f = 15 fails**.  
     2 * 14 − GCD(5,14) = 28 − 1 = 27 > D<sub>1</sub>(5). Candidate **f = 14 fails**.  
     2 * 12 − GCD(5,12) = 24 − 1 = 23 > D<sub>1</sub>(5). Candidate **f = 12 fails**.  
     2 * 10 − GCD(5,10) = 20 − 5 = 15 > D<sub>1</sub>(5). Candidate **f = 10 fails**.  
     2 * 9  − GCD(5,9)  = 18 − 1 = 17 > D<sub>1</sub>(5). Candidate **f = 9  fails**.  

> No candidate passes the first task check (*no feasible frame size exists*), checking **T<sub>2</sub>**, **T<sub>3</sub>** is unnecessary. **Cyclic scheduling is infeasible for this task set**, other methods should be considered (e.g., dynamic scheduling).

---

**Step 4**

From the passing **f**, we would select the *largest frame size* that guarantees timely completion of all tasks (*largest feasible frame size*) for each set, but only the first set passed verification — and only with a single value **f = 6**.

---

#### Result

Thus, the **result** is:

- First set: **f = 6**.  
- Second set: **no feasible frame size exists**.  
- Third set: **no feasible frame size exists**.

---

#### Automation

The solution can be automated:

```python
import math

def lcm(a, b):
    """Compute the least common multiple (LCM) of two numbers a and b."""
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
        print("Error: please enter a positive integer for the number of tasks.")
        return

    tasks = []
    for i in range(1, n+1):
        try:
            P = int(input(f"For task T{i} enter period P{i}: "))
            if P <= 0:
                raise ValueError
        except ValueError:
            print("Error: period must be a positive integer.")
            return

        try:
            e = float(input(f"For task T{i} enter execution time e{i}: "))
            if e <= 0:
                raise ValueError
        except ValueError:
            print("Error: execution time must be a positive integer or floating point.")
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
                print("Error: deadline must be a positive integer or floating point.")
                return

        tasks.append({'P': P, 'e': e, 'D': D})

    find_max_frame(tasks)

if __name__ == "__main__":
    main()
```

Run, input, output:

![Freehand Drawing.svg](assets/05.cyclicSchedulerEN1.png)

![Freehand Drawing.svg](assets/05.cyclicSchedulerEN2.png)

![Freehand Drawing.svg](assets/05.cyclicSchedulerEN3.png)

--- 

### Second Task

