#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@文件        :sensorMachine.py
@说明        :手动实现感知机
@时间        :2020/10/26 23:32:13
@作者        :DR
@版本        :1.0
'''

import numpy as np
import random
import matplotlib.pyplot as plt

def sign(x):
    if x > 0:
        return 1
    else:
        return -1

def training(pos_data,neg_data):
    '''
    感知机的训练函数
    '''

    weights = [0,0]
    bias = 0
    learning_rate = 0.1
    all_data = pos_data + neg_data
    n = len(all_data)
    i = 1
    num = 0

    #   modify codes -> all points can be sorted correctedly
    while True:
        while i < n and num <= 2000:
            x1 = all_data[i][0]
            x2 = all_data[i][1]
            y = all_data[i][2]

            y_pred = sign( weights[0]*x1 + weights[1]*x2 + bias )
            print("train data:x:(%d, %d) y:%d ==>y_predict:%d" %(x1,x2,y,y_pred))
            flag = y*y_pred
            num = num+1;

            # refresh
            if flag <= 0:
                #   update
                weights[0] += learning_rate*y*x1
                weights[1] += learning_rate*y*x2
                bias += learning_rate*y
                print("update weight and bias:")
                print(weights[0], weights[1], bias)
                i = 1
            else:
                i = i + 1
        break
    
    #   plot
    plt.plot(np.array(pos_data)[:,0],np.array(pos_data)[:,1],'ro')
    plt.plot(np.array(neg_data)[:,0],np.array(neg_data)[:,1],'bo')
    x_1 = []
    x_2 = []
    for i in range(-10,10):
        x_1.append(i)
        x_2.append((-weights[0]*i-bias)/weights[1])
    plt.plot(x_1,x_2)
    plt.show()

    return weights,bias

def testing():
    '''
    测试训练样本
    '''
    pos = [[1,3,1], [2,5,1], [3,8,1], [2,6,1]]
    neg = [[3,1,-1], [4,1,-1], [6,2,-1], [7,3,-1]]
    pos1 = []
    neg1 = []

    for i in range(1,10):
        x = random.randint(0,10)
        y = random.randint(0,10)

        m = random.randint(0,10)
        n = random.randint(0,10)
        pos1.append([x,y,1])
        neg1.append([m,n,-1])

    weights,bias = training(pos1,neg1)
    print( weights )
   
if __name__ == "__main__":
    testing()
