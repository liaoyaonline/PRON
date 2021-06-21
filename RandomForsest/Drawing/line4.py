#!/usr/bin/env python
# coding=utf-8
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0,1000)
##y = 1 /(1 + np.exp(-0.01 * x))
y = 2/np.pi * np.arctan(0.01* x);

plt.figure()
plt.plot(x,y)

plt.show()
