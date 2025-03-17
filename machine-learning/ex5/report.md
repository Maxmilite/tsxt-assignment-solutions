# Experiment 5 Report

<center>于斐, 202200130119. Oct. 31, 2024</center>


## Description

The following report shows the results of *Linear Discriminant Analysis* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

## LDA

![image](https://img.picgo.net/2024/10/30/imagef1de8a7f8371a00c.png)

Data were loaded and plotted.

Implementation in Python:

```python
red = np.loadtxt('ex3red.dat')
blue = np.loadtxt('ex3blue.dat')
plt.plot(red[:,0], red[:,1], 'ro')
plt.plot(blue[:,0], blue[:,1], 'bo')
plt.axis([0, 10, 0, 10])
plt.show()
```

### Projection Points

We have $\mu_1$ and $\mu_2$ as the mean of the red and blue points, and $\Sigma$ as the covariance matrix of the red and blue points.

Then for the LDA, we have the following formula:
$$
w = \Sigma^{-1}(\mu_1 - \mu_2)
$$

Then we normalize $w$ by dividing it by its norm.

Finally, we project the points onto the line defined by $w$ using the dot product, and the figure can be obtained.

![image](https://img.picgo.net/2024/10/30/image51bee1ee2a9a2f54.png)

Implementation in Python:

```python
class LDA:
    def __init__(self):
        self.mu1 = None
        self.mu2 = None
        self.sigma = None
        self.w = None

    def fit(self, red, blue):
        self.mu1 = np.mean(red, axis=0)
        self.mu2 = np.mean(blue, axis=0)
        self.sigma = np.cov(red.T) + np.cov(blue.T)
        self.w = np.linalg.inv(self.sigma).dot(self.mu1 - self.mu2)
        self.w = self.w / np.linalg.norm(self.w)

    def projection(self, x):
        return self.w.dot(x)

lda = LDA()
lda.fit(red, blue)

plt.axline((0, 0), (lda.w[0], lda.w[1]), color='black')
plt.plot(red[:,0], red[:,1], 'ro')
plt.plot(blue[:,0], blue[:,1], 'bo')

red_proj_x = np.array([lda.projection(x) for x in red])
blue_proj_x = np.array([lda.projection(x) for x in blue])
red_proj_y = red_proj_x * lda.w[1] / lda.w[0]
blue_proj_y = blue_proj_x * lda.w[1] / lda.w[0]

plt.plot(red_proj_x, red_proj_y, 'ro')
plt.plot(blue_proj_x, blue_proj_y, 'bo')

plt.axis([0, 10, 0, 10])
plt.show()
```

### LDA for N Classes

![image](https://img.picgo.net/2024/10/30/imagea1bfe83b74c0b03c.png)

Implementation in Python:

```python
green = np.loadtxt('ex3green.dat')

plt.plot(red[:,0], red[:,1], 'ro')
plt.plot(blue[:,0], blue[:,1], 'bo')
plt.plot(green[:,0], green[:,1], 'go')
plt.axis([0, 10, 0, 10])
plt.show()
```

Like LDA for 2 classes, we first have $\mu_1, \mu_2, \mu_3$ and $\Sigma$ as the mean and covariance of the 3 classes. 

We then calculate 
$$
B = \sum_{i=1}^{3} n_i (\mu_i - \mu)(\mu_i - \mu)^T
$$
where $n_i$ is the number of samples in class $i$.

We then calculate $W = \Sigma^{-1}B$ and find the eigenvectors of $W$ to get $w_1$ and $w_2$.

We then project the data onto $w_1$ and $w_2$ to get the 2D representation of the data.

![image](https://img.picgo.net/2024/10/30/imagef275c17fa2d9f3a6.png)

Implementation in Python:

```python
class LDAFor3:
    def __init__(self):
        self.mu1 = None
        self.mu2 = None
        self.mu3 = None
        self.mu = None
        self.sigma = None
        self.b = None

        self.w1 = None
        self.w2 = None

    def fit(self, red, blue, green):
        self.mu1 = np.mean(red, axis=0)
        self.mu2 = np.mean(blue, axis=0)
        self.mu3 = np.mean(green, axis=0)
        self.mu = (self.mu1 + self.mu2 + self.mu3) / 3

        s_red = np.cov(red.T)
        s_blue = np.cov(blue.T)
        s_green = np.cov(green.T)
        self.sigma = s_red + s_blue + s_green

        b_red = len(red) * np.outer(self.mu1 - self.mu, self.mu1 - self.mu)
        b_blue = len(blue) * np.outer(self.mu2 - self.mu, self.mu2 - self.mu)
        b_green = len(green) * np.outer(self.mu3 - self.mu, self.mu3 - self.mu)

        self.b = b_red + b_blue + b_green

        self.w = np.dot(np.linalg.inv(self.sigma), self.b)
        values, vectors = np.linalg.eig(self.w)
        self.w1 = vectors[:, np.argmax(values)]
        self.w2 = vectors[:, np.argmin(values)]


    def projection(self, x):
        return np.array([self.w1.dot(x), self.w2.dot(x)])
    
lda3 = LDAFor3()
lda3.fit(red, blue, green)

plt.axline((0, 0), (lda3.w1[0], lda3.w1[1]), color='black')
plt.axline((0, 0), (lda3.w2[0], lda3.w2[1]), color='black')
plt.plot(red[:, 0], red[:, 1], 'ro')
plt.plot(blue[:, 0], blue[:, 1], 'bo')
plt.plot(green[:, 0], green[:, 1], 'go')

red_proj_x = np.array([lda3.projection(x)[0] * lda3.w1[0] for x in red])
red_proj_y = np.array([x * lda3.w1[1] / lda3.w1[0] for x in red_proj_x])
green_proj_x = np.array([lda3.projection(x)[0] * lda3.w1[0] for x in green])
green_proj_y = np.array([x * lda3.w1[1] / lda3.w1[0] for x in green_proj_x])
blue_proj_x = np.array([lda3.projection(x)[0] * lda3.w1[0] for x in blue])
blue_proj_y = np.array([x * lda3.w1[1] / lda3.w1[0] for x in blue_proj_x])

plt.plot(red_proj_x, red_proj_y, 'ro')
plt.plot(blue_proj_x, blue_proj_y, 'bo')
plt.plot(green_proj_x, green_proj_y, 'go')

red_proj_x = np.array([lda3.projection(x)[1] * lda3.w2[0] for x in red])
red_proj_y = np.array([x * lda3.w2[1] / lda3.w2[0] for x in red_proj_x])
green_proj_x = np.array([lda3.projection(x)[1] * lda3.w2[0] for x in green])
green_proj_y = np.array([x * lda3.w2[1] / lda3.w2[0] for x in green_proj_x])
blue_proj_x = np.array([lda3.projection(x)[1] * lda3.w2[0] for x in blue])
blue_proj_y = np.array([x * lda3.w2[1] / lda3.w2[0] for x in blue_proj_x])

plt.plot(red_proj_x, red_proj_y, 'ro')
plt.plot(blue_proj_x, blue_proj_y, 'bo')
plt.plot(green_proj_x, green_proj_y, 'go')

plt.axis([-10, 10, -10, 10])
plt.show()
```