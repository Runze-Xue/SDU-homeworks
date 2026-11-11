# CSE6242/CX4242 Homework 4 Sketch Code
# Please use this outline to implement your decision tree. You can add any code around this.

import csv
import os
import math
os.chdir("D:/大学/大二下/机器学习/实验/ex9/EX9PY")#转到本机工作区
# Enter You Name Here
myname = "Xue-Runze-" # or "Doe-Jane-"

# Implement your decision tree below
def Entropy(data):# data 是一个0, 1 向量
    one_num = 0
    one_num =int(sum(data))
    zero_num = len(data) - one_num
    one_calc = 0 
    zero_calc = 0
    if one_num != 0:
        one_calc = 1.0 * one_num / len(data) * math.log2(1.0 * (one_num) / len(data))
    if zero_num != 0:
        zero_calc = 1.0 * zero_num / len(data) * math.log2(1.0 * (zero_num) / len(data))
    return  -one_calc - zero_calc

def Entropy_2(zero_num, one_num):
    total_num = zero_num + one_num
    one_calc = 0 
    zero_calc = 0
    if one_num != 0:
        one_calc = 1.0 * one_num / total_num * math.log2(1.0 * (one_num) / total_num)
    if zero_num != 0:
        zero_calc = 1.0 * zero_num / total_num * math.log2(1.0 * (zero_num) / total_num)
    return  -one_calc - zero_calc

# class Node:
#     def _init_(self, data):
#         self.data = data
#         self.left = None
#         self.right = None

def is_same(data):#判断数据标签是否为相同标签
    sym = 1
    check = data[0][11]
    for i in data:
        if i[11] != check: 
            sym = 0
    return sym

def select_best_feature(data): #找到最佳分界点  // 前 11 行属性 第 12 行标签
    best_feature = 0  #属性号
    best_data = 0
    t_z_num = 0
    t_o_num = 0
    label_data = [] #0 1 序列点
    for it in data:
        label_data.append(it[11])
        if abs(it[11] - 1) <= 0.0001:
            t_o_num = t_o_num + 1
        elif abs(it[11]) <= 0.0001:
            t_z_num = t_z_num + 1
    Plus_sum = t_o_num + t_z_num # 1 , 0总个数
    Eorigin = Entropy(label_data)
    record_divide = [0,0,0,0,0,0,0,0,0,0,0]
    for i in range(0, 10):
        record_divide[i] = 0# record_divide 初始化
    for index in range(0, 11): #前11个属性
        sorted_data = sorted(data, key = lambda x:x[index])
        zero_num = 0
        one_num = 0
        divide_sym = sorted_data[0][index] #存储分离点实际值，只存储每个属性计算出最大的IG的分离点
        for label_index in range(0, len(sorted_data)):
            if sorted_data[label_index][11] == 1:
                one_num = one_num + 1
            elif sorted_data[label_index][11] == 0:
                zero_num = zero_num + 1   
            if sorted_data[label_index][index] == divide_sym:
                continue
            divide_sym = sorted_data[label_index][index]
            T_IG = 1.0 * (one_num + zero_num) / Plus_sum * Entropy_2(zero_num, one_num) + 1.0 * (Plus_sum - one_num - zero_num ) / Plus_sum * Entropy_2(t_z_num - zero_num, t_o_num - one_num)
            if Eorigin - T_IG > best_data:
                best_data = Eorigin - T_IG
                best_feature = index
                record_divide[index] = label_index #记录分割点索引下标
    sorted_data = sorted(data, key = lambda x:x[best_feature]) #按照最优属性再排一下序，取出存入record_divide[]中的分割点 索引
    divide_math = int(record_divide[best_feature]) + 1
    left_sub_list = sorted_data[:divide_math]
    right_sub_list = sorted_data[divide_math:]
    zero_or_one = 2
    if zero_num > one_num :
        zero_or_one = 0
    else:
        zero_or_one = 1
    # print(len(data))
    return [left_sub_list, right_sub_list, best_feature ,sorted_data[int(record_divide[best_feature])][best_feature], Eorigin, zero_or_one] #第三个元素是 对应属性的具体值

