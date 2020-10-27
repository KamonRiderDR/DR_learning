#ifndef BPNET1_H
#define BPNET1_H

#include<assert.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<ctime>
#include<Windows.h>
using namespace std;

class BPNet
{
private:
	int numNodesInputLayer;
	int numNodesOutputLayer;
	int numNodesHiddenLayer;

	int SampleCount;// 记录总的样本数量

	double*** weights;//权重，包括了 输入层 -> 隐层 -> 输出层
	double** bias;//偏置项，δE，隐层与输出层
	float studyRate;

	double* hiddenLayerOutput;//隐层输出
	double* outLayerOutput;// 输出层输出

	double*** sampleDeltaBias;// 样本的偏置变化量
	double**** sampleDeltaWeights;// 样本的权值变化量
	double** outputMat;// 样本的输出层输出

public:
	BPNet(int,int,int,int,float);
	~BPNet();

	double sigmoid(double x)
	{
		return (1 / (1 + exp(0 - x)));
	}
	double derivativeSig(double x)
	{
		return (x*(1.0 - x));
	}

	bool isNotConver(int, int**, double);

	void trainVec(float* , int* , int );

	void train(int, float**, int**);
	void predict(float*,float*);
};


#endif // !BPNET1.0_H
