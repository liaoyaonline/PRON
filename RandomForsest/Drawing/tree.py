#!/usr/bin/env python
# coding=utf-8
from sklearn import tree
from sklearn.datasets import load_iris
from IPython.display import Image
import pydotplus
import matplotlib.pyplot as plt
# 使用iris数据
iris=load_iris()
# 生成决策分类树实例
clf = tree.DecisionTreeClassifier()
# 拟合iris数据
clf = clf.fit(iris.data, iris.target)
# 预测类别
clf.predict(iris.data[:1, :])
# 分别预测属于所有类别的可能性
clf.predict_proba(iris.data[:1, :])
# 可视化决策树
dot_data = tree.export_graphviz(clf, out_file=None,
                               feature_names=iris.feature_names,
                               class_names=iris.target_names,
                               filled=True, rounded=True,
                               special_characters=True)
graph = pydotplus.graph_from_dot_data(dot_data)
# 显示图片
graph.write_pdf("treetwo.pdf")
Image(graph.create_png())
