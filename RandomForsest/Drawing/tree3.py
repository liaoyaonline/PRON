#!/usr/bin/env python
# coding=utf-8
from sklearn.tree import DecisionTreeRegressor
import numpy as np 
import matplotlib.pyplot as plt
# 生成一个随机数据集
rng = np.random.RandomState(1)
X = np.sort(5 * rng.rand(80, 1), axis=0)
y = np.sin(X).ravel()
y[::5] += 3 * (0.5 - rng.rand(16))

# 训练模型
tree1 = DecisionTreeRegressor(max_depth=2)
tree2 = DecisionTreeRegressor(max_depth=5)
tree1.fit(X, y)
tree2.fit(X, y)

# 预测
X_test = np.arange(0.0, 5.0, 0.01)[:, np.newaxis]
y1 = tree1.predict(X_test)
y2 = tree2.predict(X_test)

# 画出结果
plt.figure()
plt.scatter(X, y, s=20, edgecolors='black',
           c='darkorange', label='data')
plt.plot(X_test, y1, color="cornflowerblue",
         label="max_depth=2", linewidth=2)
plt.plot(X_test, y2, color="yellowgreen", label="max_depth=5", linewidth=2)
plt.xlabel("data")
plt.ylabel("target")
plt.title("Decision Tree Regression")
plt.legend()
plt.show()
