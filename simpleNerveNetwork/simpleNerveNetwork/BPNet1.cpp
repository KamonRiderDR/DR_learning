#include "BPNet1.h"
#include<math.h>
using namespace std;

BPNet::BPNet(int sampleN,int nNTL,int nNOL,int nNHL,float SR)
{
	//初始化各个参数
	SampleCount = sampleN;
	numNodesInputLayer = nNTL;
	numNodesOutputLayer = nNOL;
	numNodesHiddenLayer = nNHL;
	studyRate = SR;
	//

	srand(time(0));
	weights = new double** [2];
	//InputLayer -> HiddenLayer weights
	weights[0] = new double* [numNodesInputLayer];
	for (int i = 0; i < numNodesInputLayer; i++)
	{
		weights[0][i] = new double[numNodesHiddenLayer];
		for (int j = 0; j != numNodesHiddenLayer; j++)
		{
			weights[0][i][j] = (rand() % (2000) / 1000.0 - 1);
			cout << weights[0][i][j] << " ";
		}
	}

	cout << endl;

	//HiddenLayer -> OutputLayer weights
	weights[1] = new double* [numNodesHiddenLayer];
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		weights[1][i] = new double[numNodesOutputLayer];
		for (int j = 0; j != numNodesOutputLayer; j++)
		{
			weights[1][i][j] = (rand() % (2000) / 1000.0 - 1);
			cout << weights[1][i][j] << " ";
		}
	}//

	//初始化并创建偏置空间
	bias = new double* [2];
	//HiddenLayer bias
	bias[0] = new double[numNodesHiddenLayer];
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		bias[0][i] = (rand() % (2000) / 1000.0 - 1);
	}

	//OutputLayer bias
	bias[1] = new double[numNodesOutputLayer];
	for (int i = 0; i < numNodesOutputLayer; i++)
	{
		bias[1][i] = (rand() % (2000) / 1000.0 - 1);
	}//

	//创建隐层与输出层的输出值空间
	hiddenLayerOutput = new double[numNodesHiddenLayer];
	outLayerOutput = new double[numNodesOutputLayer];
	//

	//创建所有样本权值与偏置调整更新的存储空间
	sampleDeltaWeights = new double*** [SampleCount];
	for (int k = 0; k != SampleCount; k++)
	{
		sampleDeltaWeights[k] = new double** [2];//2 for two differrent weights
		
		sampleDeltaWeights[k][0] = new double* [numNodesInputLayer];
		for (int i = 0; i != numNodesInputLayer; i++)
		{
			sampleDeltaWeights[k][0][i] = new double[numNodesHiddenLayer];
		}

		sampleDeltaWeights[k][1] = new double* [numNodesHiddenLayer];
		for (int i = 0; i != numNodesHiddenLayer; i++)
		{
			sampleDeltaWeights[k][1][i] = new double[numNodesOutputLayer];
		}	
	}
	//
	sampleDeltaBias = new double** [SampleCount];
	for(int k = 0;k != SampleCount;k ++)
	{
		sampleDeltaBias[k] = new double* [2];
		sampleDeltaBias[k][0] = new double[numNodesHiddenLayer];
		sampleDeltaBias[k][1] = new double[numNodesOutputLayer];
	}
	//

	//创建所有样本输入输出空间
	outputMat = new double* [SampleCount];
	for (int i = 0; i != SampleCount; i++)
	{
		outputMat[i] = new double[numNodesOutputLayer];
	}

}

BPNet::~BPNet()
{
	//delete weights 
	for (int i = 0; i != numNodesInputLayer; i++)
	{
		delete[] weights[0][i];
	}
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		delete[] weights[1][i];
	}
	delete[] weights;

	//delete bias
	for (int i = 0; i != 2; i++)
	{
		delete[] bias[i];
	}
	delete[] bias;

	//delete all sample's delta weights
	for (int k = 0; k != SampleCount; k++)
	{
		for (int i = 0; i != numNodesInputLayer; i++)
		{
			delete[] sampleDeltaWeights[k][0][i];
		}
		for (int i = 0; i != numNodesHiddenLayer; i++)
		{
			delete[] sampleDeltaWeights[k][1][i];
		}
		for (int i = 0; i != 2; i++)
		{
			delete[] sampleDeltaWeights[k][i];
		}
		delete[] sampleDeltaWeights[k];
	}
	delete[] sampleDeltaWeights;

	//delete all sample's delta bias
	for (int k = 0; k != SampleCount; k++)
	{
		for (int i = 0; i != 2; i++)
		{
			delete[] sampleDeltaBias[k][i];
		}
		delete[] sampleDeltaBias[k];
	}
	delete[] sampleDeltaBias;

	//delete all sample's output
	for (int k = 0; k != SampleCount; k++)
	{
		delete[] outputMat[k];
	}
	delete[] outputMat;

	delete[] hiddenLayerOutput;
	delete[] outLayerOutput;
	cout << " Constructor completed!\n";
}

