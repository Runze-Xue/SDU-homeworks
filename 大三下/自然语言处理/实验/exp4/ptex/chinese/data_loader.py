import torch
from torch.utils.data import Dataset, DataLoader
from transformers import BertTokenizerFast

def load_data(file_path):
    sentences = []
    with open(file_path, 'r', encoding='utf-8') as f:
        tokens, tags = [], []
        for line in f:
            line = line.strip()
            if not line: 
                if tokens:
                    sentences.append((tokens, tags))
                    tokens, tags = [], []
            else:
                parts = line.split()
                if len(parts) == 2:
                    tokens.append(parts[0])
                    tags.append(parts[1])
                elif len(parts) == 1: # 有时只有词没有标签，作为 'O' 处理
                    tokens.append(parts[0])
                    tags.append('O')
        if tokens: # 处理文件末尾没有空行的情况
            sentences.append((tokens, tags))
    return sentences

def get_tags(sentences):
    all_tags = set()
    for _, tags in sentences:
        for tag in tags:
            all_tags.add(tag)
    
    tag_list = sorted(list(all_tags))
    if "O" not in tag_list: 
        tag_list.append("O")

    tag_to_idx = {tag: idx for idx, tag in enumerate(tag_list)}
    idx_to_tag = {idx: tag for tag, idx in tag_to_idx.items()}
    
    PAD_TAG_ID = tag_to_idx.get("O") 

    return tag_to_idx, idx_to_tag, PAD_TAG_ID


class NerDataset(Dataset):
    def __init__(self, sentences, tokenizer, tag_to_idx, max_len, pad_tag_id):
        self.sentences = sentences
        self.tokenizer = tokenizer
        self.tag_to_idx = tag_to_idx
        self.max_len = max_len
        self.pad_tag_id = pad_tag_id

    def __len__(self):
        return len(self.sentences)

    def __getitem__(self, idx):
        tokens, tags = self.sentences[idx]

        tokenized_inputs = self.tokenizer(
            tokens,
            is_split_into_words=True,
            return_offsets_mapping=True,
            padding='max_length',
            truncation=True,
            max_length=self.max_len,
            return_tensors="pt"
        )
        
        word_ids = tokenized_inputs.word_ids(batch_index=0)
        
        previous_word_idx = None
        label_ids = []
        for word_idx in word_ids:
            if word_idx is None: 
                label_ids.append(self.pad_tag_id) 
            elif word_idx != previous_word_idx:
                label_ids.append(self.tag_to_idx[tags[word_idx]])
            else: 
                label_ids.append(self.pad_tag_id) 
            previous_word_idx = word_idx
        
        input_ids = tokenized_inputs["input_ids"].squeeze()
        attention_mask = tokenized_inputs["attention_mask"].squeeze()
        
        return {
            "input_ids": input_ids,
            "attention_mask": attention_mask,
            "labels": torch.tensor(label_ids, dtype=torch.long)
        }

if __name__ == '__main__':
    BERT_MODEL_NAME = 'bert-base-chinese'
    MAX_LEN = 128
    BATCH_SIZE = 2

    tokenizer = BertTokenizerFast.from_pretrained(BERT_MODEL_NAME)
    
    train_sentences = load_data('train.txt')
    dev_sentences = load_data('dev.txt')
    
    tag_to_idx, idx_to_tag, PAD_TAG_ID = get_tags(train_sentences + dev_sentences)
    print("Tag to ID mapping:", tag_to_idx)
    print("Pad Tag ID:", PAD_TAG_ID)
    print(f"Number of tags: {len(tag_to_idx)}")

    train_dataset = NerDataset(train_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)
    dev_dataset = NerDataset(dev_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)

    train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
    dev_dataloader = DataLoader(dev_dataset, batch_size=BATCH_SIZE)

    # 检查一个batch
    for batch in train_dataloader:
        print("Input IDs shape:", batch['input_ids'].shape)
        print("Attention Mask shape:", batch['attention_mask'].shape)
        print("Labels shape:", batch['labels'].shape)
        
        print("\n--- Sample 0 ---")
        sample_input_ids = batch['input_ids'][0]
        sample_labels = batch['labels'][0]
        sample_tokens = tokenizer.convert_ids_to_tokens(sample_input_ids)
        
        for token, label_id in zip(sample_tokens, sample_labels):
            if token != tokenizer.pad_token: # 只打印非padding部分
                print(f"{token:<15} {idx_to_tag.get(label_id.item(), 'PAD/X')}")
        break