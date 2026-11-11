import random
import time
from sympy import isprime, nextprime, gcd
from math import gcd as math_gcd

def generate_prime(bits):
    p = random.getrandbits(bits)
    while not isprime(p):
        p = nextprime(p)
    return p

def generate_keypair(bits):
    p = generate_prime(bits)
    q = generate_prime(bits)
    while p == q:
        q = generate_prime(bits)
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 65537  
    while math_gcd(e, phi) != 1:
        e = nextprime(e)
    d = pow(e, -1, phi)
    return ((e, n), (d, n))

def encrypt(msg, pubkey):
    e, n = pubkey
    return pow(msg, e, n)
def decrypt(cipher, privkey):
    d, n = privkey
    return pow(cipher, d, n)

def factor_rsa(n):
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return i, n // i
    return None, None

def crack_rsa(n, e):
    p, q = factor_rsa(n)
    if p is None:
        return None
    phi = (p - 1) * (q - 1)
    d = pow(e, -1, phi)
    return d

def test_crack(bits):
    pub, priv = generate_keypair(bits)
    e, n = pub
    d_real, _ = priv

    start = time.time()
    d_cracked = crack_rsa(n, e)
    end = time.time()

    cracked = d_cracked == d_real
    duration = end - start

    print(f"密钥位数: {bits*2:<4} | n 位数: {n.bit_length():<5} | 破解 {'成功' if cracked else '失败'} | 用时: {duration:.4f} 秒")
    return duration
key_sizes = []
for i in range(4, 128):
    key_sizes.append(i)
times = []
for bits in key_sizes:
    t = test_crack(bits)
    times.append(t)
