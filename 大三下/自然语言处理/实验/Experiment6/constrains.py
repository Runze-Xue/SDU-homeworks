N_EPOCHS = 10 
CLIP = 1 
best_valid_loss = float('inf')

device = 'cuda'
BATCH_SIZE = 4 
SEED = 1234


ENC_EMB_DIM = 128  
DEC_EMB_DIM = 128
ENC_HID_DIM = 256
DEC_HID_DIM = 256
ENC_DROPOUT = 0.5
DEC_DROPOUT = 0.5

ORIGINAL_TRAIN_FILE = 'train.txt'
ORIGINAL_VALID_FILE = 'valid.txt'
ORIGINAL_TEST_FILE = 'test.txt'

PROCESSED_TRAIN_FILE = 'train.tsv'
PROCESSED_VALID_FILE = 'valid.tsv'
PROCESSED_TEST_FILE = 'test.tsv'