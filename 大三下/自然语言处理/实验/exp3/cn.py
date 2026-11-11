# -*- coding: utf-8 -*-
import jieba.posseg as pseg
import stanza
from snownlp import SnowNLP
import thulac
import pynlpir

def chinese_pos_tagging(file_path):
    # 读取文件
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read().strip()

    # 1. Jieba 词性标注
    jieba_tags = pseg.cut(text)
    print("\n=== Jieba 标注结果 ===")
    for word, pos in jieba_tags:
        print(f"{word}/{pos}", end=" ")

    # 2. Stanza 词性标注
    nlp = stanza.Pipeline('zh', processors='tokenize,pos', download_method=None, proxies=proxies)
    doc = nlp(text)
    print("\n\n=== Stanza 标注结果 ===")
    for sent in doc.sentences:
        for word in sent.words:
            print(f"{word.text}/{word.upos}", end=" ")

    # 3. SnowNLP 分词（无词性标注）
    s = SnowNLP(text)
    print("\n\n=== SnowNLP 分词结果 ===")
    print(list(s.tags))
    # 4. THULAC 词性标注
    thu = thulac.thulac(seg_only=False)
    thu_tags = thu.cut(text)
    print("\n\n=== THULAC 标注结果 ===")
    for word, pos in thu_tags:
        print(f"{word}/{pos}", end=" ")

    # 5. NLPIR 词性标注
    pynlpir.open()
    nlpir_tags = pynlpir.segment(text)
    print("\n\n=== NLPIR 标注结果 ===")
    for word, pos in nlpir_tags:
        print(f"{word}/{pos}", end=" ")
    pynlpir.close()


if __name__ == "__main__":
    proxies = {
    "http": "http://127.0.0.1:7890",
    "https": "http://127.0.0.1:7890"
    }

# 下载模型时指定代理    
    chinese_pos_tagging("Chinese.txt")