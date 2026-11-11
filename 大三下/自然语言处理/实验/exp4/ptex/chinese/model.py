import torch
import torch.nn as nn
from transformers import BertModel
from TorchCRF import CRF

class BertBiLSTMCRF(nn.Module):
    def __init__(self, bert_model_name, num_tags, lstm_hidden_dim, lstm_layers, dropout_rate):
        super(BertBiLSTMCRF, self).__init__()
        
        self.bert = BertModel.from_pretrained(bert_model_name)
        self.bert_hidden_dim = self.bert.config.hidden_size
        
        self.lstm = nn.LSTM(
            input_size=self.bert_hidden_dim,
            hidden_size=lstm_hidden_dim,
            num_layers=lstm_layers,
            bidirectional=True,
            batch_first=True 
        )
        
        self.hidden2tag = nn.Linear(lstm_hidden_dim * 2, num_tags)
        
        self.crf = CRF(num_tags, batch_first=True)
        
        self.dropout = nn.Dropout(dropout_rate)

    def _get_lstm_features(self, input_ids, attention_mask):
        bert_outputs = self.bert(input_ids=input_ids, attention_mask=attention_mask)
        sequence_output = bert_outputs.last_hidden_state 
        
        sequence_output = self.dropout(sequence_output)

        lstm_out, (hidden, cell) = self.lstm(sequence_output)
        
        lstm_out = self.dropout(lstm_out)
        
        emissions = self.hidden2tag(lstm_out) 
        return emissions

    def forward(self, input_ids, attention_mask, labels=None):
        """
        计算损失或进行解码
        input_ids: (batch_size, seq_length)
        attention_mask: (batch_size, seq_length), 0 for padding, 1 for real tokens
        labels: (batch_size, seq_length), 标签ID序列
        """
        emissions = self._get_lstm_features(input_ids, attention_mask)
        
        if labels is not None:
            mask = attention_mask.bool()
            loss = -self.crf(emissions, labels, mask=mask, reduction='mean') 
            return loss
        else:
            mask = attention_mask.bool()
            predictions = self.crf.decode(emissions, mask=mask)
            return predictions 