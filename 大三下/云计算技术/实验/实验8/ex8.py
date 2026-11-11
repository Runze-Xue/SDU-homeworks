from math import gcd
from sympy import randprime, mod_inverse
import time

def generate_rsa_keys(bit_length):
    # 生成两个大素数
    p = randprime(2**(bit_length//2), 2**(bit_length//2 + 1))
    q = randprime(2**(bit_length//2), 2**(bit_length//2 + 1))
    n = p * q
    phi = (p-1) * (q-1)
    
    e = 65537
    while gcd(e, phi) != 1:
        e += 2
    
    # 计算私钥d
    d = mod_inverse(e, phi)
    return (e, n), (d, n)

def encrypt(plaintext, public_key):
    e, n = public_key
    return pow(plaintext, e, n)

def decrypt(ciphertext, private_key):
    d, n = private_key
    return pow(ciphertext, d, n)

from sympy import factorint

def factorize_n(n):
    start_time = time.time()
    factors = factorint(n)
    end_time = time.time()
    return end_time - start_time

# 测试不同密钥长度的分解时间
key_lengths = [64, 128, 256, 512, 1024]
time_results = {}

for length in key_lengths:
    # 生成密钥对
    public_key, private_key = generate_rsa_keys(length)
    n = public_key[1]
    # 记录分解时间
    time_taken = factorize_n(n)
    time_results[length] = time_taken
    print(f"密钥长度 {length} bits: 分解时间 {time_taken:.4f} 秒")