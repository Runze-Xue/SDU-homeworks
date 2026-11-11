from sklearn.preprocessing import OneHotEncoder
import numpy as np

def onehot_encode(x):
    x = np.array(x).reshape(-1, 1)
    encoder = OneHotEncoder()
    onehot_sparse = encoder.fit_transform(x)
    onehot = onehot_sparse.toarray() 
    for word, vec in zip(x, onehot):
        print(f"{word[0]}: {vec.tolist()}")

x = ['体育', '军事', '娱乐', '教育', '文化', '时尚', '科技', '财经']
onehot_encode(x)