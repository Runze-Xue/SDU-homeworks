import torch
import torch.nn as nn
import torch.nn.functional as F # 确保导入
import math
import random # 确保导入

class Encoder(nn.Module):
    def __init__(self, input_dim, emb_dim, enc_hid_dim, dec_hid_dim, dropout_p): 
        super().__init__()
        self.embedding = nn.Embedding(input_dim, emb_dim)
        self.rnn = nn.GRU(emb_dim, enc_hid_dim, bidirectional=True, batch_first=True)
        self.fc = nn.Linear(enc_hid_dim * 2, dec_hid_dim)
        self.dropout = nn.Dropout(dropout_p)

    def forward(self, src):
        embedded = self.dropout(self.embedding(src))
        outputs, hidden = self.rnn(embedded)
        hidden_concat = torch.cat((hidden[-2, :, :], hidden[-1, :, :]), dim=1)
        hidden_transformed = torch.tanh(self.fc(hidden_concat))
        return outputs, hidden_transformed

class Attention(nn.Module):
    def __init__(self, enc_hid_dim, dec_hid_dim):
        super().__init__()
        self.attn = nn.Linear((enc_hid_dim * 2) + dec_hid_dim, dec_hid_dim)
        self.v = nn.Linear(dec_hid_dim, 1, bias=False)

    def forward(self, decoder_hidden, encoder_outputs, src_mask=None):
        batch_size = encoder_outputs.shape[0]
        src_len = encoder_outputs.shape[1]
        decoder_hidden_expanded = decoder_hidden.unsqueeze(1).repeat(1, src_len, 1)
        energy_input = torch.cat((decoder_hidden_expanded, encoder_outputs), dim=2)
        energy = torch.tanh(self.attn(energy_input))
        attention_scores = self.v(energy).squeeze(2)
        if src_mask is not None:
            attention_scores = attention_scores.masked_fill(src_mask == 0, -1e10)
        return F.softmax(attention_scores, dim=1)

class Decoder(nn.Module):
    def __init__(self, output_dim, emb_dim, enc_hid_dim, dec_hid_dim, dropout_p, attention_module): 
        super().__init__()
        self.output_dim = output_dim
        self.attention = attention_module 
        self.embedding = nn.Embedding(output_dim, emb_dim)
        self.rnn = nn.GRU((enc_hid_dim * 2) + emb_dim, dec_hid_dim, batch_first=True) 
        self.fc_out = nn.Linear((enc_hid_dim * 2) + dec_hid_dim + emb_dim, output_dim)
        self.dropout = nn.Dropout(dropout_p)

    def forward(self, trg_input, decoder_hidden, encoder_outputs, src_mask=None):
        trg_input_unsqueezed = trg_input.unsqueeze(1)
        embedded = self.dropout(self.embedding(trg_input_unsqueezed))
        attention_weights = self.attention(decoder_hidden, encoder_outputs, src_mask).unsqueeze(1)
        weighted_encoder_output = torch.bmm(attention_weights, encoder_outputs)
        rnn_input = torch.cat((embedded, weighted_encoder_output), dim=2)
        decoder_hidden_rnn_input = decoder_hidden.unsqueeze(0)
        rnn_output, new_decoder_hidden = self.rnn(rnn_input, decoder_hidden_rnn_input)
        embedded_squeezed = embedded.squeeze(1)
        weighted_encoder_output_squeezed = weighted_encoder_output.squeeze(1)
        rnn_output_squeezed = rnn_output.squeeze(1)
        fc_input = torch.cat((rnn_output_squeezed, weighted_encoder_output_squeezed, embedded_squeezed), dim=1)
        prediction = self.fc_out(fc_input)
        return prediction, new_decoder_hidden.squeeze(0)

class Seq2Seq(nn.Module):
    def __init__(self, encoder, decoder, src_pad_idx, device):
        super().__init__()
        self.encoder = encoder
        self.decoder = decoder
        self.src_pad_idx = src_pad_idx
        self.device = device

    def create_mask(self, src):
        # src: [batch_size, src_len]
        # mask: [batch_size, src_len] (True for non-pad, False for pad)
        mask = (src != self.src_pad_idx)
        return mask

    def forward(self, src, trg, teacher_forcing_ratio=0.5):
        # src: [batch_size, src_len]
        # trg: [batch_size, trg_len]

        batch_size = trg.shape[0]
        trg_len = trg.shape[1] # 目标序列的最大长度
        trg_vocab_size = self.decoder.output_dim

        # 张量存储所有时间步的解码器输出
        # outputs: [batch_size, trg_len, trg_vocab_size]
        outputs = torch.zeros(batch_size, trg_len, trg_vocab_size).to(self.device)

        # 创建源序列掩码
        src_mask = self.create_mask(src) # src_mask: [batch_size, src_len]

        # 通过编码器
        # encoder_outputs: [batch_size, src_len, enc_hid_dim * 2]
        # decoder_hidden: [batch_size, dec_hid_dim] (解码器的初始隐藏状态)
        encoder_outputs, decoder_hidden = self.encoder(src)

        # 解码器的第一个输入是 <sos> 标记 (通常是 trg 的第一个词元)
        # current_trg_input: [batch_size]
        current_trg_input = trg[:, 0]

        # 循环生成目标序列的其余部分
        # 注意：循环从 1 开始，因为 outputs[0] 对应 <sos> 输入，其输出我们不直接用作预测
        # 或者说，我们预测的是 trg[1] 到 trg[trg_len-1]
        for t in range(1, trg_len):
            # decoder_output: [batch_size, output_dim]
            # decoder_hidden: [batch_size, dec_hid_dim] (更新后的隐藏状态)
            decoder_output, decoder_hidden = self.decoder(current_trg_input, decoder_hidden, encoder_outputs, src_mask)

            # 存储当前时间步的预测
            outputs[:, t] = decoder_output # 也可以写作 outputs[:, t, :]

            # 决定是否使用 teacher forcing
            use_teacher_force = random.random() < teacher_forcing_ratio

            if use_teacher_force:
                # Teacher forcing: 使用真实的下一个目标词作为输入
                current_trg_input = trg[:, t]
            else:
                # Non-teacher forcing: 使用模型自己的预测作为下一个输入
                # top1_prediction: [batch_size] (预测概率最高的词的索引)
                top1_prediction = decoder_output.argmax(1)
                current_trg_input = top1_prediction
        
        # outputs 的第一个时间步 (outputs[:, 0, :]) 仍然是零，
        # 因为我们从 trg[:,0] 开始作为输入，预测的是从 trg[:,1] 开始的词。
        # 在计算损失时，通常比较 outputs[:, 1:] 和 trg[:, 1:]。
        return outputs