# Experiment 6 Report

<center>于斐, 202200130119. Nov. 25, 2024</center>


## Description

The following report shows the results of *Decision Tree* experiment. [Python](https://python.org/) and [matplotlib](https://matplotlib.org/) is used to complete this experiemnt.

## Skeleton

The implementation code is based on [this link](https://poloclub.gatech.edu/cse6242/2015fall/hw4/tree.py), which is provided in the tutorial.

## Implementation

Our decision tree support **Entropy** and **Gini Index** for classification.

### Entropy

Entropy is a measure of impurity or uncertainty in a dataset. It originates from information theory and is used to quantify the amount of disorder in a dataset. In the context of decision trees, entropy helps evaluate how well a split reduces the impurity of the data.

The entropy $ H(S) $ of a dataset $ S $ with $ n $ distinct classes can be defined by:

$$
H(S) = - \sum_{i=1}^{n} p_i \log_2(p_i)
$$

where $ p_i = \frac{\text{Number of instances in class } i}{\text{Total number of instances}} $ denotes The proportion of instances in class $ i $.

$ H(S) = 0 $ iff the dataset is completely pure (all instances belong to one class), and $ H(S) $ is maximum when all classes are equally probable (perfect disorder).

### Gini Index

The Gini Index is another measure of impurity in a dataset. It is widely used in decision tree algorithms such as CART (Classification and Regression Trees). Unlike entropy, the Gini Index is simpler to compute but often yields similar results.


The Gini Index $ G(S) $ of a dataset $ S $ with $ n $ distinct classes can be defined by:

$$
G(S) = 1 - \sum_{i=1}^{n} p_i^2
$$

where $ p_i = \frac{\text{Number of instances in class } i}{\text{Total number of instances}} $ denotes the proportion of instances in class $ i $.

$ G(S) = 0 $ iff the dataset is completely pure (all instances belong to one class), and $ G(S) $ approaches its maximum value (close to 0.5) when classes are equally distributed.

In our experiment, we compare the accuracy of the 2 methods by cross validation.

Implementation code in Python is as follows:

```python
def entropy(self, dataset):
    class_counts = {}
    for instance in dataset:
        label = instance[-1]
        if label not in class_counts:
            class_counts[label] = 0
        class_counts[label] += 1
    
    total_instances = len(dataset)
    entropy_value = 0
    for count in class_counts.values():
        probability = count / total_instances
        entropy_value -= probability * np.log2(probability) if probability > 0 else 0
    return entropy_value

def gini_index(self, dataset):
    class_counts = {}
    for instance in dataset:
        label = instance[-1]
        if label not in class_counts:
            class_counts[label] = 0
        class_counts[label] += 1
    
    total_instances = len(dataset)
    gini_value = 1
    for count in class_counts.values():
        probability = count / total_instances
        gini_value -= probability ** 2
    return gini_value
```

## Evaluation

We use 10-fold cross validation for evaluation. The evaluation results are shown as follows.

| Validation Set | Entropy Accuracy | Gini Accuracy |
| :-: | :-: | :-: |
| 0 | 0.8653061224489796 | 0.8387755102040816 |
| 1 | 0.8653061224489796 | 0.8408163265306122 |
| 2 | 0.8285714285714286 | 0.8530612244897959 |
| 3 | 0.8204081632653061 | 0.8387755102040816 |
| 4 | 0.8367346938775511 | 0.8693877551020408 |
| 5 | 0.8326530612244898 | 0.8408163265306122 |
| 6 | 0.8204081632653061 | 0.8367346938775511 |
| 7 | 0.8244897959183674 | 0.8551020408163266 |
| 8 | 0.8302658486707567 | 0.8282208588957055 |
| 9 | 0.8016359918200409 | 0.83640081799591 |

Entropy Mean Accuracy: 0.8325779391511207  
Gini Mean Accuracy: 0.8438091064646718  
Entropy Standard Deviation: 0.01870823124287128  
Gini Standard Deviation: 0.011330977363814868

Gini method shows better efficiency (accuracy & std.) in this dataset.

Code implementation in Python:

```python
def run_decision_tree():

    # Load data set
    with open("ex6Data.csv") as f:
        next(f, None)
        data = [tuple(line) for line in csv.reader(f, delimiter=",")]
    print("Number of records: %d" % len(data))

    # Split training/test sets
    # You need to modify the following code for cross validation.
    K = 10

    entropy_accuracies = []
    gini_accuracies = []

    for test in tqdm.tqdm(range(K)):
        training_set = [x for i, x in enumerate(data) if i % K != test]
        test_set = [x for i, x in enumerate(data) if i % K == test]
        
        tree = DecisionTree()
        # Construct a tree using training set
        tree.learn(training_set)
        # tree.tree = eval(open(f"decision-trees/entropy-tree-{test}.txt").read())

        # Classify the test set using the tree we just constructed
        results = []
        for instance in test_set:
            result = tree.classify(instance[:-1])
            results.append(result == instance[-1])

        # Accuracy
        accuracy = float(results.count(True)) / float(len(results))
        
        entropy_accuracies.append(accuracy)

        # save the tree
        # with open(f"decision-trees/entropy-tree-{test}.txt", "w") as f:
        #     f.write(str(tree.tree))
    
    for test in tqdm.tqdm(range(K)):
        training_set = [x for i, x in enumerate(data) if i % K != test]
        test_set = [x for i, x in enumerate(data) if i % K == test]
        
        tree = DecisionTree()
        # Construct a tree using training set
        tree.learn(training_set, criterion='gini')
        # tree.tree = eval(open(f"decision-trees/gini-tree-{test}.txt").read())

        # Classify the test set using the tree we just constructed
        results = []
        for instance in test_set:
            result = tree.classify(instance[:-1])
            results.append(result == instance[-1])

        # Accuracy
        accuracy = float(results.count(True)) / float(len(results))
        
        gini_accuracies.append(accuracy)

        # with open(f"decision-trees/gini-tree-{test}.txt", "w") as f:
        #     f.write(str(tree.tree))



    print("Entropy Accuracies: ", entropy_accuracies)
    print("Gini Accuracies: ", gini_accuracies)

    print("Entropy Mean Accuracy: ", np.mean(entropy_accuracies))
    print("Gini Mean Accuracy: ", np.mean(gini_accuracies))

    print("Entropy Standard Deviation: ", np.std(entropy_accuracies))
    print("Gini Standard Deviation: ", np.std(gini_accuracies))

    with open(f"{myname}result.txt", "w") as f:
        f.write(f"Entropy Mean Accuracy: {np.mean(entropy_accuracies)}\n")
        f.write(f"Gini Mean Accuracy: {np.mean(gini_accuracies)}\n")
        f.write(f"Entropy Standard Deviation: {np.std(entropy_accuracies)}\n")
        f.write(f"Gini Standard Deviation: {np.std(gini_accuracies)}\n")

        f.write(f"| Validation Set | Entropy Accuracy | Gini Accuracy |\n")
        f.write(f"| :-: | :-: | :-: |\n")
        for i in range(K):
            f.write(f"| {i} | {entropy_accuracies[i]} | {gini_accuracies[i]} |\n")
```

## Visualization

Since the decision tree is very large, one cannot get a visualization of good views. We drew a decision tree trained on data of 100 rows, which is more simple.

![image](https://img.picgo.net/2024/11/26/image3d49fc97eddc4e23.md.png)

Implementation in Python:

```python
def visualize_tree(path):
    import matplotlib.pyplot as plt

    with open("ex6Data.csv") as f:
        next(f, None)
        data = [tuple(line) for line in csv.reader(f, delimiter=",")]

    data = data[:100]
    K, test = 10, 9

    training_set = [x for i, x in enumerate(data) if i % K != test]
    test_set = [x for i, x in enumerate(data) if i % K == test]
        
    tree = DecisionTree()
    tree.learn(training_set)
    tree = tree.tree
    
    with open("ex6Data.csv") as f:
        features = next(csv.reader(f, delimiter=","))

    fig, ax = plt.subplots(figsize=(20, 10))
    ax.axis('off')
    ax.set_aspect('equal')

    def subtree_size(node):
        if isinstance(node, dict):
            if ('subtree_size' in node):
                return node['subtree_size']
            node['subtree_size'] = subtree_size(node['left']) + subtree_size(node['right']) + 1
            return node['subtree_size']
        else:
            return 1

    def plot_node(node, depth=0, pos=(0.5, 1), parent_pos=None):
        if isinstance(node, dict):
            feature = features[node['feature']]
            value = node['value']
            left = node['left']
            right = node['right']
            
            ax.text(pos[0], pos[1], f"{feature} = {value}", ha='center', va='center', bbox=dict(facecolor='white', edgecolor='black'))
            
            if parent_pos:
                ax.plot([parent_pos[0], pos[0]], [parent_pos[1], pos[1]], 'k-')
            
            left_size = subtree_size(left)
            right_size = subtree_size(right)
            total_size = left_size + right_size

            left_pos = (pos[0] - 0.5 * (right_size / total_size), pos[1] - 0.1)
            right_pos = (pos[0] + 0.5 * (left_size / total_size), pos[1] - 0.1)
            
            plot_node(left, depth + 1, left_pos, pos)
            plot_node(right, depth + 1, right_pos, pos)
        else:
            ax.text(pos[0], pos[1], str(node), ha='center', va='center', bbox=dict(facecolor='lightgrey', edgecolor='black'))
            if parent_pos:
                ax.plot([parent_pos[0], pos[0]], [parent_pos[1], pos[1]], 'k-')
        
    plot_node(tree)

    plt.savefig(f"{path}.png")
```