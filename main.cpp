#include "MnistDataClass.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "NNClass.h"
#include "NN_TrainerClass.h"

#include "TH2.h"
#include "TRint.h"
#include "TFile.h"

using namespace std;

const double LEARNING_RATE = 2.5;
const double MOMENTUM_FACTOR = 0.25;
const int MINI_BATCH_SIZE = 50;
const int NUM_EPOCHS = 1;

int main(){

  TRint *gROOT = new TRint("ROOT", 0, NULL);
  TH1   *hist  = new TH1D("h1", "h1 title", 500, -1.0, 1.0);
  TFile *rFile = new TFile("nn.root", "RECREATE");

  char *buf = "#include \"MatrixClass.h\"";
  gROOT->ProcessLine("#include <vector>");
  gROOT->ProcessLine(buf);

	cout << "Loading MNIST database" << endl;
	time_t t;
	srand((unsigned)time(&t));

	MnistDataClass data("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
	MnistDataClass test("t10k-images.idx3-ubyte","t10k-labels.idx1-ubyte");

	cout << "Initiate Neural Network" << endl;
	NNClass nn(784, RELU, 10, SIGMOID);
	nn.addHiddenLayer(40, RELU);
	//nn.addHiddenLayer(20, RELU);
	//nn.addHiddenLayer(16, RELU);
	nn.init();

	NN_TrainerClass trainer(nn);
	trainer.setLearningRate(LEARNING_RATE);
	trainer.setMiniBatchSize(MINI_BATCH_SIZE);
	trainer.enableMomentum(MOMENTUM_FACTOR);
/*
	//training
	for (int i = 0; i < NUM_EPOCHS; i++) {
		for (int j = 0; j < data.getNumImages() / MINI_BATCH_SIZE; j++) {
			for (int k = 0; k < MINI_BATCH_SIZE; k++) {
				nn.compute(data.getPixelData(j*MINI_BATCH_SIZE + k));

				trainer.backPropogate(data.getImageNumber(j*MINI_BATCH_SIZE + k));
			}
			trainer.updateWeightsAndBiases();
			cout << "Batch " << j << " of epoch " << i+1 << " complete" << endl;
		}
	}*/

	//assessment
	int count = 0;
	for (int i = 0; i < test.getNumImages(); i++) {
		int correctNum = 0;
		for (int j = 0; j < 10; j++){
			if (test.getImageNumber(i)[j] == 1) correctNum = j;
		}
		if (correctNum != nn.compute(test.getPixelData(i))) count++;
	}
	cout << count << " of " << test.getNumImages() << " failed: "
		<< 100*(test.getNumImages() - count) / test.getNumImages() << " pass rate." << endl;

  //trainer.plotNNWeights(0, hist);
  trainer.saveNN(rFile);
  rFile->Close();

  cout << endl << "Press 1 to quit." << endl;
  int exit = 0;
  while(exit != 1){
    cin >> exit;
  }
	return 1;
}
