import os
import math
import nltk
from nltk.tokenize import word_tokenize
from collections import defaultdict, Counter

START_TOKEN = "<s>"
END_TOKEN = "</s>"

def read_data(file_path): # 把含有__eou__的句集合分成多个单独句子
    sentences = []
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if line:
                parts = [part.strip() for part in line.split("__eou__") if part.strip()]
                sentences.extend(parts)
    return sentences

def preprocess_sentence(sentence):
    tokens = word_tokenize(sentence) # 把一个句子分割成多个词
    return [START_TOKEN] + tokens + [END_TOKEN]

class UnigramLM:
    def __init__(self):
        self.unigram_counts = Counter() #记录每个词出现频率
        self.total_count = 0
        self.vocab = set()
    
    def train(self, sentences):
        for sentence in sentences: #update函数可以将tokens(可迭代操作)中的每个词都拿出来进行操作
            tokens = preprocess_sentence(sentence)
            self.unigram_counts.update(tokens)  # 每个词的频率
            self.vocab.update(tokens) # 每个词插入集合
        self.total_count = sum(self.unigram_counts.values()) # 记录所有单词总个数（不去重）
    
    def prob(self, word):
        #使用加一平滑计算概率：P(word) = (count(word) + 1) / (total_count + |V|)
        return (self.unigram_counts[word] + 1) / (self.total_count + len(self.vocab))
    
    def perplexity(self, sentences):
        total_log_prob = 0
        total_words = 0
        for sentence in sentences:
            tokens = preprocess_sentence(sentence)
            total_words += len(tokens)
            for token in tokens:
                total_log_prob += math.log(self.prob(token))
        avg_log_prob = total_log_prob / total_words
        return math.exp(-avg_log_prob)

class BigramLM:
    def __init__(self):
        # bigram_counts: 前一个词 -> 后一个词的计数，即两个词同时出现的概率
        self.bigram_counts = defaultdict(Counter)
        # unigram_counts: 统计前一个词的出现次数
        self.unigram_counts = Counter()
        self.vocab = set()
    
    def train(self, sentences):
        for sentence in sentences:
            tokens = preprocess_sentence(sentence)
            self.vocab.update(tokens)
            self.unigram_counts.update(tokens)
            for i in range(1, len(tokens)):
                prev_word = tokens[i-1]
                curr_word = tokens[i]
                self.bigram_counts[prev_word][curr_word] += 1
    
    def prob(self, prev_word, word):
        return (self.bigram_counts[prev_word][word] + 1) / (self.unigram_counts[prev_word] + len(self.vocab))
    
    def perplexity(self, sentences):
        total_log_prob = 0
        total_words = 0
        for sentence in sentences:
            tokens = preprocess_sentence(sentence)
            total_words += (len(tokens) - 1)  # bigram从第1个bigram开始计算
            for i in range(1, len(tokens)):
                total_log_prob += math.log(self.prob(tokens[i-1], tokens[i]))
        avg_log_prob = total_log_prob / total_words
        return math.exp(-avg_log_prob)

if __name__ == '__main__':
    train_file = 'train_LM.txt'
    test_file = 'test_LM.txt'
    
    train_sentences = read_data(train_file)
    test_sentences = read_data(test_file)
    
    uni_model = UnigramLM()
    uni_model.train(train_sentences)
    
    bi_model = BigramLM()
    bi_model.train(train_sentences)
    
    uni_ppl = uni_model.perplexity(test_sentences)
    bi_ppl = bi_model.perplexity(test_sentences)
    
    print("Unigram perplexity:", uni_ppl)
    print("Bigram perplexity:", bi_ppl)
