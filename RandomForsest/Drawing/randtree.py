#!/usr/bin/env python
# coding=utf-8
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import make_moons
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import mglearn

# 生成一个用于模拟的二维数据集
X, y = make_moons(n_samples=100, noise=0.25, random_state=3)
print(X)
print(y)
# 训练集和测试集的划分
X_train, X_test, y_train, y_test = train_test_split(X, y, stratify=y,
                                                   random_state=42)
print(X_train)
print(y_train)
print(X_test)
print(y_test)
# 初始化一个包含 5 棵决策树的随机森林分类器
forest = RandomForestClassifier(n_estimators=5, random_state=2)
# 在训练数据集上进行学习
forest.fit(X_train, y_train)

# 可视化每棵决策树的决策边界
fig, axes = plt.subplots(2, 3, figsize=(20, 10))
for i, (ax, tree) in enumerate(zip(axes.ravel(), forest.estimators_)):
    ax.set_title('Tree {}'.format(i))
    mglearn.plots.plot_tree_partition(X_train, y_train, tree, ax=ax)

# 可视化集成分类器的决策边界
mglearn.plots.plot_2d_separator(forest, X_train, fill=True, ax=axes[-1, -1],
                               alpha=0.4)
axes[-1, -1].set_title('Random Forest')
mglearn.discrete_scatter(X_train[:, 0], X_train[:, 1], y_train)

plt.show()
