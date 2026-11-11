input_file = "chinese-train.txt"
output_file = "small-chinese.txt"

with open(input_file, "r", encoding="utf-8") as f_in:
    with open(output_file, "w", encoding="utf-8") as f_out:
        # 逐行读取，内存友好
        for line_num, line in enumerate(f_in, 1):
            f_out.write(line)
            if line_num >= 10000:
                break