# CSE6242/CX4242 Homework 4 Sketch Code
# Please use this outline to implement your decision tree. You can add any code around this.

import os
import sys
os.chdir(sys.path[0])
import csv
from graphviz import Digraph#这个包需要自己安装
from math import log2
# Enter You Name Here
#myname = "Doe-John-" # or "Doe-Jane-"
# Implement your decision tree below
def Entropy(p):#二分类求熵
    q=1-p
    p=max(1e-9,p)
    q=max(1e-9,q)
    E=-p*log2(p)-q*log2(q)
    return E
def Gini(p): #二分类求基尼系数
    q=1-p
    G=1-p*p-q*q
    return G
child=0
class DecisionTree():#决策树
    def __init__(self,data,threshold,level,*,way):
        self.threshold=threshold
        self.levelfeature=level #there is 11 features in data
        self.way=way
        self.tree=self.learn(data)
    def learn(self, training_set):#训练集上学习
        # implement this function
        select=sorted([(i,)+self.split(i,training_set) for i in range(self.levelfeature)],key=lambda example:example[1])
        selected=select[0]#the least
        feature=selected[0]#the least entropy of feature
        ans=selected[1]
        pos=selected[2]
        tree = {} 
        if(ans<self.threshold):# the tree is single node tree
            tree['feature']=-1
            label=[example[-1] for example in training_set]
            if(label.count(1)>label.count(0)):
                tree['result']=1
            else:
                tree['result']=0
            return tree
        tree['feature']=feature
        tree['position']=pos
        left_tree=[example for example in training_set if example[feature]<pos]
        right_tree=[example for example in training_set if example[feature]>pos]
        #build the tree childs
        label1=[example[-1] for example in left_tree]
        if(len(left_tree)<self.levelfeature or label1.count(1)==0 or label1.count(0)==0):
            leaf={}
            leaf['feature']=-1
            if(label1.count(1)>label1.count(0)):
                leaf['result'] = 1
            else:
                leaf['result'] = 0
            tree['left_tree'] = leaf
        else:
            tree['left_tree']=self.learn(left_tree)
        
        label2=[example[-1] for example in right_tree]
        if(len(right_tree)<self.levelfeature or label2.count(1)==0 or label2.count(0)==0):
            leaf={}
            leaf['feature']=-1
            if(label2.count(1)>label2.count(0)):
                leaf['result'] = 1
            else:
                leaf['result'] = 0
            tree['right_tree'] = leaf
        else:
            tree['right_tree']=self.learn(right_tree)
        return tree
    # implement this function
    def split(self,feature,training_set):#find the best site of splitting
        training_set.sort(key=lambda sample:sample[feature])
        y0=[example[-1] for example in training_set].count(0)#label 0 number,right
        y1=[example[-1] for example in training_set].count(1)#label 1 number,right
        x0=0#left 0
        x1=0#left 1
        pos=-1
        ans=1#the minimum -H(D|A)
        t=len(training_set)
        for i in range(len(training_set)-1):
            if((x0+x1)!=0 and (y0+y1)!=0):
                if(training_set[i][-1]!=training_set[i+1][-1]):
                    h1=(training_set[i][feature]+training_set[i+1][feature])/2  #the feature/2
                    h2=(x0+x1)/t*self.way(x0/(x0+x1))+(y0+y1)/t*self.way(y0/(y0+y1)) 
                    if(ans>h2):
                        ans=h2
                        pos=h1
            if(training_set[i][-1]==1):#get a step,if the value is 1,then...else ...
                x1+=1
                y1-=1
            else:
                x0+=1
                y0-=1
        return ans,pos
    def classify(self, test_instance):
         # baseline: always classifies as 0
        thetree=self.tree
        while thetree['feature']!=-1:
            feature=thetree['feature']
            pos=thetree['position']
            if(test_instance[feature]<pos):
                thetree=thetree['left_tree']
            else:
                thetree=thetree['right_tree']   
        return thetree['result']
def preorder(tree,r):#前序遍历
    if(tree['feature']==-1):
          print("%d result %d" %(tree['feature'],tree['result']))
          r.write("%d result %d\n" %(tree['feature'],tree['result']))
    else:
          print("%d position %d" %(tree['feature'],tree['position']))
          r.write("%d position %d\n" %(tree['feature'],tree['position']))
          preorder(tree['left_tree'],r)
          preorder(tree['right_tree'],r)
def inorder(tree,r):#中序遍历
    if(tree['feature']==-1):
          print("%d result %d" %(tree['feature'],tree['result']))
          r.write("%d result %d\n" %(tree['feature'],tree['result']))
    else:
          inorder(tree['left_tree'],r)
          print("%d position %d" %(tree['feature'],tree['position']))
          r.write("%d position %d\n" %(tree['feature'],tree['position']))
          inorder(tree['right_tree'],r) 
dot = Digraph(comment='decision tree')
def draw(tree,fea,father):#画树
    global child
    if(child>1):
        dot.edge("n%d" %(father),"n%d" %(child))    
    if tree['feature']==-1:
        dot.node("n%d" %(child),"%d" %(tree['result']))
    else :
        dot.node("n%d" %(child),"feature:%s\nposition:%d" %(fea[tree['feature']],tree['position']))
        father=child
        child+=1
        draw(tree['left_tree'],fea,father)
        child+=1
        draw(tree['right_tree'],fea,father)
def run_decision_tree():
    # Load data set
    with open("ex6Data.csv") as f:
        for i,feas in enumerate(csv.reader(f)):
            if i==0:
                fea=feas;
    f.close()
    with open("ex6Data.csv") as f:
        next(f, None)
        data = [tuple(line) for line in csv.reader(f, delimiter=",")]
    print("Number of records: %d" % (len(data)))
    for i in range(len(data)):
        label=[int(data[i][-1])]
        data[i]=[float(x) for x in data[i][:-1]]+label
    # Split training/test sets
    # You need to modify the following code for cross validation.
    K = 10
    Accuracy=0;
    r = open("result.txt", "w")
    for j in range(10):
        accuracy=0
        training_set = [x for i, x in enumerate(data) if i % K != j]
        test_set = [x for i, x in enumerate(data) if i % K == j]    
        tree = DecisionTree(training_set,0.2,11,way=Gini)
        results = []
        for instance in test_set:
            result = tree.classify( instance[:-1] )
            results.append( result == instance[-1])
        accuracy+= float(results.count(True))/float(len(results))
        Accuracy+=accuracy
        print("preorder traversal\nFeaturePosition，fractional dose，result")
        r.write("preorder traversal FeaturePosition，fractional dose，result\n")
        preorder(tree.tree,r)
        print('inorder traversal\nFeaturePosition，fractional dose，result')
        r.write('inorder traversal FeaturePosition，fractional dose，result')
        inorder(tree.tree,r)
        print("accuracy: %.4f" % accuracy)       
        r.write("accuracy: %.4f\n" % accuracy)
    
    Accuracy/=10 
    draw(tree.tree,fea,0)
    dot.view()
    print("Accuracy: %.4f" % Accuracy)
    r.write("accuracy: %.4f" % accuracy)
    r.close()
    f.close()
    # Writing results to a file (DO NOT CHANGE)
if __name__ == "__main__":
    run_decision_tree()
