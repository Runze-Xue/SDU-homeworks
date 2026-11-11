from math import gcd
from collections import defaultdict, Counter
import re

def kasiski_examination(ciphertext: str, min_sub_len=3) -> list:
    ciphertext = ciphertext.upper().replace(" ", "")
    seq_positions = defaultdict(list)
    for i in range(len(ciphertext) - min_sub_len + 1):
        seq = ciphertext[i:i+min_sub_len]
        seq_positions[seq].append(i)
    
    repeated_seq = {seq: pos for seq, pos in seq_positions.items() if len(pos) > 1}
    distances = []
    for seq, positions in repeated_seq.items():
        for i in range(len(positions)-1):
            distances.append(positions[i+1] - positions[i])
    
    possible_key_lengths = Counter()
    for d in distances:
        for factor in range(2, d+1):
            if d % factor == 0:
                possible_key_lengths[factor] += 1
                
    return possible_key_lengths.most_common()

def friedman_test(ciphertext: str) -> float:
    ciphertext = re.sub("[^A-Z]", "", ciphertext.upper())
    N = len(ciphertext)
    if N <= 1:
        return 0
    frequency = Counter(ciphertext)
    ic = sum(v * (v-1) for v in frequency.values()) / (N*(N-1))
    estimated_key_length = (0.027 * N) / ((N - 1) * ic - 0.038 * N + 0.065) if ic != 0 else 0
    return estimated_key_length


content1 = "i like sleeping, and many pepole call me the king of sleep."
content = content1 * 20
key = "cloudcomputing"

# 加密过程
encryption = ""
index = 0
for e in content:
    if ord(e) - ord('a') < 0 or ord(e) - ord('a') > 25:
        encryption += e
        continue
    encryption += chr((ord(e) - ord('a') + ord(key[index]) - ord('a')) % 26 + ord('a'))
    index = (index + 1) % len(key)

# 解密过程
decrypt = ""
index = 0
for e in encryption:
    if ord(e) - ord('a') < 0 or ord(e) - ord('a') > 25:
        decrypt += e
        continue
    decrypt += chr((26 + ord(e) - ord(key[index])) % 26 + ord('a'))
    index = (index + 1) % len(key)

# 输出结果
print("原始文本 ", content )
print("密钥 ", key)
print("加密结果 ", encryption)
print("解密结果 ", decrypt)
print("Kasiski 检测 ", kasiski_examination(encryption))
print("Friedman 检测 ", friedman_test(encryption))