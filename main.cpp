#include "MnistDataClass.h"
#include "MatrixClass.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace std;

MatrixClass X, W1, H, W2, Y, B1, B2, Y2, dJdB1, dJdB2, dJdW1, dJdW2, dJdB1s, dJdB2s, dJdW1s, dJdW2s;
double learningRate;

void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate);
double random(double x);
MatrixClass computeOutput(vector<double> input);
double sigmoid(double x);
void learn(vector<double> expectedOutput);
double sigmoidePrime(double x);
double stepFunction(double x);
void updateMatrix();

const int MINI_BATCH_SIZE = 50;
const int NUM_EPOCHS = 3;

int main(){
	MnistDataClass data("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
	srand(time(NULL));
	init(784, 20, 10, 3.0);
	for (int m = 0; m < NUM_EPOCHS; m++) {
		for (int i = 0; i < (data.getNumImages() - 100) / MINI_BATCH_SIZE; i++) { //
			dJdB1s.clear();
			dJdB2s.clear();
			dJdW1s.clear();
			dJdW2s.clear();
			for (int j = 0; j < MINI_BATCH_SIZE; j++) {
				computeOutput(data.getPixelData(i*MINI_BATCH_SIZE + j));
				learn(data.getImageNumber(i*MINI_BATCH_SIZE + j));
			}
			updateMatrix();
		}
	}
	
	for (int i = data.getNumImages() - 100; i < data.getNumImages(); i++) {
		for (int j = 0; j < 10; j++){
			if (data.getImageNumber(i)[j] == 1) cout << j;
		}
		cout << " : " << computeOutput(data.getPixelData(i)).applyFunction(stepFunction) << endl;
	}
	
	
	cout << "done" << endl;
	while (1){}
	return 1;
}

void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate) {
	learningRate = rate;

	W1 = MatrixClass(inputNeuron, hiddenNeuron);
	W2 = MatrixClass(hiddenNeuron, outputNeuron);
	B1 = MatrixClass(1, hiddenNeuron);
	B2 = MatrixClass(1, outputNeuron);

	dJdB1s = MatrixClass(1, hiddenNeuron);
	dJdB2s = MatrixClass(1, outputNeuron);
	dJdW2s = MatrixClass(hiddenNeuron, outputNeuron);
	dJdW1s = MatrixClass(inputNeuron, hiddenNeuron);

	W1 = W1.applyFunction(random);
	W2 = W2.applyFunction(random);
	B1 = B1.applyFunction(random);
	B2 = B2.applyFunction(random);
}

double random(double x) {
	return (double)(rand() % 10000 + 1) / 10000 - 0.5;
}

MatrixClass computeOutput(vector<double> input) {
	X = MatrixClass({ input }); // row matrix
	H = X.dot(W1).add(B1).applyFunction(sigmoid);
	Y = H.dot(W2).add(B2).applyFunction(sigmoid);
	return Y;
}

void learn(vector<double> expectedOutput) {
	Y2 = MatrixClass({ expectedOutput }); // row matrix

	// We need to calculate the partial derivative of J, the error, with respect to W1,W2,B1,B2

	// compute gradients
	dJdB2 = Y.subtract(Y2).multiply(H.dot(W2).add(B2).applyFunction(sigmoidePrime));
	dJdB1 = dJdB2.dot(W2.transpose()).multiply(X.dot(W1).add(B1).applyFunction(sigmoidePrime));
	dJdW2 = H.transpose().dot(dJdB2);
	dJdW1 = X.transpose().dot(dJdB1);

	dJdB1s = dJdB1s.add(dJdB1);
	dJdB2s = dJdB2s.add(dJdB2);
	dJdW1s = dJdW1s.add(dJdW1);
	dJdW2s = dJdW2s.add(dJdW2);
}

void updateMatrix() {
	W1 = W1.subtract(dJdW1s.multiply(learningRate / MINI_BATCH_SIZE));
	W2 = W2.subtract(dJdW2s.multiply(learningRate / MINI_BATCH_SIZE));
	B1 = B1.subtract(dJdB1s.multiply(learningRate / MINI_BATCH_SIZE));
	B2 = B2.subtract(dJdB2s.multiply(learningRate / MINI_BATCH_SIZE));
}

double sigmoid(double x) {
	return (1.0 / (1.0 + exp(-x)));
}

double sigmoidePrime(double x) {
	return sigmoid(x)*(1 - sigmoid(x));
}

double stepFunction(double x) {
	if (x > 0.9) {
		return 1.0;
	}
	if (x < 0.1) {
		return 0.0;
	}
	return x;
}