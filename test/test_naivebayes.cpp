

#include <iostream> 
#include <string>
#include <vector> 

using namespace std; 

#include "../util/system.h" 
#include "../util/util.h" 
#include "../src/naiveBayes.h"


void test_naiveBayes()
{
    double startTime = PITime(); 
    cout << "start test naiveBayes " << endl; 

    vector<int> testlabel, trainlabel; 
    vector<vector<int>> testfeature, trainfeature;
    string fileName = "../data/mnist_train.csv"; 
    string testFileName = "../data/mnist_test.csv"; 

    LoadData(fileName, trainlabel, trainfeature); 
    LoadData(testFileName, testlabel, testfeature); 

    cout << "start naiveBayes  train data "  << trainfeature.size() << " , " << trainlabel.size() << endl; 

    typedef NaiveBayes<float> NaiveBayesMachine; 
    NaiveBayesMachine a; 
    a.train(trainfeature, trainlabel); 

    cout << "start naiveBayes  test data  " << testfeature.size() << ", " << testlabel.size()  << endl; 

    float acc = a.test(testfeature, testlabel); 

    cout << "acc is " << acc << endl; 

    double endTime = PITime(); 

    cout << "cost time : " << (endTime - startTime) << " with start " << startTime << " and end:  " << endTime << endl; 
}


int main()
{
    test_naiveBayes(); 

    return 0; 
}

