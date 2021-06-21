#!/usr/bin/env python
# coding=utf-8
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import matplotlib.ticker as ticker
sns.set(style="darkgrid") #这是seaborn默认的风格
#scatterplot参数
#数据来源可在seaborn的GitHub上查找
tips = sns.load_dataset("Picture")
ax = sns.lineplot(x="N", y="OOBErrorRate",
markers=True, dashes=False, data=tips)
ax.set_xlim(10**0, 10**3);
ax.xaxis.set_major_locator(ticker.LogLocator(base=10.0, numticks=None))
plt.show()
