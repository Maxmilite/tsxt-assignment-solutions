# Experiment 4 Report

<center>于斐, 202200130119. Oct. 18, 2024</center>


## Description

The following report shows the results of *Regularization* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

The helper function `map_feature.m` is reimplemented in Python.

```python
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
```

## Regularized Linear Regression

Initialize libraries and read data from `ex5Linx.dat` and `ex5Liny.dat`.

```python
import numpy as np
import torch
from matplotlib import pyplot as plt
x = np.loadtxt('ex5Linx.dat')
y = np.loadtxt('ex5Liny.dat')
```

![image](https://img.picgo.net/2024/10/18/image09090a8da60cae0e.png)

In the task, a fifth-order polynomial is expected to be fit.

$$
h_\theta (x) = \theta_0 + \theta_1 x + \theta_2 x^2 + \theta_3 x^3 + \theta_4 x^4 + \theta_5 x^5
$$

where 6 features $\theta_0, \cdots, \theta_5$ are to be determined.

Regularization is to be used in the model to prevent over-fitting.

### Find Values for $\theta$ With 3 Regulariation Parameters

For $\lambda = 0, 1, 10$, we figured out the best $\theta$ with the following normal equation:

$$
\theta=(X^TX+\lambda\begin{bmatrix}0&&&\\&1&&\\&&\ddots&\\&&&1\end{bmatrix})^{-1}X^T\vec{y}
$$

```plain
0 tensor([ 0.4726,  0.6814, -1.3803, -5.9780,  2.4419,  4.7374])
1 tensor([ 0.3242, -0.4204,  0.1891, -0.3953,  0.1972, -0.3359])
10 tensor([ 0.1990, -0.1909,  0.1250, -0.1588,  0.1143, -0.1400])
```

![image](https://img.picgo.net/2024/10/18/imageb8dae93ee2c00986.png)

As lambda increases, the model becomes less overfit to the training data. This is because the regularization term penalizes large coefficients, which in turn reduces the model's complexity. This is evident in the lambda = 10 graph, where the model is less overfit to the training data compared to the lambda = 0 graph.

### Regularized Logistic Regression

The figure is shown as follows:

![image](https://img.picgo.net/2024/10/18/image8ed3d61c63481253.png)

```python
x = np.loadtxt('ex5Logx.dat')
y = np.loadtxt('ex5Logy.dat')

pos = np.where(y > 0)
neg = np.where(y == 0)

plt.plot(x[pos, 0], x[pos, 1], 'b+')
plt.plot(x[neg, 0], x[neg, 1], 'yo')
plt.show()
```

Now features are extracted using the following code:

```python
u, v = x[:, 0], x[:, 1]

from map_feature import map_feature
X = map_feature(u, v)
```

Like Experiment 3, with respect to the hypothesis function and its gradient, an optimizer is implemented for this task.

The hypothesis function can be represented as 

$$
h_\theta(x) = 1 / (1 + \exp(-\theta^T x))
$$ 

and the batch gradient descent update can be represented as 

$$
\theta := \theta - H^{-1} \nabla_\theta J.
$$

```python
def sigmoid(z):
    return 1 / (1 + np.exp(-z))

def cost(theta, x, y):
    m = len(y)
    h = sigmoid(x @ theta)
    return -1 / m * (y.T @ np.log(h) + (1 - y).T @ np.log(1 - h))

class Optimizer:
    def __init__(self, x: np.ndarray, y: np.ndarray, theta: np.ndarray):
        self.x = x
        self.y = y
        self.theta = theta
    
    def forward(self, lam: float):
        m = len(self.y)
        h = sigmoid(self.x @ self.theta)
        return (
            -1 / m * (self.y.T @ np.log(h) + (1 - self.y).T @ np.log(1 - h))
        ) + (lam / (2 * m) * self.theta[1:].T @ self.theta[1:])
    
    def step(self, lam: float): # using newton's method
        m = len(self.y)
        h = sigmoid(self.x @ self.theta)
        grad = 1 / m * self.x.T @ (h - self.y) \
            + lam / m * np.concatenate([[0], self.theta[1:]])
        hessian = 1 / m * self.x.T @ np.diag(h) @ np.diag(1 - h) @ self.x \
            + lam / m * np.diag(np.concatenate([[0], np.ones(len(self.theta) - 1)]))
        self.theta -= np.linalg.inv(hessian) @ grad

fig, ax = plt.subplots(1, 3, figsize=(15, 5))

for index, lam in enumerate([0, 1, 10]):
    theta = np.zeros(X.shape[1])
    optimizer = Optimizer(X, y, theta)
    for i in range(100):
        optimizer.step(lam)

    u = np.linspace(-1, 1.5, 200)
    v = np.linspace(-1, 1.5, 200)
    z = np.zeros((len(u), len(v)))

    for i in range(len(u)):
        for j in range(len(v)):
            z[i, j] = map_feature(np.array([u[i]]), np.array([v[j]])).dot(theta)

    z = z.T
    ax[index].plot(x[pos, 0], x[pos, 1], 'b+')
    ax[index].plot(x[neg, 0], x[neg, 1], 'yo')
    ax[index].contour(u, v, z, 0)
    ax[index].set_title(f"lambda = {lam}")
    ax[index].legend(['y = 1', 'y = 0', 'Decision boundary'])
```

![image](https://img.picgo.net/2024/10/18/imagebe726bb531aef11a.png)