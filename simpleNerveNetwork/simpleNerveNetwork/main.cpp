#include"BPNet1.h"
using namespace std;

int main()
{
	//�������ʼ������
	int inNodes = 10;
	int hiddenNodes = 8;
	int outNodes = 5;

	int trainClass = 5;
	int numClass = 30;
	int sampleCount = trainClass * numClass;
	//�����ܵ�����

	//����������ģ��
	float** trainMat = new float* [sampleCount];
	for (int k = 0; k != trainClass; k++)
	{
		for (int i = k * numClass; i != (k + 1) * numClass; i++)
		{
			trainMat[i] = new float[inNodes];
			for (int j = 0; j != inNodes; j++)
			{
				trainMat[i][j] = rand() % 1000 / 10000.0 + 0.1 * (2 * k + 1);
				//����ģ�������������
			}
		}
	
	}

	int** labelMat = new int* [sampleCount];
	for (int k = 0; k != trainClass; k++)
	{
		for (int i = k*numClass; i != (k+1)*numClass; i++)
		{
			labelMat[i] = new int[outNodes];
			for (int j = 0; j != trainClass; j++)
			{
				if (j == k)
					labelMat[i][j] = 1;
				else
				labelMat[i][j] = 0;
			}
		}
	}//

	BPNet testingNet(sampleCount, inNodes, outNodes, hiddenNodes, 0.12);
	testingNet.train(sampleCount, trainMat, labelMat);

	for (int i = 0; i != numClass; i++)
	{
		testingNet.predict(trainMat[i + 120], NULL);
		cout << endl;
	}

	//destructor
	for (int i = 0; i != sampleCount; i++)
	{
		delete[] trainMat[i];
	}
	delete[] trainMat;
	for (int i = 0; i != sampleCount; i++)
		delete[] labelMat[i];
	delete[] labelMat;
	cout << "space destructed!\n";

	system("pause");
	return 0;
}