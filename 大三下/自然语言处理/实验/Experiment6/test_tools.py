def translate_sentence_new(sentence_str, src_tokenizer, src_vocab, trg_vocab, model, device, max_len=50):
    model.eval()
    if isinstance(sentence_str, str):
        tokens = src_tokenizer(sentence_str)
    else: # Assuming it's already a list of tokens
        tokens = [token.lower() for token in sentence_str] # Apply lower if not done by tokenizer
        tokens = tokens[::-1] # Reverse if it was part of original tokenize_en

    src_indexes = [SRC_SOS_IDX] + [src_vocab[token] for token in tokens] + [SRC_EOS_IDX]
    src_tensor = torch.LongTensor(src_indexes).unsqueeze(0).to(device) # Add batch dimension

    with torch.no_grad():
        encoder_outputs, hidden = model.encoder(src_tensor) # Assuming model.encoder exists

    # For decoder, start with <sos> token
    trg_indexes = [TRG_SOS_IDX]
    attentions = torch.zeros(max_len, 1, len(src_indexes)) # For storing attention weights

    for i in range(max_len):
        trg_tensor = torch.LongTensor([trg_indexes[-1]]).to(device) # Last predicted token

        with torch.no_grad():
            # Decoder might need hidden and cell from encoder, or just hidden
            # And encoder_outputs for attention
            # This depends on your exact model.decoder interface
            src_mask_translate = (src_tensor != SRC_PAD_IDX) # SRC_PAD_IDX 也需要可访问
            output, hidden = model.decoder(trg_tensor, hidden, encoder_outputs, src_mask_translate)
            # attentions[i] = attention_weights # Store attention

        pred_token_idx = output.argmax(1).item()
        trg_indexes.append(pred_token_idx)

        if pred_token_idx == TRG_EOS_IDX:
            break

    trg_tokens = [trg_vocab.get_itos()[i] for i in trg_indexes]
    # Remove <sos> and <eos> if present at ends
    return trg_tokens[1:-1] if trg_tokens[-1] == EOS_TOKEN else trg_tokens[1:]




from nltk.translate.bleu_score import corpus_bleu, sentence_bleu
from nltk.tokenize import word_tokenize # For consistency if needed, but our TRG field already tokenizes

def calculate_bleu(data, src_field, trg_field, model, device, max_len=50):
    trgs = []
    pred_trgs = []

    for datum in data:
        src = vars(datum)['src'] # Accessing list of tokens
        trg = vars(datum)['trg']

        # Translate sentence (src is already tokenized by Field)
        pred_trg = translate_sentence(src, src_field, trg_field, model, device, max_len)
        
        # pred_trg is a list of tokens, trg is also a list of tokens
        pred_trgs.append(pred_trg)
        trgs.append([trg]) # NLTK corpus_bleu expects a list of reference translations

    # For corpus BLEU, arguments are: list_of_references, hypotheses
    # list_of_references: [[[ref1_tok1, ref1_tok2], [ref2_tok1, ref2_tok2], ...]]
    # hypotheses: [[hyp1_tok1, hyp1_tok2], [hyp2_tok1, hyp2_tok2], ...]
    return corpus_bleu(trgs, pred_trgs)

# # --- 示例翻译和BLEU计算 ---
# # 加载训练好的模型 (或者用当前训练结束的模型)
# model.load_state_dict(torch.load('tut6-model.pt', map_location=device))

# # 翻译一个例子
# example_idx = 0
# if len(test_data.examples) > 0 :
#     src_example = vars(test_data.examples[example_idx])['src']
#     trg_example = vars(test_data.examples[example_idx])['trg']
    
#     print(f'src = {" ".join(src_example)}') # Remember src was reversed
#     print(f'trg = {" ".join(trg_example)}')
    
#     translation = translate_sentence(src_example, SRC, TRG, model, device)
#     print(f'predicted trg = {" ".join(translation)}')

#     # 计算测试集上的BLEU分数
#     # 注意：对于我们的玩具数据集，BLEU分数可能没有意义或很低
#     bleu_score = calculate_bleu(test_data, SRC, TRG, model, device)
#     print(f'BLEU score on test data = {bleu_score*100:.2f}')
# else:
#     print("Test data is empty. Cannot perform translation example or BLEU score calculation.")

# # 如果用 valid data 计算 BLEU
# if len(valid_data.examples) > 0:
#     bleu_score_valid = calculate_bleu(valid_data, SRC, TRG, model, device)
#     print(f'BLEU score on validation data = {bleu_score_valid*100:.2f}')