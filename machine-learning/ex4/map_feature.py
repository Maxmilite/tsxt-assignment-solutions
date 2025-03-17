import numpy as np

def map_feature(feat1, feat2):
    """
    Feature mapping function to polynomial terms.
    
    Parameters:
    feat1 (numpy array): First feature column vector
    feat2 (numpy array): Second feature column vector
    
    Returns:
    numpy array: A new feature array with polynomial features.
    
    Note: This is specific to the hard-coded degree (6).
    """
    degree = 6
    if feat1.shape != feat2.shape:
        raise ValueError("feat1 and feat2 must be the same size")
    
    out = np.ones((feat1.shape[0], 1))
    
    for i in range(1, degree + 1):
        for j in range(i + 1):
            feature = (feat1 ** (i - j)) * (feat2 ** j)
            out = np.hstack((out, feature.reshape(-1, 1)))
    
    return out
