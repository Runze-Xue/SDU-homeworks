def hash_map(input_string: str) -> int:
    if not isinstance(input_string, str):
        raise TypeError("wrong_type")
    hash_value = 0
    prime_multiplier = 31  
    modulus = 2**32       
    for char in input_string:
        hash_value = (hash_value * prime_multiplier + ord(char)) % modulus
    return hash_value
