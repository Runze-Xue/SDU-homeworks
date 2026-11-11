def preprocess_to_tsv(original_file_path, processed_file_path):
    print(f"Preprocessing {original_file_path} into {processed_file_path}...")
    with open(original_file_path, 'r', encoding='utf-8') as infile, \
         open(processed_file_path, 'w', encoding='utf-8') as outfile:
        lines = infile.readlines()
        i = 0
        pairs_written = 0
        while i + 1 < len(lines): # Ensure there's a pair of lines
            eng_sentence = lines[i].strip()
            chn_sentence = lines[i+1].strip()

            if eng_sentence and chn_sentence: # Make sure neither is empty after stripping
                outfile.write(f"{eng_sentence}\t{chn_sentence}\n")
                pairs_written +=1
            elif eng_sentence and not chn_sentence:
                print(f"  Warning: English sentence found without a Chinese translation at original lines {i+1}-{i+2}. Skipping: '{eng_sentence[:50]}...'")
            elif not eng_sentence and chn_sentence:
                 print(f"  Warning: Chinese translation found without an English sentence at original lines {i+1}-{i+2}. Skipping: '{chn_sentence[:50]}...'")
            # else: both empty, skip silently
            i += 2

        if i < len(lines) and lines[i].strip(): # Handle a possible last odd line
            print(f"  Warning: The last line in {original_file_path} ('{lines[i].strip()[:50]}...') does not have a pair and was ignored.")
        print(f"Preprocessing finished. {pairs_written} pairs written to {processed_file_path}.")
    return pairs_written > 0 # Return True if any data was written