import os
import re
import jieba
import zhconv
from gensim.corpora import WikiCorpus
from gensim.models import Word2Vec
import multiprocessing
from gensim.models import word2vec

def extract_wiki(input_file, output_file, limit=50000):
    wiki_corpus = WikiCorpus(input_file, dictionary={})
    with open(output_file, 'w', encoding='utf-8') as f:
        i = 0
        for text in wiki_corpus.get_texts():
            f.write(' '.join(text) + '\n')
            i += 1
            if i % 10000 == 0:
                print(f'已处理 {i} 篇文章')
            if i >= limit:
                break

def clean_and_cut(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as fin, \
         open(output_file, 'w', encoding='utf-8') as fout:
        for line in fin:
            line = zhconv.convert(line, 'zh-cn')  
            line = re.sub(r'[^\u4e00-\u9fa5]', '', line)  
            words = jieba.cut(line)
            fout.write(' '.join(words) + '\n')

def train_word2vec(corpus_file, model_path):
    sentences = list(word2vec.LineSentence(corpus_file))  
    model = Word2Vec(
        vector_size=100,
        window=5,
        min_count=5,
        workers=multiprocessing.cpu_count()
    )
    model.build_vocab(sentences)      
    model.train(sentences, total_examples=model.corpus_count, epochs=model.epochs)
    model.save(model_path)
    return model

def test_model(model, aim_word): #aim_word type=str
    print(aim_word+"词向量：", model.wv[aim_word])
    print("与"+aim_word+"相近的词：")
    for word, score in model.wv.most_similar(aim_word, topn=10):
        print(f"{word} : {score:.4f}")
if __name__ == '__main__':
    input_file_name = 'zhwiki-latest-pages-articles.xml.bz2'    
    output_file_name = 'corpus_cn.txt'
    clean_corpus_file = 'corpus_cut.txt'
    model_path = 'wiki_word2vec.model'
    if not os.path.exists(output_file_name):
        extract_wiki(input_file_name, output_file_name)
    print('over extract_wiki')
    if not os.path.exists(clean_corpus_file):
        clean_and_cut(output_file_name, clean_corpus_file)
    print('over clean_and _cut')
    if not os.path.exists(model_path):
        model = train_word2vec(clean_corpus_file, model_path)
    else:
        model = Word2Vec.load(model_path)
    test_model(model,"助教")
