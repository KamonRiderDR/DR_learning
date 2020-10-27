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

	int SampleCount;// ��¼�ܵ���������

	double*** weights;//Ȩ�أ������� ����� -> ���� -> �����
	double** bias;//ƫ�����E�������������
	float studyRate;

	double* hiddenLayerOutput;//�������
	double* outLayerOutput;// ��������

	double*** sampleDeltaBias;// ������ƫ�ñ仯��
	double**** sampleDeltaWeights;// ������Ȩֵ�仯��
	double** outputMat;// ��������������

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
