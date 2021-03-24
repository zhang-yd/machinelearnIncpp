

#include <iostream> 
#include <string>
#include <vector> 

using namespace std; 

#include "../util/system.h" 
#include "../util/util.h" 
#include "../src/knn.h"


void test_knn()
{
    double startTime = PITime(); 

    typedef KNN<float> knnMachine; 
    knnMachine a; 
    a.setTopNum(10); 

    cout << "start test KNN " << endl; 

    
    vector<int> testlabel, trainlabel; 
    vector<vector<int>> testfeature, trainfeature;
    string fileName = "../data/mnist_train.csv"; 
    string testFileName = "../data/mnist_test.csv"; 

    int maxNum = 500; 

    LoadData(fileName, trainlabel, trainfeature); 
    LoadData(testFileName, testlabel, testfeature, maxNum); 

    cout << "start KNN  train data " << trainfeature.size() << " , " << trainlabel.size() << endl; 

    a.train(trainfeature, trainlabel); 

    cout << "start KNN  test data  " << testfeature.size() << ", " << testlabel.size()   << endl; 

    float acc = a.test(testfeature, testlabel); 

    cout << "acc is " << acc << endl; 

    double endTime = PITime(); 

    cout << "cost time : " << (endTime - startTime) << " with start " << startTime << " and end:  " << endTime << endl; 
}


int main()
{
    test_knn(); 

    return 0; 
}

