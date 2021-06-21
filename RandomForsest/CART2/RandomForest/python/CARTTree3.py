#!/usr/bin/env python
# coding=utf-8
# !/usr/bin/env python3
# coding=utf-8
"""
Decision Tree on the Basis of sklearn module
Author  :Chai Zheng
Blog    :http://blog.csdn.net/chai_zheng/
Github  :https://github.com/Chai-Zheng/Machine-Learning
Email   :zchaizju@gmail.com
Date    :2017.10.13
"""

import os
import time
import pydotplus
import numpy as np
from sklearn import tree
from sklearn import datasets
from sklearn.externals.six import StringIO
from sklearn.model_selection import train_test_split

print('Step 1.Loading data...')
data = datasets.load_iris()
features = data['data']
target = data['target'].reshape(150, 1)
data = np.concatenate((features, target), axis=1)
x = data[:,: -1]
y = data[:,-1].reshape(150,1)
#X_train,X_test,Y_train,Y_test = train_test_split(x,y,test_size=0.4)
print('---Loading and splitting completed.')

print('Step 2.Training...')
startTime = time.time()
clf = tree.DecisionTreeClassifier()
#clf = clf.fit(X_train,Y_train)
clf = clf.fit(x,y)
print('---Training Completed.Took %f s.'%(time.time()-startTime))

print('Step 3.Testing...')
#Y_predict = clf.predict(X_test)
#matchCount = 0
#for i in range(len(Y_predict)):
#    if Y_predict[i] == Y_test[i]:
#        matchCount += 1
#accuracy = float(matchCount/len(Y_predict))
#print('---Testing completed.Accuracy: %.3f%%'%(accuracy*100))

feature_name = ['sepal length (cm)','sepal width (cm)','petal length (cm)','petal width (cm)']
target_name = ['Class1','Class2','Class3']

dot_data = StringIO()
tree.export_graphviz(clf,out_file = dot_data,feature_names=feature_name,
                     class_names=target_name,filled=True,rounded=True,
                     special_characters=True)
graph = pydotplus.graph_from_dot_data(dot_data.getvalue())
graph.write_pdf("WineTree.pdf")
print('Visible tree plot saved as pdf.')
