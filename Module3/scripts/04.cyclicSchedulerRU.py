import math

def lcm(a, b):
    """Вычисляем НОК (LCM) двух чисел a и b."""
    return a * b // math.gcd(a, b)

def compute_hyperperiod(periods):
    """Вычисляем гиперпериод (LCM) списка периодов."""
    H = periods[0]
    for p in periods[1:]:
        H = lcm(H, p)
    return H

def get_divisors(n):
    """Возвращаем отсортированный список всех делителей числа n."""
    divisors = []
    for i in range(1, int(math.sqrt(n)) + 1):
        if n % i == 0:
            divisors.append(i)
            if i != n // i:
                divisors.append(n // i)
    return sorted(divisors)

def find_max_frame(tasks):
    """
    Ищем наибольший размер кадра для набора задач.
    Task - список словарей {'P': период, 'e': время выполнения, 'D': дедлайн}.
    """
    periods = [t['P'] for t in tasks]
    exec_times = [t['e'] for t in tasks]
    deadlines = [t['D'] for t in tasks]

    # Заголовок расчетов
    print("\n=== Поиск наибольшего размера кадра ===\n")

    # Шаг 1: гиперпериод и минимальный f
    H = compute_hyperperiod(periods)
    e_max = max(exec_times)
    print(f"Гиперпериод H = {H}")
    print(f"Минимально допустимый размер кадра f ≥ max(e_i) = {e_max}\n")

    # Шаг 2: кандидаты
    divisors = get_divisors(H)
    candidates = [f for f in divisors if f >= e_max]
    candidates.sort(reverse=True)
    print("Кандидаты (делители гиперпериода) после f ≥ max(e_i) в порядке убывания:", candidates, "\n")

    # Шаг 3: проверка третьего требования
    for f in candidates:
        print(f"### Проверка f = {f} ###")
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
            print(f"\n✓ Наибольший размер кадра, гарантирующий своевременное выполнение, f = {f}\n")
            return
        print()  # разделитель

    print("✗ Не существует наибольшего размера кадра, гарантирующего своевременное выполнение\n")

def main():
    print("=== Найти наибольший размер кадра для циклического планировщика ===")
    try:
        n = int(input("Введите число задач в наборе: "))
        if n <= 0:
            raise ValueError
    except ValueError:
        print("Ошибка: введите целое положительное число.")
        return

    tasks = []
    for i in range(1, n+1):
        try:
            P = int(input(f"Для задачи T{i} введите период P{i}: "))
            if P <= 0:
                raise ValueError
        except ValueError:
            print("Ошибка: период должен быть целым положительным числом.")
            return

        try:
            e = float(input(f"Для задачи T{i} введите время выполнения e{i}: "))
            if e <= 0:
                raise ValueError
        except ValueError:
            print("Ошибка: время выполнения должно быть положительным числом.")
            return

        D_input = input(f"Для задачи T{i} введите дедлайн D{i} или нажмите Enter, если D{i} = P{i}: ")
        if D_input.strip() == "":
            D = P
        else:
            try:
                D = float(D_input)
                if D <= 0:
                    raise ValueError
            except ValueError:
                print("Ошибка: дедлайн должен быть целым положительным числом.")
                return

        tasks.append({'P': P, 'e': e, 'D': D})

    find_max_frame(tasks)

if __name__ == "__main__":
    main()
