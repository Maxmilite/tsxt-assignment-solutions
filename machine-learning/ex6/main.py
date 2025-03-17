# CSE6242/CX4242 Homework 4 Sketch Code
# Please use this outline to implement your decision tree. You can add any code around this.

import csv
import numpy as np
import tqdm
import os

# Enter You Name Here
myname = "Fei-Yu-" # or "Doe-Jane-"

os.makedirs('decision-trees', exist_ok=True)

# Implement your decision tree below
class DecisionTree():
    def __init__(self):
        self.tree = {}

    # entropy function
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

    # gini index function
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

    # split based on feature
    def split(self, dataset, feature_index, value):
        left_split = [instance for instance in dataset if instance[feature_index] == value]
        right_split = [instance for instance in dataset if instance[feature_index] != value]
        return left_split, right_split

    # find the best split
    def best_split(self, dataset, criterion='entropy'):
        best_score = float('inf')
        best_feature = None
        best_split_value = None
        best_left_split = None
        best_right_split = None

        num_features = len(dataset[0]) - 1
        for feature_index in range(num_features):
            feature_values = set(instance[feature_index] for instance in dataset)
            for value in feature_values:
                left_split, right_split = self.split(dataset, feature_index, value)
                if criterion == 'entropy':
                    score = self.entropy(left_split) * len(left_split) / len(dataset) + \
                            self.entropy(right_split) * len(right_split) / len(dataset)
                elif criterion == 'gini':
                    score = self.gini_index(left_split) * len(left_split) / len(dataset) + \
                            self.gini_index(right_split) * len(right_split) / len(dataset)

                if score < best_score:
                    best_score = score
                    best_feature = feature_index
                    best_split_value = value
                    best_left_split = left_split
                    best_right_split = right_split

        return best_feature, best_split_value, best_left_split, best_right_split

    def build_tree(self, dataset, criterion='entropy'):
        class_values = set(instance[-1] for instance in dataset)
        
        # all instances have the same class label
        if len(class_values) == 1:
            return class_values.pop()

        # no features left to split
        if len(dataset[0]) == 1:
            return max(class_values, key=lambda value: sum(1 for instance in dataset if instance[-1] == value))

        # select the best split
        feature_index, split_value, left_split, right_split = self.best_split(dataset, criterion)

        # build the tree recursively
        left_branch = self.build_tree(left_split, criterion)
        right_branch = self.build_tree(right_split, criterion)

        return {
            'feature': feature_index,
            'value': split_value,
            'left': left_branch,
            'right': right_branch
        }

    def learn(self, training_set, criterion='entropy'):
        self.tree = self.build_tree(training_set, criterion)

    def classify(self, test_instance):
        node = self.tree
        while isinstance(node, dict):
            feature_index = node['feature']
            split_value = node['value']
            if test_instance[feature_index] == split_value:
                node = node['left']
            else:
                node = node['right']
        return node

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
        # tree.learn(training_set)
        tree.tree = eval(open(f"decision-trees/entropy-tree-{test}.txt").read())

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
        # tree.learn(training_set, criterion='gini')
        tree.tree = eval(open(f"decision-trees/gini-tree-{test}.txt").read())

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

if __name__ == "__main__":
    # run_decision_tree()
    visualize_tree("decision-trees/entropy-tree-0.txt")
