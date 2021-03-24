
#ifndef _NAIVEBAYES_H_ 
#define _NAIVEBAYES_H_ 

#include <cassert>
#include <cmath> 
#include <iostream> 
#include <vector> 


template <typename T>
class NaiveBayes 
{
public:
    NaiveBayes()
    {
        m_classNum = 10; 
        m_seperation = 128; 
    }
    ~NaiveBayes()
    {

    }

    void setClassNum(const int& v)
    {
        m_classNum = v; 
    }

    void setSeperation(const int& v)
    {
        m_seperation = v; 
    }

    template<typename U>
    bool train(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
        int sample_num = feature.size(); 
        assert(sample_num == label.size()); 
        if(sample_num <= 0) { return false; } 
        int feature_size = feature[0].size(); 

        m_p = std::vector<T>(m_classNum, 0);  
        // [m_classNum]
        m_Pxy = std::vector<std::vector<std::vector<T>>>(m_classNum, std::vector<std::vector<T>>(feature_size, std::vector<T>(2, 0))); 
        // [m_classNum, feature_size, 2] 

        std::vector<int> labelCnt(m_classNum, 0); 
        for(int i=0; i<sample_num; ++i)
        {
            labelCnt[label[i]] += 0;   
        }
        for(int i=0; i<m_classNum; ++i)
        {
            m_p[i] = (labelCnt[i] + 1.0)*1.0/(sample_num + 10); 
            m_p[i] = log(m_p[i]); 
        }

        for(int i=0; i<sample_num; ++i)
        {
            for(int j=0; j<feature_size; ++j)
            {
                int idx = (feature[i][j]>=m_seperation)?1:0; 
                m_Pxy[label[i]][j][idx] += 1; 
            }
        }
        for(int i=0; i<m_classNum; ++i)
        {
            for(int j=0; j<feature_size; ++j)
            {
                T left = m_Pxy[i][j][0]; 
                T right = m_Pxy[i][j][1]; 
                m_Pxy[i][j][0] = log((left + 1.0)*1.0/(left + right + 2)); 
                m_Pxy[i][j][1] = log((right + 1.0)*1.0/(left + right + 2)); 
            }
        }
        return true; 
    }

    template<typename U>
    int predict(const std::vector<U>& feature) const 
    {
        int feature_size = feature.size(); 
        std::vector<int> resultCnt(m_classNum, 0); 
        for(int i=0; i<m_classNum; ++i)
        {
            T sum = 0; 
            for(int j=0; j<feature_size; ++j)
            {
                int idx = (feature[j]>=m_seperation)?1:0; 
                sum += m_Pxy[i][j][idx];  
            }
            resultCnt[i] = sum + m_p[i]; 
        }
        int maxIdx = 0, maxVal = resultCnt[0]; 
        for(int i=0;i<m_classNum; ++i)
        {
            if(maxVal < resultCnt[i])
            {
                maxVal = resultCnt[i]; 
                maxIdx = i; 
            }
        }
        return maxIdx; 
    }

    template <typename U>
    const float test(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
        int accNum = 0, totalNum = feature.size(); 
        for(int i=0; i<totalNum; ++i)
        {
            int result = predict(feature[i]); 
            if(result == static_cast<int>(label[i]))
            {
                accNum += 1; 
            }
        }
        return (1.0*accNum / totalNum); 
    }

private:
    int m_classNum; 
    int m_seperation; 
    std::vector<T> m_p; 
    std::vector<std::vector<std::vector<T>>> m_Pxy; 
}; 


#endif //_NAIVEBAYES_H_  
