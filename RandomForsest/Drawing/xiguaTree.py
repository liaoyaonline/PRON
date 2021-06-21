#!/usr/bin/env python
# coding=utf-8
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set(style="darkgrid") #这是seaborn默认的风格
#scatterplot参数
#数据来源可在seaborn的GitHub上查找
tips = sns.load_dataset("TreeData")
ax = sns.scatterplot(x="density", y="suger",hue="size",data=tips)

plt.show()
