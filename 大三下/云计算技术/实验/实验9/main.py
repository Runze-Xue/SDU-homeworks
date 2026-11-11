from key_generator import generate_large_prime, calculate_private_key_d
from hash_mapping import hash_map
import time
#generate_large_prime(bits)
#calculate_private_key_d(e, phi_n)
#hash_map(type=string)
if __name__ == '__main__':
    e = 65537 # 公钥
    bits_list =[5, 10, 11, 12, 13, 14]
    content = "I love cloud computing technology. "
    print("content:", content)
    for bits in bits_list:
        start_time = time.time()
        p = generate_large_prime(bits)
        q = generate_large_prime(bits)
        while p== q:
            q = generate_large_prime(bits)
        phi_n = (p-1) *(q-1)
        mod_number = p * q
        d = calculate_private_key_d(e, phi_n)
        print('bits:', bits)
        print("模数", mod_number)
        print("公钥", e)
        print("私钥", d)
        hash_num = hash_map(content) % mod_number
        print("hash num ", hash_num)
        encoding = pow(hash_num, d) % mod_number
        decoding = pow(encoding, e) % mod_number
        print("真实哈希值为", hash_num)
        print("数字签名解密后的哈希值为",decoding)
        if(hash_num == decoding):
            print("数据有效")
        else:
            print("数据无效")
        end_time = time.time()
        elapsed_time = end_time - start_time
        print("用时：", elapsed_time)

