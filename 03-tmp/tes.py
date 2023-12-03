#!/usr/bin/env python
# coding=utf-8
import numpy as np
def compute_position_features(max_seq_len, sequence_lengths, dtype=np.float16):
    """Compute position features for sequences of lengths `sequence_lengths`, given the maximum sequence length
    `max_seq_len`.
    """
    sequences = np.zeros((max_seq_len+1, max_seq_len, 3), dtype=dtype)
    half_sequence_lengths = np.asarray(np.ceil(sequence_lengths / 2.), dtype=np.int)    # [0, 1, 1, 2, 2, ..]
    for i in range(len(sequences)):
        sequence, seq_len, half_seq_len = sequences[i], sequence_lengths[i], half_sequence_lengths[i]
        sequence[:seq_len, -1] = np.abs(0.5 - np.linspace(1.0, 0, num=seq_len)) * 2.    
        sequence[:half_seq_len, -3] = sequence[:half_seq_len, -1]
        sequence[half_seq_len:seq_len, -2] = sequence[half_seq_len:seq_len, -1]
        sequence[:seq_len, -1] = 1. - sequence[:seq_len, -1]
    return sequences

slen = 4
a = compute_position_features(slen, np.arange(slen+1))
print(a)
