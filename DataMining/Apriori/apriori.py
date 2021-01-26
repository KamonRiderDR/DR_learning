from numpy import *
import time

def loadDataSet():
    return [[1,3,4], [2,3,5], [1,2,3,5], [2,5]]

def createC1(dataSet):
    C1 = []
    for transaction in dataSet:
        for item in transaction:
            if not [item] in C1:
                C1.append([item])
    C1.sort()
    return list(map(frozenset,C1))
 
def scanD(D,Ck,minSupport):
    ssCnt = {}
    for tid in D:
        for can in Ck:
            if can.issubset(tid):
                if not can in ssCnt:
                    ssCnt[can] = 1
                else:
                    ssCnt[can] += 1
    numItems = float(len(D))
    retList = []
    supportData = {}
    for key in ssCnt:
        support = ssCnt[key]/numItems
        if support >= minSupport:
            # retList.insert(0,key)
            retList.append(key)
        supportData[key] = support
    return retList, supportData
 
# def aprioriGen(Lk, k):
#     retList = []
#     lenLk = len(Lk)
#     for i in range(lenLk):
#         for j in range(i+1, lenLk):
#             L1 = list(Lk[i])[:k-2]
#             L2 = list(Lk[j])[:k-2]
#             L1.sort()
#             L2.sort()
#             if L1 == L2:
#                 retList.append(Lk[i]|Lk[j])
#     return retList
 
def aprioriGen(Lk, k):
    lenLk = len(Lk)
    temp_dict = {}
    for i in range(lenLk):
        for j in range(i+1, lenLk):
            L1 = Lk[i]|Lk[j]
            if len(L1) == k:
                if not L1 in temp_dict:
                    temp_dict[L1] = 1
    return list(temp_dict)
 
def apriori(dataSet, minSupport = 0.5):
    C1 = createC1(dataSet)
    # print("C1",C1)
    D = list(map(set,dataSet))
    L1,supportData = scanD(D, C1, minSupport)
    # print("L1",L1)
    L = [L1]
    k = 2
    while (len(L[k-2]) > 0):
        Ck = aprioriGen(L[k-2],k)  # 生成候选项集
        # print("生成的候选项集",Ck)
        Lk, supK = scanD(D,Ck,minSupport) # 按支持度筛选候选项集
        # print("筛选出频繁项集",Lk)
        supportData.update(supK)
        L.append(Lk)
        k += 1
    return L,supportData
 
def calcConf(freqSet, H, supportData, br1, minConf = 0.7): # 筛选符合可信度要求的规则，并返回符合可信度要求的右件
    prunedH = []  # 存储符合可信度的右件
    for conseq in H:  # conseq就是右件，freqSet是原始频繁项,freqSet-conseq是左件
        conf = supportData[freqSet]/supportData[freqSet-conseq] # 计算可信度
        if conf>= minConf:
            print(freqSet-conseq,"-->",conseq,"\tconf:",conf)
            br1.append((freqSet-conseq,conseq,conf))
        else:
            prunedH.append(conseq) # 不符合可信度的右件添加到列表中
    return prunedH
 
# def rulesFromConseq(freqSet, H, supportData, br1, minConf=0.7):  # 原版Apriori原理来减少创造的规则
#     m = len(H[0])
#     if (len(freqSet)>(m+1)):
#         Hmp1 = aprioriGen(H, m+1)
#         Hmp1 = calcConf(freqSet, Hmp1, supportData, br1, minConf)
#         if (len(Hmp1) >1):
#             rulesFromConseq(freqSet, Hmp1, supportData, br1,minConf)
 
def rulesFromConseq(freqSet, H, supportData, br1, minConf=0.7): # 新版Apriori原理来减少创造的规则
    is_find = True  # 循环标志
    m = 1 # 先创造右件为一个元素的规则
    Hmp1 = H   # H是初始频繁项分散后的列表，[frozenset({2}),frozenset({3}),frozenset({5)],Hmp1是组合后的右件，因为我们的aprioriGen不能组建只有1个元素的右件，所以右件为1个元素的时候我们直接H赋值过去，当右件元素数是2以上的时候，再用aprioriGen组合出来
    while is_find:
        if len(freqSet) > m: # 最多循环len(freqSet)-1次，因为右件最多len(freqSet)-1个元素，右件元素的数从1增长到len(freqSet)-1，故最多循环len(freqSet)-1次
            if m > 1: # 我们改编的aprioriGen()函数至少产生C2,不能产生C1，因此这里加了if
                Hmp1 = aprioriGen(H,m)  # H里的元素自由组合成右件，右件的元素个数是m
            H_no = calcConf(freqSet, Hmp1, supportData, br1, minConf) # 筛选符合可信度的规则,把不符合的右件存起来
            if len(H_no) != 0: # 如果有不满足可信度的右件
                H_no = list(set(frozenset([item]) for row in Hmp1 for item in row)) # 我们把列表中的每个元素都分割出来，比如[{2,3},{3,4}] 分割后为[{2},{3},{4}]，方便我们再次组合，这里也是Apriori原理的精髓所在，这么操作就是把不满足的右件及其超集提出来，然后后面做减法。
                H = list(set(H)-set(H_no))  # 可组合的集合减去不满足可信度的右件的集合
            m = m + 1 # 右件个数不断增加，第一次右件元素只有1个，第二次循环右件元素就有两个了
            if len(H) < m:  # 如果剩余的可自由组合的元素个数少于新右件所需要的元素数，比如就剩两个元素可组合了，想要组成C3作右件，肯定是不可能的，那么结束循环
                is_find = False
        else:  # 如果循环次数达到最大，也结束循环
            is_find = False
 
def generateRules(L, supportData, minConf=0.7):  # 产生规则
    bigRuleList = []
    for i in range(1, len(L)):  # 从L2开始创造规则
        for freqSet in L[i]:
            H1 = [frozenset([item]) for item in freqSet]
            if i>1:  # L3开始使用Apriori原理
                rulesFromConseq(freqSet, H1, supportData, bigRuleList, minConf)
            else:  # L2不能使用Apriori原理，只能老老实实挨个创造规则
                calcConf(freqSet, H1, supportData, bigRuleList, minConf)
    return bigRuleList
 
if __name__ == "__main__":
    dataSet = loadDataSet()
    begin_time = time.time()
    L,suppData = apriori(dataSet)
 
    rules = generateRules(L, suppData, minConf=0.5)
    end_time = time.time()
    print("程序花费时间{}秒".format(end_time-begin_time))
    # print(L)
    # print(suppData)