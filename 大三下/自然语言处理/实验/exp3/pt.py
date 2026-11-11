# -*- coding: utf-8 -*-
#环境：tf
import numpy as np
from sklearn.model_selection import train_test_split
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input, Embedding, Bidirectional, LSTM, Dense
import tensorflow as tf
import tensorflow_addons as tfa
import re

def parse_data(file_path):
    sentences = []
    tags = []
    with open(file_path, 'r', encoding='gbk') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
                
            parts = line.split()
            if not parts:
                continue

            words = []
            word_tags = []
            for part in parts[1:]:  

                match = re.match(r'^(.+)/([a-zA-Z]+)$', part)
                if match:
                    word = match.group(1)
                    tag = match.group(2).lower() 
                    words.append(word)
                    word_tags.append(tag)
            
            if words and word_tags:
                sentences.append(words)
                tags.append(word_tags)
    
    return sentences, tags


def conllu_to_sentences_tags(conllu_file):
    sentences = []
    tags = []
    current_sentence = []
    current_tags = []
    
    with open(conllu_file, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            
            if line.startswith('#') or not line:

                if line.startswith('# sent_id') and current_sentence:
                    sentences.append(current_sentence)
                    tags.append(current_tags)
                    current_sentence = []
                    current_tags = []
                continue
                

            fields = line.split('\t')
            if len(fields) >= 4:  
                word = fields[1]  
                pos_tag = fields[3]  
                
                current_sentence.append(word)
                current_tags.append(pos_tag.lower())  
    
    if current_sentence:
        sentences.append(current_sentence)
        tags.append(current_tags)
        
    return sentences, tags


# sentences, tags = parse_data('dataset.txt')
conllu_file = 'enronsent00_01.xml.conllu' 
sentences, tags = conllu_to_sentences_tags(conllu_file)


words = list(set(word for sent in sentences for word in sent))
tags_flat = ["PAD"] + list(set(tag for tag_seq in tags for tag in tag_seq))
word2idx = {w: i + 1 for i, w in enumerate(words)}
word2idx["PAD"] = 0
word2idx["UNK"] = len(word2idx)
tag2idx = {t: i for i, t in enumerate(tags_flat)}
idx2tag = {i: t for t, i in tag2idx.items()}

sent_lengths = [len(sent) for sent in sentences]
max_len = np.max(sent_lengths) 
mean_len = np.mean(sent_lengths) 
num_tags = len(tag2idx)

X = [[word2idx.get(w, word2idx["UNK"]) for w in s] for s in sentences]
X = pad_sequences(X, maxlen=max_len, padding='post', value=word2idx["PAD"])

y = [[tag2idx[t] for t in ts] for ts in tags]
y = pad_sequences(y, maxlen=max_len, padding='post', value=tag2idx["PAD"])

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)


input_layer = Input(shape=(max_len,))
embedding = Embedding(input_dim=len(word2idx), output_dim=64, mask_zero=True)(input_layer)
lstm = Bidirectional(LSTM(64, return_sequences=True))(embedding)
logits = Dense(num_tags)(lstm) 

model = Model(inputs=input_layer, outputs=logits)

transition_params = tf.Variable(tf.random.uniform(shape=(num_tags, num_tags))) 

@tf.function
def crf_loss(y_true, y_pred):
    y_true = tf.cast(y_true, dtype=tf.int32)
    sequence_lengths = tf.math.reduce_sum(tf.cast(tf.math.not_equal(y_true, 0), tf.int32), axis=1)
    log_likelihood, _ = tfa.text.crf_log_likelihood(
        inputs=y_pred,
        tag_indices=y_true,
        sequence_lengths=sequence_lengths,
        transition_params=transition_params
    )
    return -tf.reduce_mean(log_likelihood)

model.compile(optimizer='adam', loss=crf_loss)


model.fit(X_train, y_train, batch_size=2, epochs=10)



# test_sent = ['3', '月', '23', '日', '下午', '青年报', '杯赛', 'U19', '邀请赛', '在', '越南', '芽庄', '进行', '前', '国脚', '曲波',
#               '挂帅', '的', '中国', 'U19B', '队', '迎战', '泰国', 'U19', '上半场', '国青队', '的', '门户', '大开', '泰国', '在', '第', '11', '分钟', '和', '第', '17', '分钟', '连进', '2', '球', '半场', '国青', '0', '射门', '0', '角球', '几乎', '被', '完全', '压制', '下半场', '国青', '的', '进攻', '一度', '有所', '起色', '并', '由', '马辅', '渔', '利用', '远射', '扳回', '一', '球', '但', '最终', '未能', '扳平', '比分', '全场', '比赛', '结束', '国青',
#             '1', '2', '输球', '继', '中国', '杯国足', '0', '1', '输给', '泰国', '之后', '3', '天内', '遭遇', '泰国', '足球', '双杀']

test_sent = ['Xi', 'Jinping', ',', 'male', ',', 'Han', 'ethnicity', ',', 'was', 'born', 'in', 'June', '1953', 'and', 'is', 'from', 'Fuping', ',', 'Shaanxi', 'Province', '.' ,
 'He', 'began', 'his', 'first', 'job', 'in', 'January', '1969', 'and', 'joined', 'the', 'Communist', 'Party', 'of', 'China', '(', 'CPC', ')', 'in', 'January', '1974', '.',
 'Xi', 'graduated', 'from', 'School', 'of', 'Humanities', 'and', 'Social', 'Sciences', ',', 'Tsinghua', 'University', 'where', 'he', 'completed', 'an', 'in-service', 'graduate', 'program', 'in', 'Marxist', 'theory', 'and', 'ideological', 'and', 'political', 'education', '.',
'He', 'holds', 'a', 'Doctor', 'of', 'Law', 'degree', '.','Xi', 'is', 'currently', 'General', 'Secretary', 'of', 'the', 'CPC', 'Central', 'Committee', ',', 'Chairman', 'of', 'the', 'CPC', 'Central', 'Military', 'Commission', ',', 'President', 'of', 'the', 'People', "'s", 'Republic', 'of', 'China', '(', 'PRC', ')', ',', 'and', 'Chairman', 'of', 'the', 'PRC', 'Central', 'Military', 'Commission', '.']

test_seq = [[word2idx.get(w, word2idx["UNK"]) for w in test_sent]]
test_pad = pad_sequences(
    test_seq, 
    maxlen=max_len, 
    padding='post', 
    value=word2idx["PAD"]  
)

logits_val = model.predict(test_pad)

sequence_lengths = tf.math.reduce_sum(
    tf.cast(tf.math.not_equal(test_pad, word2idx["PAD"]), tf.int32),
    axis=1
)

sequence_lengths = tf.minimum(sequence_lengths, max_len)

viterbi_seq, _ = tfa.text.crf_decode(
    logits_val, 
    transition_params, 
    sequence_lengths
)

pred_ids = viterbi_seq.numpy()[0][:len(test_sent)]
pred_tags = [idx2tag[i] for i in pred_ids]

print("\n预测结果：")
print(list(zip(test_sent, pred_tags)))