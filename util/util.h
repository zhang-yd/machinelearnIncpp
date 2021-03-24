
#ifndef _UTIL_H_ 
#define _UTIL_H_ 

#include <string>
#include <iostream>
#include <vector> 
#include <fstream> 
#include <sstream> 

void LoadData(const std::string& file_name, std::vector<int>& label, std::vector<std::vector<int>>& feature, const int& maxNum=-1)
{
    std::ifstream inData; 

    inData.open(file_name.c_str(), std::ifstream::in); 
    if(!inData.good())
	{
		std::cout << " open file faield " << std::endl;
		inData.close();
		return;
	}
    std::string line; 
    std::string cell; 

    label.clear(); 
    feature.clear(); 
    int num = 0; 
    while(std::getline(inData, line))
    {
        num = num + 1; 
        if(maxNum != -1 && num > maxNum)
        {
            break; 
        }

        std::stringstream lineStream(line); 
        // std::istringstream lineStream(line.c_str()); 
        int idx = 0; 

        std::vector<int> tmp; 
        while(std::getline(lineStream, cell, ','))
        {
            int val = std::stoi(cell); 
            if(idx == 0)
            {
                label.push_back(val); 
            }
            else
            {
                tmp.push_back(val); 
            }
            ++idx; 
        }
        feature.push_back(tmp); 
    }
    inData.close(); 
}

void LoadDataInTwoClass(const std::string& file_name, std::vector<int>& label, std::vector<std::vector<int>>& feature,  const int& maxNum=-1)
{
    std::ifstream inData; 

    inData.open(file_name.c_str(), std::ifstream::in); 
    if(!inData.good())
	{
		std::cout << " open file faield " << std::endl;
		inData.close();
		return;
	}
    std::string line; 
    std::string cell; 

    label.clear(); 
    feature.clear(); 
    int num = 0; 
    while(std::getline(inData, line))
    {
        num = num + 1; 
        if(maxNum != -1 && num > maxNum)
        {
            break; 
        }

        std::stringstream lineStream(line); 
        // std::istringstream lineStream(line.c_str()); 
        int idx = 0; 

        std::vector<int> tmp; 
        while(std::getline(lineStream, cell, ','))
        {
            int val = std::stoi(cell); 
            if(idx == 0)
            {
                int l = val > 5? 1:-1; 
                label.push_back(l); 
            }
            else
            {
                tmp.push_back(val); 
            }
            ++idx; 
        }
        feature.push_back(tmp); 
    }
    inData.close(); 
}


#endif // _UTIL_H_ 

