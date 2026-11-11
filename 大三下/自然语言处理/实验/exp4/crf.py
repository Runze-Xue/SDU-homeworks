import tensorflow as tf
from transformers import TFBertModel, BertTokenizer
from keras.layers import Dense, Bidirectional, LSTM, Dropout, Input
from keras.models import Model
from keras.optimizers import Adam
from keras_crf import CRF  # 用的是 keras-crf 而非 tensorflow_addons
import os

# 设置代理
os.environ['http_proxy'] = 'http://127.0.0.1:7890'
os.environ['https_proxy'] = 'http://127.0.0.1:7890'

# 加载预训练 BERT
bert_model_name = 'bert-base-multilingual-cased'
tokenizer = BertTokenizer.from_pretrained(bert_model_name)
bert = TFBertModel.from_pretrained(bert_model_name, from_pt=True)

def create_model(max_len, num_tags):
    input_ids = Input(shape=(max_len,), dtype=tf.int32, name='input_ids')
    attention_mask = Input(shape=(max_len,), dtype=tf.int32, name='attention_mask')

    # BERT
    bert_outputs = bert(input_ids, attention_mask=attention_mask)[0]  # shape: (batch, max_len, hidden_size)

    # BiLSTM + Dropout
    x = Bidirectional(LSTM(128, return_sequences=True))(bert_outputs)
    x = Dropout(0.1)(x)

    # Dense + CRF
    x = Dense(num_tags)(x)
    crf = CRF(num_tags)
    output = crf(x)

    model = Model(inputs=[input_ids, attention_mask], outputs=output)
    model.compile(optimizer=Adam(5e-5), loss=crf.loss_function, metrics=[crf.accuracy])
    return model

# 参数
max_len = 128
num_tags = 9  # 替换为你实际的标签数

model = create_model(max_len, num_tags)
model.summary()
