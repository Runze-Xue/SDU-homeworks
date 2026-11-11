# utils/data_loader.py

# from transformers import BertTokenizerFast # BertTokenizerFast is loaded in main.py

def tokenize_and_align_labels(examples, tokenizer, max_length, label_all_tokens=False):
    tokenized_inputs = tokenizer(
        examples["tokens"],
        truncation=True,
        is_split_into_words=True, # Crucial for CoNLL-2003 format
        max_length=max_length,
        padding=False,  # Padding will be handled by DataCollator
        # return_offsets_mapping=False, # Explicitly set if you want to ensure it's off,
                                      # though default might be False for some tokenizers/versions
                                      # The original error suggested it was present.
    )

    aligned_labels = []
    for i, ner_tag_sequence in enumerate(examples["ner_tags"]):
        word_ids = tokenized_inputs.word_ids(batch_index=i)
        previous_word_idx = None
        label_ids_for_sequence = []
        for word_idx in word_ids:
            if word_idx is None:  # Special tokens ([CLS], [SEP], PAD)
                label_ids_for_sequence.append(-100)
            elif word_idx != previous_word_idx:  # First token of a new word
                label_ids_for_sequence.append(ner_tag_sequence[word_idx])
            else:  # Subsequent tokens of the same word
                label_ids_for_sequence.append(ner_tag_sequence[word_idx] if label_all_tokens else -100)
            previous_word_idx = word_idx
        aligned_labels.append(label_ids_for_sequence)

    tokenized_inputs["labels"] = aligned_labels
    return tokenized_inputs