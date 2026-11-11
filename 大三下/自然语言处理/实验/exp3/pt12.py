# -*- coding: utf-8 -*-
#环境 tf12
import numpy as np
from sklearn.model_selection import train_test_split
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input, Embedding, Bidirectional, LSTM, Dense
import tensorflow as tf
import tensorflow_addons as tfa
import re
import os

# 配置GPU加速（确保使用兼容Windows 11的CUDA 11.8+）
os.environ['TF_GPU_ALLOCATOR'] = 'cuda_malloc_async'  # 提高GPU内存分配效率
physical_devices = tf.config.list_physical_devices('GPU')
if physical_devices:
    tf.config.experimental.set_memory_growth(physical_devices[0], True)

def parse_data(file_path):
    sentences = []
    tags = []
    
    # 使用 'gbk' 编码打开文件（兼容ANSI编码的中文文件）
    with open(file_path, 'r', encoding='gbk') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
                
            # 分割每行的内容（去掉开头的日期ID部分）
            parts = line.split()
            if not parts:
                continue
                
            # 提取词语和词性标注
            words = []
            word_tags = []
            for part in parts[1:]:  # 跳过第一个日期ID部分
                # 使用正则匹配词语和词性标注
                match = re.match(r'^(.+)/([a-zA-Z]+)$', part)
                if match:
                    word = match.group(1)
                    tag = match.group(2).lower()  # 统一转为小写
                    words.append(word)
                    word_tags.append(tag)
            
            if words and word_tags:
                sentences.append(words)
                tags.append(word_tags)
    
    return sentences, tags

# 数据加载和预处理
sentences, tags = parse_data('dataset.txt')

# 构建词汇表和标签表
words = list(set(word for sent in sentences for word in sent))
tags_flat = ["PAD"] + list(set(tag for tag_seq in tags for tag in tag_seq))
word2idx = {w: i + 1 for i, w in enumerate(words)}
word2idx["PAD"] = 0
word2idx["UNK"] = len(word2idx)
tag2idx = {t: i for i, t in enumerate(tags_flat)}
idx2tag = {i: t for t, i in tag2idx.items()}

# 动态计算最大长度（95%分位数避免异常值影响）
sent_lengths = [len(sent) for sent in sentences]
max_len = int(np.percentile(sent_lengths, 95))  # 取95%分位数作为max_len
num_tags = len(tag2idx)

# 序列填充
X = [[word2idx.get(w, word2idx["UNK"]) for w in s] for s in sentences]
X = pad_sequences(X, maxlen=max_len, padding='post', value=word2idx["PAD"])

y = [[tag2idx[t] for t in ts] for ts in tags]
y = pad_sequences(y, maxlen=max_len, padding='post', value=tag2idx["PAD"])

# 数据集划分
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

y_train = y_train.astype(np.int32)  # 标签必须为整型


# 构建模型（使用TF2.x风格）
input_layer = Input(shape=(max_len,), dtype=tf.int32)
embedding = Embedding(input_dim=len(word2idx), output_dim=64, mask_zero=True)(input_layer)
lstm = Bidirectional(LSTM(64, return_sequences=True))(embedding)
# logits = Dense(num_tags)(lstm)  # 输出logits
logits = Dense(num_tags, dtype='float32')(lstm)  # 替换原有输出层


model = Model(inputs=input_layer, outputs=logits)

# CRF层配置（兼容TF2.12+）
transition_params = tf.Variable(
    tf.random.uniform(shape=(num_tags, num_tags)),
    trainable=True,  # 显式设置为可训练参数
    name="crf_transitions"
)

@tf.function
def crf_loss(y_true, y_pred):
    y_true = tf.cast(y_true, tf.int32)
    y_pred = tf.cast(y_pred, tf.float32)  # 显式类型转换
    sequence_lengths = tf.reduce_sum(tf.cast(tf.not_equal(y_true, 0), tf.int32), axis=1)
    log_likelihood, _ = tfa.text.crf_log_likelihood(
        y_pred, y_true, sequence_lengths, transition_params
    )
    return -tf.reduce_mean(log_likelihood)

# 使用AdamW优化器（更适合现代Transformer类模型）
optimizer = tf.keras.optimizers.AdamW(learning_rate=0.001)
model.compile(optimizer=optimizer, loss=crf_loss)

# 训练模型（添加回调函数）
callbacks = [
    tf.keras.callbacks.EarlyStopping(patience=3, restore_best_weights=True),
    tf.keras.callbacks.ModelCheckpoint('best_model.h5', save_best_only=True)
]

model.fit(
    X_train, y_train,
    batch_size=32,  # 增大batch size提高GPU利用率
    epochs=1,
    validation_data=(X_test, y_test),
    callbacks=callbacks
)

# 预测函数
def predict_tags(text, model, word2idx, idx2tag, max_len):
    test_seq = [[word2idx.get(w, word2idx["UNK"]) for w in text]]
    test_pad = pad_sequences(test_seq, maxlen=max_len, padding='post', value=word2idx["PAD"])
    
    logits_val = model.predict(test_pad, verbose=0)
    sequence_lengths = [len(text)]
    
    viterbi_seq, _ = tfa.text.crf_decode(
        logits_val, transition_params, sequence_lengths
    )
    
    pred_ids = viterbi_seq.numpy()[0][:len(text)]
    return [idx2tag[i] for i in pred_ids]

# 测试预测

test_sent = ['3', '月', '23', '日', '下午', '青年报', '杯赛', 'U19', '邀请赛', '在', '越南', '芽庄', '进行', '前', '国脚', '曲波',
            '挂帅', '的', '中国', 'U19B', '队', '迎战', '泰国', 'U19', '上半场', '国青队', '的', '门户', '大开', '泰国', '在', '第', '11', '分钟', '和', '第', '17', '分钟', '连进', '2', '球', '半场', '国青', '0', '射门', '0', '角球', '几乎', '被', '完全', '压制', '下半场', '国青', '的', '进攻', '一度', '有所', '起色', '并', '由', '马辅', '渔', '利用', '远射', '扳回', '一', '球', '但', '最终', '未能', '扳平', '比分', '全场', '比赛', '结束', '国青',
            '1', '2', '输球', '继', '中国', '杯国足', '0', '1', '输给', '泰国', '之后', '3', '天内', '遭遇', '泰国', '足球', '双杀']

pred_tags = predict_tags(test_sent, model, word2idx, idx2tag, max_len)

print("\n预测结果：")
print(list(zip(test_sent, pred_tags)))