//
bool BPNet::isNotConver(int sampleNum, int** labelMat, double thresh)
{
	double lossFun = 0.0;
	//loss = 1/2 * Σ △di²
	for (int k = 0; k != sampleNum; k++)
	{
		double loss = 0.00;
		for (int i = 0; i != numNodesOutputLayer; i++)
		{
			loss += (outputMat[k][i] - labelMat[k][i]) * (outputMat[k][i] - labelMat[k][i]);
		}
		lossFun += 0.50 * loss;
	}

	//mark
	int tt = 0;
	cout << tt << " times for training "
		<< lossFun;

	if (lossFun > thresh)
	{
		return true;
	}
	return false;
}
//

//计算各种输出与改变值
void BPNet::trainVec(float* trainVec, int* labelVec, int index)
{
	//计算隐层输出值
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		double hiddenIn = 0.0;
		for (int j = 0; j != numNodesInputLayer; j++)
		{
			hiddenIn += trainVec[j] * weights[0][j][i];
		}
		hiddenIn += bias[0][i];
		hiddenLayerOutput[i] = sigmoid(hiddenIn);
	}//
	
	 //计算输出层的输出
	for (int i = 0; i != numNodesOutputLayer; i++)
	{
		double outIn = 0.0;
		for (int j = 0; j != numNodesHiddenLayer; j++)
		{
			outIn += hiddenLayerOutput[j] * weights[1][j][i];
		}
		outIn += bias[1][i];
		outLayerOutput[i] = sigmoid(outIn);
		outputMat[index][i] = outLayerOutput[i];
	}//

	//计算偏置量的改变量，顺序不能错
	for (int i = 0; i != numNodesOutputLayer; i++)
	{
		sampleDeltaBias[index][1][i] = (-0.1) * (labelVec[i] - outLayerOutput[i]) * derivativeSig(outLayerOutput[i]);
		for (int j = 0; j != numNodesHiddenLayer; j++)
		{
			sampleDeltaWeights[index][1][j][i] = sampleDeltaBias[index][1][i] * hiddenLayerOutput[j];
		}	
	}
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		double z = 0.0;
		for (int k = 0; k != numNodesOutputLayer; k++)
		{
			z += weights[1][i][k] * sampleDeltaBias[index][1][k];//???
		}

		sampleDeltaBias[index][0][i] = z * derivativeSig(hiddenLayerOutput[i]);
		for (int j = 0; j != numNodesInputLayer; j++)
		{
			sampleDeltaWeights[index][0][j][i] = sampleDeltaBias[index][0][i] * trainVec[j];
		}

	}
}

void BPNet::train(int sampleNum, float** trainMat, int** labelMat)
{
	double thre = 1e-4;
	cout << "training start!\n";
	for (int k = 0; k != sampleNum; k++)
	{
		trainVec(trainMat[k], labelMat[k], k);
	}
	cout << "training end!\n";

	int tt = 0;
	//判断是否到了误差允许范围
	while ( isNotConver(sampleNum,labelMat,thre)&&tt<10000 )
	{
		tt++;
		//调整权值
		for (int index = 0; index != sampleNum; index++)
		{
			for(int i = 0;i != numNodesInputLayer;i++)
				for (int j = 0; j != numNodesHiddenLayer; j++)
				{
					weights[0][i][j] -= studyRate * sampleDeltaWeights[index][0][i][j];
				}
		     
			for (int i = 0; i != numNodesHiddenLayer; i++)
			{
				for (int j = 0; j != numNodesOutputLayer; j++)
				{
					weights[1][i][j] -= studyRate * sampleDeltaWeights[index][1][i][j];
				}
			}
		
		}

		for (int index = 0; index != sampleNum; index++)
		{
			for (int i = 0; i < numNodesHiddenLayer; i++)
			{
				bias[0][i] -= studyRate * sampleDeltaBias[index][0][i];
			}
			for (int j = 0; j != numNodesOutputLayer; j++)
			{
				bias[1][j] -= studyRate * sampleDeltaBias[index][1][j];
			}
		}
		//

		for (int k = 0; k != sampleNum; k++)
		{
			trainVec(trainMat[k], labelMat[k], k);
		}
	}
	cout << "training weights and bias succeed!\n";
}

void BPNet::predict(float*in, float*proda)
{
	//计算隐层节点的输出
	for (int i = 0; i != numNodesHiddenLayer; i++)
	{
		double z = 0.0;
		for (int j = 0; j != numNodesInputLayer; j++)
		{
			z += in[i] * weights[0][j][i];
		}
		z += bias[0][i];
		hiddenLayerOutput[i] = sigmoid(z);
	}
	//

	//计算输出节点的输出
	for (int i = 0; i != numNodesOutputLayer; i++)
	{
		double z = 0.0;
		for (int j = 0; j != numNodesHiddenLayer; j++)
		{
			z += hiddenLayerOutput[j] * weights[1][j][i];
		}
		z += bias[1][i];
		outLayerOutput[i] = sigmoid(z);

		cout << outLayerOutput[i] << " ";
	}
}
   
