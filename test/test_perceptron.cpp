
#include <iostream> 
#include <string>
#include <vector> 

using namespace std; 

#include "../util/util.h" 
#include "../src/perceptron.h" 
#include "../util/system.h" 

void test_perception()
{
    double startTime = PITime(); 

    vector<int> testlabel, trainlabel; 
    vector<vector<int>> testfeature, trainfeature;
    string fileName = "../data/mnist_train.csv"; 
    string testFileName = "../data/mnist_test.csv"; 

    LoadDataInTwoClass(fileName, trainlabel, trainfeature); 
    LoadDataInTwoClass(testFileName, testlabel, testfeature); 
    
    cout << "start KNN  train data " << trainfeature.size() << " , " << trainlabel.size() << endl; 
    PerceptionMachine a; 
    a.setLearningStep(0.0001);
    a.setTargetSize(trainfeature); 
    a.setMaxIter(30); 
    a.train(trainfeature, trainlabel); 
    
    
    cout << "start KNN  test data  " << testfeature.size() << ", " << testlabel.size()   << endl; 
    float acc_rate = a.test(testfeature, testlabel); 

    double endTime = PITime(); 
    cout << " using time: " << endTime - startTime << endl; 

    cout << " acc is " << acc_rate << endl; 
}


int main()
{
    test_perception(); 

    return 0; 
}


