import nltk
import spacy
import stanza 
proxies = {
    "http": "http://127.0.0.1:7890",
    "https": "http://127.0.0.1:7890"
}

def english_pos_tagging(file_path):
    # NLTK实现
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read()
    tokens = nltk.word_tokenize(text)
    nltk_tags = nltk.pos_tag(tokens)
    print("NLTK标注结果:", nltk_tags)  

    # SpaCy实现
    nlp = spacy.load("en_core_web_sm")
    doc = nlp(text)
    spacy_tags = [(token.text, token.pos_) for token in doc]
    print("\nSpaCy标注结果:", spacy_tags)  

    # Stanza实现
    nlp = stanza.Pipeline(lang="en", processors="tokenize,pos",  download_method=None, proxies=proxies)  # 仅启用分词和词性标注
    doc = nlp(text)
    stanford_tags = [
        (word.text, word.upos) 
        for sentence in doc.sentences 
        for word in sentence.words
    ]
    print("\nStanza标注结果:", stanford_tags)

english_pos_tagging("English.txt")