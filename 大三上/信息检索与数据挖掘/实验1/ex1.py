import os
import json
import sys
import numpy as np 
import pandas as pd
from pathlib import Path
nd = 'D:/大学/大三上/信息检索与数据挖掘/实验课/实验1'
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
word_to_docid = {}
Set_all = set()
for tweet in tweets:
    text = (tweet['text'])
    spilit_text = text.split()
    tmps = set()
    Set_all.add(docid)
    for s in spilit_text:
        ts = s.lower()
        if ts not in tmps:
            tmps.add(ts)
            if ts in word_to_docid:
                word_to_docid[ts].append(docid)
            else:
                word_to_docid[ts] = []
                word_to_docid[ts].append(docid)
    docid = docid + 1
# word_to_docid 记录的是单词，与对应docid之间的关系

def calc_and(a, b):
    l = []
    a_tag = 0
    b_tag = 0
    while a_tag < len(a) and b_tag < len(b):
        if a[a_tag] == b[b_tag]:
            l.append(a[a_tag])
            a_tag += 1
            b_tag += 1
        elif a[a_tag] < b[b_tag]:
            a_tag += 1
        else :
            b_tag += 1
    return l
def calc_or(a, b):
    S = set()
    l = []
    for i in a:
        S.add(i)
    for i in b:
        S.add(i)
    for i in S:
        l.append(i)
    return l
def calc_not(a):
    l = []
    for i in Set_all:
        if i not in a:
            l.append(i)
    return l

def calc_and_not(a, b):
    c = calc_not(b)
    d = calc_and(a, c)
    return d
CalcMod = input("Input Mod:  ")
CalcMod = CalcMod.lower()
if CalcMod == "not":
    word1 = input("Input the word:  ")
    word1 = word1.lower()
elif CalcMod == 'and' or CalcMod == 'or' or CalcMod =='and not' :
    word1 = input("Input the first word:  ")
    word1 = word1.lower()
    word2 = input("Input the second word:  ")
    word2 = word2.lower()
else:
    print("Wrong Mod")
    sys.exit() 
if CalcMod == 'and':
    l2 = calc_and(word_to_docid[word1], word_to_docid[word2])
elif CalcMod == 'or':
    l2 = calc_or(word_to_docid[word1], word_to_docid[word2])
elif CalcMod == 'not':
    l2 = calc_not(word_to_docid[word1])
elif CalcMod == 'and not':
    l2 = calc_and_not(word_to_docid[word1], word_to_docid[word2])
    
l2.sort()
print(l2)
with open('output.txt', 'w', encoding='utf-8') as file:
    for item in l2:
        file.write(f"{item}\n")