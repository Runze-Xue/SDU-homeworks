import csv
import math
import matplotlib.pyplot as plt

myname = "MCWoodenBoat"

class DecisionTree():

    tree = {"root": (None, None)}

    def learn(self, samples, key):

        # pre pruning: if all the samples belong to the same class
        if len({samples[i][-1] for i in range(0, len(samples))}) == 1:
            self.tree[key] = (-1, samples[0][-1])
            return

        # initialize the target attribute and split point
        attr = None; split_point = None; gain = float('-inf')

        # select the target attribute and target split point
        for a in range(0, len(samples[0]) - 1):
          
            # attribute values
            attr_value_set = {x[a] for x in samples}

            # find the best select point
            for v in attr_value_set:

                TRU = 1; FLS = 0; GRT = 1; LES = 0
                count = [[0, 0], [0, 0]]
                for x in samples: count[int(x[a] > v)][int(x[-1])] += 1

                # E(S) entropy
                total = len(samples)
                pt = (count[GRT][TRU] + count[LES][TRU]) / total
                pf = (count[GRT][FLS] + count[LES][FLS]) / total
                entropy = - pt * math.log2(pt) - pf * math.log2(pf)

                # E(S_g), E(S_l) entropy of the greater set and lesser set
                entropy_grt = 0; entropy_les = 0;
                if sum(count[GRT]) != 0:
                    pt = count[GRT][TRU] / sum(count[GRT])
                    pf = count[GRT][FLS] / sum(count[GRT])
                    if pt != 0: entropy_grt -= pt * math.log2(pt)
                    if pf != 0: entropy_grt -= pf * math.log2(pf)
                if sum(count[LES]) != 0:
                    pt = count[LES][TRU] / sum(count[LES])
                    pf = count[LES][FLS] / sum(count[LES])
                    if pt != 0: entropy_les -= pt * math.log2(pt)
                    if pf != 0: entropy_les -= pf * math.log2(pf)

                # I(S, A): average entropy
                average_entropy = 0
                average_entropy += sum(count[GRT]) / total * entropy_grt 
                average_entropy += sum(count[LES]) / total * entropy_les
                
                # Gain(S, A): E(S) - I(S, A)
                temp_gain = entropy - average_entropy
                if temp_gain > gain:
                    gain = temp_gain; attr = a; split_point = v

        # confirm the attribute and the split point
        self.tree[key] = (attr, split_point)

        # recursive select
        l_set = [x for x in samples if x[attr] <= split_point]
        r_set = [x for x in samples if x[attr] > split_point]
        self.tree[key + "/L"] = (None, None)
        self.tree[key + "/R"] = (None, None)
        self.learn(l_set, key + "/L")
        self.learn(r_set, key + "/R")

        return

    # implement this function
    def classify(self, test_instance):

        A = 0; V = 1; L = 0; R = 1
        key = "root"
        while self.tree[key][A] != -1:
            if test_instance[self.tree[key][A]] > self.tree[key][V]: key += "/R"
            else: key += "/L"
        return self.tree[key][V]

    def visualize(self):

        fig, axis = plt.subplots()
        axis.axis('off')

        for k, v in self.tree.items():
            depth = len(k) / 2 - 2
            direction = 0.5
            delta = 0.5
            for i in range(5, len(k), 2):
                delta /= 2
                if k[i] == 'L': direction -= delta
                else: direction += delta
            text = None
            if v[0] == -1: text = str(int(v[1]))
            else: text = f"x[{v[0]}] > {v[1]:.4f}"
            axis.text(direction, 1 - depth/16, text, fontsize = 8, \
                ha = "center", va = "center", rotation = 270, \
                bbox = dict(facecolor = "white", edgecolor = "black"))
        plt.show()

def string_tuple_to_float(string_tuple):

    float_tuple = [float(string) for string in string_tuple]
    return float_tuple

def run_decision_tree():

    # Load data set
    with open("ex6Data.csv") as f:
        next(f, None)
        data = [string_tuple_to_float(tuple(line)) for line in csv.reader(f, delimiter=",")]
    print("Number of records: %d" % len(data))

    min_accuracy = 0
    best_tree = DecisionTree()

    # 10-fold cross validation
    for turn in range(0, 10):

        # split the training set and test set
        K = 10
        training_set = [x for i, x in enumerate(data) if i % K != turn]
        test_set = [x for i, x in enumerate(data) if i % K == turn]
        
        # Declare a decision tree
        tree = DecisionTree()

        # Construct a tree using training set
        tree.learn(training_set, "root")

        # Classify the test set using the tree we just constructed
        results = []
        for instance in test_set:
            result = tree.classify(instance[:-1])
            results.append(result == instance[-1])

        # Accuracy: Correctly Classified / Total
        accuracy = float(results.count(True)) / float(len(results))
        print(f"Round: {turn + 1}, Accuracy: {accuracy}")

        if accuracy > min_accuracy:
            best_tree = tree
    
    return best_tree

if __name__ == "__main__":
    output = run_decision_tree()
    output.visualize()