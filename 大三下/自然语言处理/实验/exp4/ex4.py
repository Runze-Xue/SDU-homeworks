with open('Chinese.txt', 'r', encoding='utf-8') as file:
    chinese_content = file.read()
with open('English.txt', 'r', encoding='utf-8') as file:
    english_content = file.read()

import nltk
from nltk import word_tokenize, pos_tag, ne_chunk
def nltk_ner(text):
    tokens = word_tokenize(text)
    pos_tags = pos_tag(tokens)
    named_entities = ne_chunk(pos_tags)
    return named_entities
print("NLTK NER结果:")
print(nltk_ner(english_content))



import spacy
nlp = spacy.load("en_core_web_sm")
def spacy_ner(text):
    doc = nlp(text)
    return [(ent.text, ent.label_) for ent in doc.ents]
print("\nSpacy NER结果:")
print(spacy_ner(english_content))


import stanza
# 初始化pipeline
nlp = stanza.Pipeline('en', processors='tokenize,ner')  # 中文使用 'zh'
def stanza_ner(text):
    doc = nlp(text)
    entities = []
    for sentence in doc.sentences:
        for ent in sentence.ents:
            entities.append((ent.text, ent.type))
    return entities
print("\nStanza (StanfordNLP) NER结果:")
print(stanza_ner(english_content))



import jieba.posseg as pseg
def jieba_ner(text):
    words = pseg.cut(text)
    return [(word, flag) for word, flag in words]
print("\nJieba NER结果:")
print(jieba_ner(chinese_content))


# from snownlp import SnowNLP
# def snownlp_ner(text):
#     s = SnowNLP(text)
#     return s.words
# print("\nSnowNLP NER结果:")
# print(snownlp_ner(chinese_content))


import thulac
thu = thulac.thulac(seg_only=False)
def thulac_ner(text):
    return thu.cut(text)

print("\nTHULAC NER结果:")
print(thulac_ner(chinese_content))



import pynlpir
pynlpir.open()
def nlpir_ner(text):
    pynlpir.open()
    segments = pynlpir.segment(text, pos_names='all')
    return segments
print("\nNLPIR NER结果:")
print(nlpir_ner(chinese_content))
pynlpir.close()