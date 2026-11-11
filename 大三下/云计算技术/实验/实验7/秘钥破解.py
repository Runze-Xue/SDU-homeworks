from math import gcd
from collections import defaultdict, Counter
import re

def kasiski_examination(ciphertext: str, min_sub_len=3) -> list:
    ciphertext = ciphertext.upper().replace(" ", "")
    seq_positions = defaultdict(list)
    # 记录所有长度为 min_sub_len 的子串位置
    for i in range(len(ciphertext) - min_sub_len + 1):
        seq = ciphertext[i:i+min_sub_len]
        seq_positions[seq].append(i)
    
    # 过滤出至少出现两次的子串
    repeated_seq = {seq: pos for seq, pos in seq_positions.items() if len(pos) > 1}
    distances = []
    for seq, positions in repeated_seq.items():
        for i in range(len(positions)-1):
            distances.append(positions[i+1] - positions[i])
    
    # 求各距离的公约数并统计出现次数
    possible_key_lengths = Counter()
    for d in distances:
        # 考虑因子范围2 ~ d
        for factor in range(2, d+1):
            if d % factor == 0:
                possible_key_lengths[factor] += 1
                
    # 返回可能的密钥长度从高频到低频排列结果
    return possible_key_lengths.most_common()

def friedman_test(ciphertext: str) -> float:
    ciphertext = re.sub("[^A-Z]", "", ciphertext.upper())
    N = len(ciphertext)
    if N <= 1:
        return 0
    frequency = Counter(ciphertext)
    ic = sum(v * (v-1) for v in frequency.values()) / (N*(N-1))
    # 假设英文随机文本IC约为0.038，标准文本IC约为0.065  
    # 参考公式：K ≈ (0.027N) / ((N - 1)IC - 0.038N + 0.065)
    estimated_key_length = (0.027 * N) / ((N - 1) * ic - 0.038 * N + 0.065) if ic != 0 else 0
    return estimated_key_length

# 示例使用
if __name__ == "__main__":
    # 假定密文
    cipher_text = "LXFOPVEFRNHR"
    print("Kasiski 检测结果：", kasiski_examination(cipher_text))
    print("Friedman 检测估计密钥长度：", friedman_test(cipher_text))