# def split_data(data, best_feature):# data 12行， 前11行属性，最后一行标签
#     print(best_feature)
#     sorted_data = sorted(data, key = lambda x:x[best_feature]) # 按照特征行排序
#     label_index = [] #label_index 是0 1 序列
#     for i in sorted_data:
#         label_index.append(i[11])
#     spilit_value = sorted_data[0][best_feature]
#     label_index = [float (x) for x in label_index]
#     t_o_num = sum(label_index)
#     t_z_num = len(label_index) - t_o_num
#     one_num = 0
#     zero_num = 0
#     best_value = 0
#     best_index = 0
#     Eorigin = Entropy(label_index)
#     for i in range(0, len(label_index)):
#         if label_index[i] == 1:
#             one_num = one_num + 1
#         elif label_index[i] == 0:
#             zero_num = zero_num + 1
#         IG = Entropy_2(zero_num, one_num) + Entropy_2(t_z_num -zero_num, t_o_num - one_num)
#         if Eorigin - IG > best_value:
#             best_value = Eorigin - IG
#             spilit_value = sorted_data[i][best_feature]
#             best_index = i
    
#     one_or_zero = 2
#     if zero_num > one_num :
#         one_or_zero = 0
#     elif zero_num <= one_num:
#         one_or_zero = 1
#     left_sub_list = sorted_data[:(best_index + 1)]
#     right_sub_list = sorted_data[(best_index + 1):]
#     return [left_sub_list, right_sub_list, spilit_value, Eorigin, one_or_zero]
# Tree_list 结构 [sym, feature, feature data, left_node, right_node] sym 标志这是一个什么节点，0 是 neg 叶子 1 是 pos叶子 2 是普通过程点 ,默认left_node相应feature的值小于等于feature_data，right_node 大于feature_data
def BuildTree(data, Tree_list, cnt_index):
    # if cnt_index > 1000 :
    #     return  
    # print(len(data))
    if len(data) == 0 :
        Tree_list[cnt_index] = (3,-1)
        return None
    if is_same(data):
        if data[0][11] == 1:
            Tree_list[cnt_index] = (1,-1)
        else :
            Tree_list[cnt_index] = (0, -1)
        return
    
    [left_sub_list, right_sub_list, best_feature, spilit_value, Eorigin, one_or_zero] = select_best_feature(data)
    if Eorigin <0.2:
        Tree_list[cnt_index] = (one_or_zero, -1)
        return 
    # print("right")
    # print(right_sub_list)
    #这里有多个前剪枝策略，所需值 存入第4个返回元素中， 我先选择使用 熵大小来判断是否前剪枝，如果熵小于0.2 则直接划归叶子结点
    if Eorigin <= 0.2:
        Tree_list[cnt_index] = (one_or_zero, -1)
        return 
    left_index = len(Tree_list)
    right_index = len(Tree_list) + 1
    Tree_list.append(left_index)
    Tree_list.append(right_index)
    Tree_list[cnt_index] =(2, best_feature, spilit_value, left_index, right_index)
    if(len(left_sub_list) > 0):
        BuildTree(left_sub_list, Tree_list, left_index)
    if(len(right_sub_list) > 0):
        BuildTree(right_sub_list, Tree_list, right_index)

class DecisionTree():
    Tree = []
    def learn(self, training_set):
        # implement this function
        BuildTree(training_set, self.Tree, 0)

    # implement this function
    def classify(self, test_instance):
        result = 0 # baseline: always classifies as 0
        root = self.Tree[0] #root 为 sym 0, feature 1, feature data 2, left_node 3, right_node 4
        # print("root", root)
        while root[0] == 2:
            if test_instance[root[1]] <= root[2]:
                root = self.Tree[root[3]] #向左子树找
            else:
                root = self.Tree[root[4]] #向右子树找
        if root[0] == test_instance[10]:
            result = 1
        return result

def run_decision_tree():

    # Load data set
    with open("ex6Data.csv") as f:
        next(f, None)
        data = [tuple(float(item) for item in line) for line in csv.reader(f, delimiter=",")]
    print ("Number of records: %d" % len(data))

    # Split training/test sets
    # You need to modify the following code for cross validation.
    K = 10
    training_set = [x for i, x in enumerate(data) if i % K != 9]
    test_set = [x for i, x in enumerate(data) if i % K == 9]
    
    tree = DecisionTree()
    # Construct a tree using training set
    tree.learn( training_set )

    # Classify the test set using the tree we just constructed
    results = []
    for instance in test_set:
        result = tree.classify( instance[:-1] )
        results.append( result == instance[-1])
    # Accuracy
    accuracy = float(results.count(True))/float(len(results))
    print ("accuracy: %.4f" % accuracy)       

    # Writing results to a file (DO NOT CHANGE)
    f = open(myname+"result.txt", "w")
    f.write("accuracy: %.4f" % accuracy)
    f.close()


if __name__ == "__main__":
    run_decision_tree()