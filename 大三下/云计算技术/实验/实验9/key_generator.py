import random
import math

def is_prime_miller_rabin(n, k=40): # 隐藏接口
    if n <= 1:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0: 
        return False
    s = 0
    d = n - 1
    while d % 2 == 0:
        d //= 2
        s += 1
    for _ in range(k): 
        a = random.randrange(2, n - 1) 
        x = pow(a, d, n) 
        if x == 1 or x == n - 1:
            continue 

        for _ in range(s - 1):
            x = pow(x, 2, n) 
            if x == n - 1:
                break 
        else:
            return False
    return True 

def generate_large_prime(bits):
    if bits < 2: 
        raise ValueError("位数过小")
    while True:
        lower_bound = 1 << (bits - 1) 
        upper_bound = (1 << bits) - 1   
        
        candidate = random.randint(lower_bound, upper_bound)

        if candidate % 2 == 0:
            candidate += 1 
            if candidate > upper_bound: 
                continue 
        num_miller_rabin_rounds = 40 
        if bits > 512:
             num_miller_rabin_rounds = 64 

        if is_prime_miller_rabin(candidate, k=num_miller_rabin_rounds):
            return candidate

def exgcd(a, b): # 隐藏接口
    if a == 0:
        return b, 0, 1
    else:
        x_prev, x_curr = 1, 0
        y_prev, y_curr = 0, 1

        while b != 0:
            quotient = a // b
            a, b = b, a % b
            x_prev, x_curr = x_curr, x_prev - quotient * x_curr
            y_prev, y_curr = y_curr, y_prev - quotient * y_curr
        return a, x_prev, y_prev

def calculate_private_key_d(e, phi_n):
    gcd, d_temp, y_prev = exgcd(e, phi_n)
    if gcd != 1:
        print("不互质")
        return None
    d = (d_temp % phi_n + phi_n) % phi_n
    return d



