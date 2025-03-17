# Experiment 2 Report

<center>于斐, 202200130119. Sep 29, 2024</center>


## Description

The following report shows the results of *multivariate linear regression* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

## Supervised Learning Problem

Initialize libraries and read data from `ex2x.dat` and `ex2y.dat`.

```python
import numpy as np
import torch
from matplotlib import pyplot as plt
x = np.loadtxt('ex2x.dat')
y = np.loadtxt('ex2y.dat')
```

Before starting gradient descent, add $x_0 = 1$ intercept term to every example.

```python
m = len(y)
x = np.c_[np.ones(m), x]
```

and scale both types of inputs by their standard deviations and set their means to zero.

```python
sigma = np.std(x[:, 1:], axis=0)
mu = np.mean(x[:, 1:], axis=0)
x[:, 1:] = (x[:, 1:] - mu) / sigma
```

## Gradient Descent

### Gradient Descent Implementation

With respect to the hypothesis function and its gradient, an optimizer is implemented for this task.

Like experiment 1, the  hypothesis function can be represented as 

$$h_\theta(x) = \theta^T x$$ 

and the batch gradient descent update can be represented as 

$$\theta_j := \theta_j - \alpha \cfrac 1m \sum \limits_{i = 1}^{m} \left( h_\theta (x^{(i)}) - y^{(i)} \right) x_j^{(i)}.$$

Implementation in python:

```python
n = x.shape[1]
theta = np.zeros(n)

alpha = 0.07

class Optimizer:
    def __init__(self, x: np.ndarray, y: np.ndarray, theta: np.ndarray, alpha: float):
        self.x = x
        self.y = y
        self.theta = theta
        self.alpha = alpha

    def forward(self):
        return np.dot(self.x, self.theta)

    def gradient(self):
        h = self.forward()
        return np.array([np.sum((h - y) * x[:, i]) / m for i in range(n)])

    def step(self):
        self.theta -= self.alpha * self.gradient()
```

### Selecting A Learning Rate Using $J(\theta)$

I tested `alpha = [0.001, 0.003, 0.01, 0.03, 0.1, 0.3, 1]`. For each learning rate, its costs with iterations are shown in the following figure.

![image](https://img.picgo.net/2024/09/29/image25eff1925ce8267d.png)

It seems that $\alpha = 0.1$ is a good choice.

Implementation in Python:

```python
alphas = [0.001, 0.003, 0.01, 0.03, 0.1, 0.3, 1]

for alpha in alphas:
    x_ = x.copy()
    y_ = y.copy()
    theta_ = theta.copy()
    optimizer = Optimizer(x_, y_, theta_, alpha)

    iterations = 50
    J = np.zeros(iterations)

    for i in range(iterations):
        J[i] = np.sum((optimizer.forward() - y) ** 2) / (2 * m)
        optimizer.step()

    plt.plot(J)
plt.legend([f'alpha={alpha}' for alpha in alphas])
plt.xlabel('Iterations')
plt.ylabel('Cost')
plt.title('Different learning rates')
plt.show()
```

### Observe the changes in the cost function happens as the learning rate changes. What happens when the learning rate is too small? Too large?

When the learning rate is too small, the rate of convergence will be too small.

When the learning rate is too large (like setting $\alpha = 10$), the value of $J(\theta)$ will increase or even blow up.

### Using the best learning rate that you found, run gradient descent until convergence.

I used the best learning rate $\alpha = 0.1$ in experiment. The final $\theta = [340412.65957447, 109447.79646964,  -6578.35485416]$.

Implementation in Python:

```python
alpha = 0.1
x_ = x.copy()
y_ = y.copy()
theta_ = theta.copy()
optimizer = Optimizer(x_, y_, theta_, alpha)

iterations = 1500
J = np.zeros(iterations)

for i in range(iterations):
    J[i] = np.sum((optimizer.forward() - y) ** 2) / (2 * m)
    optimizer.step()

plt.plot(J)
theta_
```

The predicted price of a house with 1650 square feet and 3 bedrooms will be $293081.4643349$.

```python
predict_x = np.array([[1650, 3]])
predict_x = (predict_x - mu) / sigma
predict_x = np.c_[np.ones(1), predict_x]
predict_y = np.dot(predict_x, theta_)
print(predict_y)
```

## Normal Equation

I used 

$$
\theta = (X^T X)^{-1} X^T \mathbf{y}
$$

to calculate $\theta$, and the result is $\theta = [89597.9095428, 139.21067402, -8738.01911233]$.

And the predicted result can be regarded as the same.

$$y' = 293081.46433489$$

Implementation in Python:

```python
x = np.loadtxt('ex2x.dat')
y = np.loadtxt('ex2y.dat')
x = np.c_[np.ones(m), x]
theta = np.dot(np.dot(np.linalg.inv(np.dot(x.T, x)), x.T), y)
print(theta)

predict_x = np.array([[1650, 3]])
predict_x = np.c_[np.ones(1), predict_x]
predict_y = np.dot(predict_x, theta)
print(predict_y)
```