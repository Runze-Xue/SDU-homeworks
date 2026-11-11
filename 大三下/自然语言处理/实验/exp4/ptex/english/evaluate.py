from seqeval.metrics import classification_report

model.eval()
all_preds, all_labels = [], []

with torch.no_grad():
    for batch in valid_loader:
        input_ids = batch["input_ids"].to(device)
        attention_mask = batch["attention_mask"].to(device)
        labels = batch["labels"].to(device)

        preds = model(input_ids, attention_mask)
        for i, pred in enumerate(preds):
            true_labels = labels[i].tolist()
            pred_tags = [id2tag[p] for p, l in zip(pred, true_labels) if l != -100]
            true_tags = [id2tag[l] for l in true_labels if l != -100]
            all_preds.append(pred_tags)
            all_labels.append(true_tags)

print(classification_report(all_labels, all_preds))
