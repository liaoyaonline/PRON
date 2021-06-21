#!/usr/bin/env python
# coding=utf-8
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
# 条形图的绘制--垂直条形图
# 读入数据
gini = pd.read_csv('GiniImportance.csv')
#GDP = pd.read_excel('Province GDP 2017.xlsx')
'''
Province    GDP
北京    2.8
上海    3.01
广东    8.99
江苏    8.59
重庆    1.95
天津    1.86
'''
# 设置绘图风格（不妨使用R语言中的ggplot2风格）
plt.style.use('ggplot')
# 绘制条形图
plt.bar(x = range(gini.shape[0]), # 指定条形图x轴的刻度值
        height = gini.GiniImportance, # 指定条形图y轴的数值
        tick_label = gini.FeatureName, # 指定条形图x轴的刻度标签
        color = 'steelblue', # 指定条形图的填充色
        width = 0.8
       )
# 添加y轴的标签
plt.ylabel('Gini Importance')
# 添加条形图的标题
plt.title('Feature_GiniImportance')
# 为每个条形图添加数值标签
for x,y in enumerate(gini.GiniImportance):
    plt.text(x,y+0.01,'%s' %round(y,2),ha='center')
# 显示图形
plt.show()
