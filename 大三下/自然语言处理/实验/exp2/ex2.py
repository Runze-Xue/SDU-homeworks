import jieba

# 读取待分词中文文本
jieba.load_userdict("userdict.txt")
with open("Chinese.txt", "r", encoding="utf-8") as f:
    text = f.read()

# 精确模式
words_exact = jieba.cut(text, cut_all=False)
# 全模式
words_full = jieba.cut(text, cut_all=True)
# 搜索引擎模式
words_search = jieba.cut_for_search(text)

print("精确模式：")
print("/ ".join(words_exact))
print("\n全模式：")
print("/ ".join(words_full))
print("\n搜索引擎模式：")
print("/ ".join(words_search))

print("snownlp 分词：")
from snownlp import SnowNLP
s = SnowNLP(text)
print(s.words)

print("thulac 分词：")
import thulac
thu = thulac.thulac()
seg_list = thu.cut(text, text=True)
print(seg_list)

print("NLPIR pynlpir 分词：")
import pynlpir
pynlpir.open()
seg_list = pynlpir.segment(text, pos_tagging=False)
print(seg_list)
pynlpir.close()

import nltk
from nltk.tokenize import word_tokenize

with open("English.txt", "r", encoding="utf-8") as f:
    english_text = f.read()

tokens = word_tokenize(english_text)
print("NLTK 分词结果：")
print(tokens)

import spacy

nlp = spacy.load("en_core_web_sm")
doc = nlp(english_text)
tokens = [token.text for token in doc]
print("SpaCy 分词结果：")
print(tokens)

with open("Chinese.txt", "r", encoding="utf-8") as f:
    text_zh = f.read()

with open("English.txt", "r", encoding="utf-8") as f:
    text_en = f.read()


from stanfordcorenlp import StanfordCoreNLP
corenlp_path = "stanford-corenlp-4.5.6"
nlp_zh = StanfordCoreNLP(r'./stanford-corenlp-4.5.6',lang='zh')
nlp_en = StanfordCoreNLP(r'./stanford-corenlp-4.5.6',lang='en')


tokens_zh = nlp_zh.word_tokenize(text_zh)
tokens_en = nlp_en.word_tokenize(text_en)
print("stanford 中文分词结果: ", tokens_zh )
print("stanford 英文分词结果: ", tokens_en )

nlp_zh.close()
nlp_en.close()
