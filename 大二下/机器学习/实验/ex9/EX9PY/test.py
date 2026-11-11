import csv
import os
import math
os.chdir("D:/大学/大二下/机器学习/实验/ex9/EX9PY")#转到本机工作区
def Entropy(data):# data 是一个0, 1 向量
    data = [float(x)for x in data]
    one_num = sum(data)
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

def split_data(data, best_feature):# data 12行， 前11行属性，最后一行标签
    sorted_data = sorted(data, key = lambda x:x[best_feature]) # 按照特征行排序
    # print(sorted_data)
    label_index = [] #label_index 是0 1 序列
    for i in sorted_data:
        label_index.append(i[11])
    spilit_value = sorted_data[best_feature][0]
    label_index = [int (x) for x in label_index]
    t_o_num = sum(label_index)
    t_z_num = len(label_index) - t_o_num
    one_num = 0
    zero_num = 0
    best_value = 0
    best_index = 0
    Eorigin = Entropy(label_index)
    for i in range(0, len(label_index)):
        if label_index[i] == 1:
            one_num = one_num + 1
        elif label_index[i] == 0:
            zero_num = zero_num + 1
        IG = Entropy_2(zero_num, one_num) + Entropy_2(t_z_num -zero_num, t_o_num - one_num)
        if Eorigin - IG > best_value:
            best_value = Eorigin - IG
            spilit_value = sorted_data[best_feature][i]
            best_index = i
    one_or_zero = 2
    print(best_value)
    if zero_num > one_num :
        one_or_zero = 0
    elif zero_num <= one_num:
        one_or_zero = 1
    left_sub_list = sorted_data[:(best_index + 1)]
    right_sub_list = sorted_data[(best_index + 1):]
    return [left_sub_list, right_sub_list, spilit_value, Eorigin, one_or_zero]

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
    print(label_data)
    Plus_sum = t_o_num + t_z_num # 1 , 0总个数
    Eorigin = Entropy(label_data)
    for index in range(0, 11): #前11个属性
        sorted_data = sorted(data, key = lambda x:x[index])
        zero_num = 0
        one_num = 0
        for label_index in sorted_data:
            if label_index[11] == 1:
                one_num = one_num + 1
            elif label_index[11] == 0:
                zero_num = zero_num + 1    
            T_IG = 1.0 * (one_num + zero_num) / Plus_sum * Entropy_2(zero_num, one_num) + 1.0 * (Plus_sum - one_num - zero_num ) / Plus_sum * Entropy_2(t_z_num - zero_num, t_o_num - one_num)
            if Eorigin - T_IG > best_data:
                best_data = Eorigin - T_IG
                best_feature = index
                # print(T_IG)
    return best_feature
data = [(1, 2, 3,4, 5,6,7,8,9,10,11, 0), (1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11 ,1), (1,2, 3,4, 5,6,7,8,9,10, 11, 0)]
# data = [(1,2, 3,4, 5,6,7,8,9,10,11, 0), (11,12,13,14,15,16,17,18,19,110,111,1)]
# [data1, data2, spv, e, oz] = split_data(data, 0)
# print(data1, data2)
feature = select_best_feature(data)
print(feature)
# print(e)
# print(Entropy_2(1, 2))




from graphviz import Digraph

def draw_tree(tree, dot=None):
    if dot is None:
        dot = Digraph()
        dot.node(name=str(tree), label=str(tree.value))

    for child in tree.children:
        if child is not None:
            dot.node(name=str(child) ,label=str(child.value))
            dot.edge(str(tree), str(child))
            draw_tree(child, dot)

    return dot

# 假设你的决策树对象是 tree
dot = draw_tree(tree)
dot.render('decision_tree.gv', view=True)  # 保存并打开生成的图
