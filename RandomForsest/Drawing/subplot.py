#!/usr/bin/env python
# coding=utf-8
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
sns.set(style="darkgrid")
x = np.linspace(0.0,1.0)
y1 = np.sin(np.pi*x)
tips = sns.load_dataset("TreeData")
ax = sns.scatterplot(x="density", y="suger",hue="size",data=tips)
ax = plt.plot(x,y1,'b--',label='sin(pi*x)')
plt.legend()
plt.show()
