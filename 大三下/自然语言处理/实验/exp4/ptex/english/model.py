# model/model.py

import torch
import torch.nn as nn
from transformers import BertModel
from TorchCRF import CRF # Assuming using 'pytorch-crf' library

class BERT_BiLSTM_CRF(nn.Module):
    def __init__(self, bert_model_name, hidden_dim, tag2id, dropout_prob=0.1):
        super().__init__()
        self.bert = BertModel.from_pretrained(bert_model_name)
        self.lstm = nn.LSTM(
            input_size=self.bert.config.hidden_size,
            hidden_size=hidden_dim,
            num_layers=1,
            batch_first=True,
            bidirectional=True
        )
        self.dropout = nn.Dropout(dropout_prob)
        self.classifier = nn.Linear(hidden_dim * 2, len(tag2id))
        self.crf = CRF(num_tags=len(tag2id), batch_first=True)
        # It's good practice to store num_tags if you need it elsewhere or for clarity
        self.num_tags = len(tag2id)
        # Find the ID for the 'O' tag, default to 0 if not found (though 'O' should always be present)
        self.o_tag_id = tag2id.get('O', 0)


    def forward(self, input_ids, attention_mask, labels=None):
        bert_outputs = self.bert(input_ids=input_ids, attention_mask=attention_mask)
        sequence_output = bert_outputs.last_hidden_state
        sequence_output = self.dropout(sequence_output)
        lstm_output, _ = self.lstm(sequence_output)
        lstm_output_dropped = self.dropout(lstm_output)
        emissions = self.classifier(lstm_output_dropped)

        crf_mask = attention_mask.bool()

        if labels is not None:
            # --- Modification Starts Here ---
            # Create a copy of labels to modify for CRF input
            labels_for_crf = labels.clone()

            # Replace -100 with a valid tag_id (e.g., 'O' tag's ID) ONLY where crf_mask is True.
            # CRF will use the crf_mask to ignore positions, but the label values themselves
            # at active positions must be valid indices.
            # We only care about replacing -100 where crf_mask is True, because if crf_mask is False,
            # the CRF layer will ignore that position anyway.
            # However, the error occurs when accessing transitions using a -100 index.
            # So, any label that *could* be used as an index by CRF (i.e., where mask is True)
            # must be valid.
            labels_for_crf[labels == -100] = self.o_tag_id # Replace all -100s with 'O' tag ID
            # The crf_mask will then ensure that loss is only computed for actual tokens.
            # --- Modification Ends Here ---

            loss = -self.crf(emissions, labels_for_crf, mask=crf_mask, reduction='mean')
            
            with torch.no_grad():
                # For decoding, the mask should still be crf_mask (attention_mask.bool())
                # The emissions are for all tokens indicated by attention_mask.
                predictions = self.crf.decode(emissions, mask=crf_mask)
            return loss, predictions
        else:
            predictions = self.crf.decode(emissions, mask=crf_mask)
            return predictions