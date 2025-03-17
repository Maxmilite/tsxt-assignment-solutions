# Experiment 3 Report

<center>于斐, 202200130119. Oct 10, 2024</center>


## Description

The following report shows the results of *Logistic Regression and Newton’s Method* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

## Plot the Data

Initialize libraries and read data from `ex4x.dat` and `ex4y.dat`.

```python
import numpy as np
import torch
from matplotlib import pyplot as plt
x = np.loadtxt('ex4x.dat')
y = np.loadtxt('ex4y.dat')
```

Before starting gradient descent, add $x_0 = 1$ intercept term to every example.

```python
m = len(y)
x = np.c_[np.ones(m), x]
```

The plot is shown as follows:

![image](https://img.picgo.net/2024/10/15/image018946a40b550489.png)

Implementation in Python:

```python
pos = np.where(y == 1)
neg = np.where(y == 0)

x_pos = x[pos]
y_pos = y[pos]
x_neg = x[neg]
y_neg = y[neg]

plt.plot(x_pos[0, 1], x_pos[0, 2], 'b+', label='Admitted')
plt.plot(x_neg[0, 1], x_neg[0, 2], 'bo', label='Not admitted')
plt.legend(loc='upper right')
plt.plot(x_pos[1:, 1], x_pos[1:, 2], 'b+', label='Admitted')
plt.plot(x_neg[1:, 1], x_neg[1:, 2], 'bo', label='Not admitted')
plt.xlabel('Exam 1 score')
plt.ylabel('Exam 2 score')
plt.show()
```

## Newton's Method

With respect to the hypothesis function and its gradient, an optimizer is implemented for this task.

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
    def __init__(self, x: np.ndarray, y: np.ndarray, theta: np.ndarray,):
        self.x = x
        self.y = y
        self.theta = theta
    
    def forward(self):
        m = len(self.y)
        h = sigmoid(self.x @ self.theta)
        return -1 / m * (self.y.T @ np.log(h) + (1 - self.y).T @ np.log(1 - h))
    
    def step(self): # using newton's method
        m = len(self.y)
        h = sigmoid(self.x @ self.theta)
        grad = 1 / m * self.x.T @ (h - self.y)
        hessian = 1 / m * self.x.T @ np.diag(h) @ np.diag(1 - h) @ self.x
        self.theta -= np.linalg.inv(hessian) @ grad
```

When optimizing, after 5 iterations, it coverges.

```plain
Cost at iter 0: 0.6931471805599453
Cost at iter 1: 0.4409414251174191
Cost at iter 2: 0.40889169253305674
Cost at iter 3: 0.40551077605901753
Cost at iter 4: 0.4054474539166042
Cost at iter 5: 0.4054474249282527
Cost at iter 6: 0.40544742492824626
Cost at iter 7: 0.4054474249282463
Cost at iter 8: 0.40544742492824626
Cost at iter 9: 0.40544742492824626
Cost at iter 10: 0.40544742492824626
```

Implementation in Python:

```python
theta = np.zeros(x.shape[1])
optimizer = Optimizer(x, y, theta)

for i in range(10 + 1):
    print(f"Cost at iter {i}: {optimizer.forward()}")
    optimizer.step()
```

As it turned out, the figure is shown as follows:

![image](https://img.picgo.net/2024/10/15/imageb1a4c217ff50d8ed.png)

Implementation in Python:

```python
plt.plot(x_pos[0, 1], x_pos[0, 2], 'b+', label='Admitted')
plt.plot(x_neg[0, 1], x_neg[0, 2], 'bo', label='Not admitted')
x1 = np.linspace(np.min(x[:, 1]), np.max(x[:, 1]), 100)
x2 = (-optimizer.theta[0] - optimizer.theta[1] * x1) / optimizer.theta[2]
plt.plot(x1, x2, 'r-', label='Decision boundary')
plt.legend(loc='upper right')
plt.plot(x_pos[1:, 1], x_pos[1:, 2], 'b+', label='Admitted')
plt.plot(x_neg[1:, 1], x_neg[1:, 2], 'bo', label='Not admitted')
plt.xlabel('Exam 1 score')
plt.ylabel('Exam 2 score')
plt.show()
```

## Answers for Questions

### What values of θ did you get? How many iterations were required for convergence?

Theta: [-16.37874341   0.14834077   0.15890845], and it takes 5 iterations to converge.

### What is the probability that a student with a score of 20 on Exam 1 and a score of 80 on Exam 2 will not be admitted?

Probability of not being admitted: 0.668021864022303