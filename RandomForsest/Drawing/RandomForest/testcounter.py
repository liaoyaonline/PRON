#!/usr/bin/env python
# coding=utf-8
import numpy as np
from collections import Counter
from sklearn import datasets

#data = datasets.load_iris()
m = 3
n = 6
aa = [0,1,2,3,4,5,6,7,8,9]
array = ([[ 0, 1, 2, 3],
[ 4, 5, 6, 7],
[ 8, 9, 10, 11],
[12, 13, 14, 15],
[16, 17, 18, 19]])
dataSet=np.array(array)
a = dataSet[:,:-1]
b = dataSet[:,-1]
np.random.shuffle(aa);
print(a)
print(b)
print(aa)
