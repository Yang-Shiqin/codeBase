#!/usr/bin/env python
# coding=utf-8
import torch
def __compute_features__(sequence_char_indices, sequence_lengths, max_mb_seq_len, counts_per_sequence):
    """Compute one-hot sequence features + position features with shape (n_sequences, sequence_length, n_features)
    from sequence indices
    """
    n_features = 8 
        # Send indices to device
    sequence_char_indices = sequence_char_indices.to(dtype=torch.long)
    sequence_lengths = sequence_lengths.to(dtype=torch.long)
        # Only send sequence counts to device, if using sequence counts
        # Allocate tensor for one-hot sequence features + position features
    features_one_hot_shape = (sequence_char_indices.shape[0], max_mb_seq_len, n_features)
    features_one_hot_padded = torch.zeros(size=features_one_hot_shape)
    print('pad, ', features_one_hot_padded.shape)
        # Set one-hot sequence features
    features_one_hot = features_one_hot_padded[:, :sequence_char_indices.shape[1]]
    features_one_hot = features_one_hot.reshape((-1, n_features))
    features_one_hot[torch.arange(features_one_hot.shape[0]), sequence_char_indices.reshape((-1))] = 1.
    # Set padded sequence-parts to 0 (sequence_char_indices == -1 marks the padded positions)
    features_one_hot[sequence_char_indices.reshape((-1)) == -1, -1] = 0.
    features_one_hot = features_one_hot.reshape((sequence_char_indices.shape[0], sequence_char_indices.shape[1],
                                                n_features))
    print('asd, ', features_one_hot.shape)
    features_one_hot_padded[:, :sequence_char_indices.shape[1], :] = features_one_hot
    # Scale by sequence counts
    features_one_hot_padded = features_one_hot_padded * counts_per_sequence[:, None, None]
        # Add position information
        # Perform normalization to std=1
    features_one_hot_padded = features_one_hot_padded / features_one_hot_padded.std()
    return features_one_hot_padded

sequence_char_indices=[
    torch.tensor([[1,2,4,-1],[2,5,3,1],[0,-1,-1,-1],[2,3,5,6],[2,3,-1,-1]]),
    torch.tensor([[1,4,-1],[3,5,6],[3,-1,-1]]),
    torch.tensor([[1,2,4,-1,-1],[2,5,3,1,7],[2,3,5,6,-1],[2,3,-1,-1,-1]]),
]
sequence_lengths=[
    torch.tensor([3,4,1,4,2]),
    torch.tensor([2,3,1]),
    torch.tensor([3,5,4,2]),
] 
max_mb_seq_len=max(t.max() for t in sequence_lengths)
counts_per_sequences=[
    torch.tensor([1,2,1,2,2]),
    torch.tensor([2,1,1]),
    torch.tensor([1,1,2,2]),
] 
inputs_list = [__compute_features__(sequence_of_indices, sequence_lengths, max_mb_seq_len, counts_per_sequence)
                           for sequence_of_indices, sequence_lengths, counts_per_sequence
                           in zip(sequence_char_indices, sequence_lengths, counts_per_sequences)]
print('inputs, ', sequence_char_indices)
print(inputs_list)
