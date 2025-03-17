# Experiment 1 Report

<center>于斐, 202200130119. Sep 22, 2024</center>


## Description

The following report shows the results of *linear regression* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

## Supervised Learning Problem

Initialize libraries and read data from `ex1x.dat` and `ex1y.dat`.

```python
import numpy as np
import torch
from matplotlib import pyplot as plt
x = np.loadtxt('ex1x.dat')
y = np.loadtxt('ex1y.dat')

plt.plot(x, y, 'o')
plt.ylabel('Height in meters')
plt.xlabel('Age in years')
plt.show()
```

The data are shown as the following figure:

![image](https://img.picgo.net/2024/09/22/image16279392e70ff259.png)

Before starting gradient descent, add $x_0 = 1$ intercept term to every example.

```python
m = len(y)
x = np.c_[np.ones(m), x]
```

## 2D Linear Regression

### Gradient Descent Implementation

With respect to the linear regression model and its gradient, an optimizer is implemented for this task.

The linear regression model can be represented as $$h_\theta(x) = \theta^T x$$ and the batch gradient descent update can be represented as $$\theta_j := \theta_j - \alpha \cfrac 1m \sum \limits_{i = 1}^{m} \left( h_\theta (x^{(i)}) - y^{(i)} \right) x_j^{(i)}.$$

Implementation in python:

```python
n = 1
theta = np.zeros(n + 1)

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
        xp = x[:, 1]
        return np.array([np.sum(h - y) / m, np.sum((h - y) * xp) / m])

    def step(self):
        self.theta -= self.alpha * self.gradient()
```

### Optimization

After 1 iteration, $\mathbf{\theta} = [0.07452802, 0.38002167]$.

After 1500 iterations, $\mathbf{\theta} = [0.75015039, 0.06388338]$.

![image](https://img.picgo.net/2024/09/22/image5caea1becfba250f.png)

Code:

```python
optimizer = Optimizer(x, y, theta, alpha)

for i in range(1500):
    optimizer.step()
    if (i == 0):
        print(f'theta = {optimizer.theta}')

print(f'theta = {optimizer.theta}')

plt.plot(x[:, 1], y, 'o')
plt.plot(x[:, 1], optimizer.forward())
plt.legend(['Training data', 'Linear regression'])
plt.title('Linear regression')
```

### Prediction

The model is used to predict the height for two boys of ages 3.5 and 7, and the results are $$0.97374221, 1.19733402$$ respectively.

Code:

```python
predict_x = np.array([3.5, 7])
predict_x = np.c_[np.ones(2), predict_x]
predict_y = np.dot(predict_x, optimizer.theta)

print(predict_y)
```

## Understanding $J(\theta)$

The $J(\theta)$ is shown in the following figure.

| ![image](https://img.picgo.net/2024/09/22/image38518732b30fd080.png) | ![image](https://img.picgo.net/2024/09/22/imagea96477ed1d2fbbcb.png) |
| :-: | :-: |

During my experiment, linearly spaced vector achieves a better performance than logarithmically spaced vector.

Code:

```python
J_vals = np.zeros((100, 100))
theta0_vals = np.linspace(-3, 3, 100)
theta1_vals = np.linspace(-1, 1, 100)

for i in range(len(theta0_vals)):
    for j in range(len(theta1_vals)):
        t = np.array([theta0_vals[i], theta1_vals[j]])
        J_vals[i, j] = np.sum((np.dot(x, t) - y) ** 2) / (2 * m)

from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.view_init(elev=15, azim=-120)

theta0_vals_, theta1_vals_ = np.meshgrid(theta0_vals, theta1_vals)

ax.plot_surface(theta0_vals_, theta1_vals_, J_vals.T, cmap='turbo')
ax.set_xlabel('theta0')
ax.set_ylabel('theta1')
plt.title('J')

# contour plot
plt.contourf(theta0_vals_, theta1_vals_, J_vals.T, levels=50, cmap='turbo')
plt.xlabel('theta0')
plt.ylabel('theta1')
plt.plot(optimizer.theta[0], optimizer.theta[1], 'rx')
plt.legend(['Gradient descent'])
plt.title('Contour, showing minimum')
```