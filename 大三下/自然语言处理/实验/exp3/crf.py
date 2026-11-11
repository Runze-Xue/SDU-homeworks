import numpy as np
from sklearn.model_selection import train_test_split
from keras.preprocessing.sequence import pad_sequences
from keras.utils import to_categorical

# 模拟数据
sentences = [["我", "爱", "你"], ["天气", "真", "好"]]
tags = [["r", "v", "r"], ["n", "d", "a"]]

# 构建词表和标签表
words = list(set(word for sent in sentences for word in sent))
tags_flat = list(set(tag for tag_seq in tags for tag in tag_seq))

word2idx = {w: i + 2 for i, w in enumerate(words)}
word2idx["PAD"] = 0
word2idx["UNK"] = 1

tag2idx = {t: i for i, t in enumerate(tags_flat)}
idx2tag = {i: t for t, i in tag2idx.items()}

max_len = 10

X = [[word2idx.get(w, 1) for w in s] for s in sentences]
X = pad_sequences(X, maxlen=max_len, padding="post")

y = [[tag2idx[t] for t in ts] for ts in tags]
y = pad_sequences(y, maxlen=max_len, padding="post")
y = [to_categorical(i, num_classes=len(tag2idx)) for i in y]


from keras.models import Model
from keras.layers import Input, Embedding, Bidirectional, LSTM, TimeDistributed, Dense
from keras_crf import CRF

input = Input(shape=(max_len,))
model = Embedding(input_dim=len(word2idx), output_dim=64, input_length=max_len)(input)
model = Bidirectional(LSTM(units=64, return_sequences=True))(model)
model = TimeDistributed(Dense(64, activation="relu"))(model)
crf = CRF(len(tag2idx))
out = crf(model)

model = Model(input, out)
model.compile(optimizer="adam", loss=crf.loss_function, metrics=[crf.accuracy])
model.summary()

X_train, X_test, y_train, y_test = train_test_split(X, np.array(y), test_size=0.1)

model.fit(X_train, y_train, batch_size=32, epochs=5, validation_split=0.1, verbose=1)

test_sent = ["天气", "真", "好"]
test_seq = pad_sequences([[word2idx.get(w, 1) for w in test_sent]], maxlen=max_len, padding="post")
pred = model.predict(test_seq)
pred_tags = [idx2tag[np.argmax(p)] for p in pred[0]]
print(list(zip(test_sent, pred_tags[:len(test_sent)])))
