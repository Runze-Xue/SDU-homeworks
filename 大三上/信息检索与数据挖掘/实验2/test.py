import os
import json
import math
import numpy as np

# 设置路径
nd = 'D:/大学/大三上/信息检索与数据挖掘/实验2'
os.chdir(nd)

# 读取tweets数据
with open('tweets.txt', 'r', encoding='utf-8') as file:
    tweets = []
    for line in file:
        try:
            tweet = json.loads(line)
            tweets.append(tweet)
        except json.JSONDecodeError as e:
            print(f"JSONDecodeError: {e}")

# 初始化变量
docid = 1
word_to_docid = {}  # 倒排索引，word -> document
allwordsset = set()
DocFc = {}  # 词 -> 出现的文档数
TermFc = {}  # 词 -> 文档 -> 出现次数

# 构建倒排索引表和正向索引表
for tweet in tweets:
    text = tweet['text']
    spilit_text = text.split()
    tmps = set()
    for s in spilit_text:
        ts = s.lower()
        allwordsset.add(ts)
        
        # 正向索引表
        if ts not in TermFc:
            TermFc[ts] = {}
        if docid not in TermFc[ts]:
            TermFc[ts][docid] = 1
        else:
            TermFc[ts][docid] += 1
        
        # 倒排索引表
        if ts not in tmps:
            tmps.add(ts)
            if ts in word_to_docid:
                word_to_docid[ts].append(docid)
            else:
                word_to_docid[ts] = [docid]
            
            # 记录DF
            if ts not in DocFc:
                DocFc[ts] = 1
            else:
                DocFc[ts] += 1

    docid += 1

# 查询输入
query_in = input("Enter query: ")
query_terms = query_in.lower().split()

# 构建查询向量Q
Q_list = list(set(query_terms))
num_docs = docid - 1
num_terms = len(Q_list)
query_vector = []

# 构建查询向量的权重
for term in Q_list:
    tf = query_terms.count(term)
    idf = math.log10(num_docs / (1 + DocFc.get(term, 0)))
    query_vector.append((1 + math.log10(tf)) * idf if tf > 0 else 0)

# 计算每个文档的向量并求余弦相似度
cosine_scores = {}
for doc_id in range(1, num_docs + 1):
    doc_vector = []
    doc_norm = 0
    for term in Q_list:
        tf = TermFc.get(term, {}).get(doc_id, 0)
        doc_tf_weight = 1 + math.log10(tf) if tf > 0 else 0
        doc_vector.append(doc_tf_weight)
        doc_norm += doc_tf_weight ** 2
    
    # 归一化文档向量
    doc_norm = math.sqrt(doc_norm)
    doc_vector = [x / doc_norm if doc_norm > 0 else 0 for x in doc_vector]
    
    # 计算余弦相似度
    cosine_score = np.dot(doc_vector, query_vector)
    cosine_scores[doc_id] = cosine_score

# 排序并输出Top K结果
K = 10
top_k_docs = sorted(cosine_scores.items(), key=lambda item: item[1], reverse=True)[:K]
print("Top ",K ,"relevant documents:", top_k_docs)
