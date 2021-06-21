#!/usr/bin/env python
# coding=utf-8
import matplotlib.pyplot as plt 
import seaborn as sns
sns.set(style="darkgrid")
x4=[0.381,0.381,1,1,0,0,1]
y4=[0.5,0.2045,0.2045,0.155,0.155,0.126,0.126]
test = plt.plot(x4, y4)
tips = sns.load_dataset("TreeData")
test = sns.scatterplot(x="density", y="suger",hue="size",data=tips)
plt.fill_between(x4,y4, where=(y4 <= 0.2045) & (y4 <= 0.126) , facecolor='purple')
plt.show()
