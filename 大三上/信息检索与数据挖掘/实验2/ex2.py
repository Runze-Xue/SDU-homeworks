# 1、倒排索引表 2、正向索引表（存document-word-次数）
# 构建 map[word, doc] 映射，存储对应 tf-idf值 tf从正向索引表查，对应 tf_w,d idf 由倒排索引表查
# 做每一次 query时，解出query中含有的所有词
# 循环所有document，循环query中所有词，进行 document[tf-idf1, tf-idf……]向量计算
# 对所有document向量进行cos计算，完成后进行排序

import os
import json
import math
import sys
import numpy as np 
# import pandas as pd
from pathlib import Path
nd = 'D:/大学/大三上/信息检索与数据挖掘/实验2'
os.chdir(nd)

with open('tweets.txt', 'r', encoding='utf-8') as file:
    tweets = []
    for line in file:
        try:
            tweet = json.loads(line)
            tweets.append(tweet)
        except json.JSONDecodeError as e:
            print(f"JSONDecodeError: {e}")

docid = 1
word_to_docid = {} #倒排索引，word -> document
allwordsset = set()
DocFc = {} # 词->占文档数(key->value)
TermFc = {} #词->文档->出现次数 key->key2->value
for tweet in tweets: # 每个tweet是一个文档
    text = (tweet['text']) #取该文档的text部分
    spilit_text = text.split() 
    tmps = set() #记录每个词 
    #从文档text中，遍历每一个词
    for s in spilit_text: # s 是一个分词
        ts = s.lower()
        allwordsset.add(ts)
        if ts not in TermFc:
            TermFc[ts] = {}
            TermFc[ts][docid] = 1
        else:
            if docid not in TermFc[ts]:
                TermFc[ts][docid] = 1
            else:
                TermFc[ts][docid] += 1
        
        if ts not in tmps:# 检测该词在文档中是不是第一次出现
            tmps.add(ts)
            #构建倒排索引开始
            if ts in word_to_docid:
                word_to_docid[ts].append(docid)
            else:
                word_to_docid[ts] = []
                word_to_docid[ts].append(docid)
            #构建倒排索引结束
            #构建DF开始
            if ts not in DocFc:
                DocFc[ts] = 1
            else:
                DocFc[ts] += 1
            #构建DF结束
        
    docid = docid + 1

#将所有词有序化存入allwords列表
allwords = []
for element in allwordsset:
    allwords.append(element)


query_in = input()
query_mes = query_in.split()

Q_set = set()
for element in query_mes:
    Q_set.add(element)
Q_list = []
for element in Q_set:
    Q_list.append(element)

wtd = [[None for _ in range(len(Q_list) + 1)] for _ in range(docid)] #docid + 1行，len + 1列的数组
# 对所有文档，对查询中所有非重合词
# 所有角标更改为从1开始， 构建向量
#DocFc = {} # 词->占文档数(key->value)
#TermFc = {} #词->文档->出现次数 key->key2->value
cos_value = {} # docid -> value
for row in range(docid):# row -> docid, col -> words，遍历每个文档，每个文裆中所有词
    print(row)
    if row == 0:
        continue
    col = 1
    sum_on = 0
    for index_col in allwordsset:
        #检查该词是不是在查询中
        if index_col not in Q_set:
            continue
        #检查该词是不是在遍历文档中
        #词既在查询中，又在遍历文档中
        # print(docid)
        if index_col in TermFc and docid in TermFc[index_col]:
            wtd[row][col] = math.log10(1 + TermFc[index_col][docid]) * math.log10(len(allwordsset) / DocFc(index_col))
            sum_on = sum_on + wtd[row][col]
        else:#词在查询中，但是不在遍历文档中，置0
            wtd[row][col] = 0
        col = col + 1
    #归一化向量
        


# print("YES")
# print(query_mes)