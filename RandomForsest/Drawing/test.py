#!/usr/bin/env python
# coding=utf-8
#调用seaborn
import seaborn as sns
#调用seaborn自带数据集
df = sns.load_dataset('iris')
#显示数据集
df.head()
