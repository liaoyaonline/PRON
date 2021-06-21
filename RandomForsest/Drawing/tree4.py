#!/usr/bin/env python
# coding=utf-8
#coding=utf-8
import  numpy as np
import pandas as  pd
import pydotplus
def lrisTrain():
  #预处理-引入鸢尾数据：
  from  sklearn.datasets import load_iris
  iris = load_iris()
 
  from sklearn.cross_validation import train_test_split
  # 把数据分为测试数据和验证数据
  train_data,test_data,train_target,test_target=train_test_split(iris.data,iris.target,test_size=0.2,random_state=1)
  #Model(建模)-引入决策树
  from  sklearn import tree
  #建立一个分类器
  clf = tree.DecisionTreeClassifier(criterion="entropy")
  #训练集进行训练
  clf.fit(train_data,train_target)
  #画图方法1-生成dot文件
  with open('treeone.dot', 'w') as f:
      dot_data = tree.export_graphviz(clf, out_file=None)
      f.write(dot_data)
 
  #画图方法2-生成pdf文件
  dot_data = tree.export_graphviz(clf, out_file=None,feature_names=clf.feature_importances_,
                                  filled=True, rounded=True, special_characters=True)
  graph = pydotplus.graph_from_dot_data(dot_data)
  ###保存图像到pdf文件
  graph.write_pdf("treetwo.pdf")
 
  #进行预测
  y_pred = clf.predict(test_data)
 
  #Verify(验证)
  #引入模块
  from sklearn import metrics
  #法一：通过准确率进行验证
  print(metrics.accuracy_score(y_true =test_target,y_pred=y_pred))
  #法二：通过混淆矩阵验证（横轴：实际值，纵轴：预测值）（理想情况下是个对角阵）
  #print(metrics.confusion_matrix(y_true=test_target,y_pred=y_pred))
lrisTrain()
