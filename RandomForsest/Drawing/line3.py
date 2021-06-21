#!/usr/bin/env python
# coding=utf-8
import numpy as np
import matplotlib.pyplot as plt 
import seaborn as sns
sns.set(style="darkgrid")
x=[0.381,0.381,1,1,0,0,1]
y=[0.5,0.2045,0.2045,0.155,0.155,0.126,0.126]
test = plt.plot(x, y)
tips = sns.load_dataset("TreeData")
test = sns.scatterplot(x="density", y="suger",hue="size",data=tips)
plt.show()
