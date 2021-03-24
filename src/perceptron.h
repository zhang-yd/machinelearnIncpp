
#ifndef _PERCEPTRON_H_ 
#define _PERCEPTRON_H_ 

#include <vector> 
#include <string> 
#include <iostream> 
#include <cassert> 


template<typename T>
class Perception 
{
public:
    Perception()
    {

    }
    ~Perception()
    {

    }

    template<typename U>
    void train(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
    if(m_w.size() <= 0)
    {
        return;
    }

    int cur_iter = 0; 
    while(cur_iter < m_max_iter)
    {
        cur_iter = cur_iter + 1; 

        for(int i=0; i<feature.size(); ++i)
        {

            float result = 0; 
            for(int j=0; j<feature[i].size(); ++j)
            {
                result += m_w[j] * feature[i][j]; 
            }
            result += m_b; 

            result = -1 * label[i] * result; 

            if(result >= 0)
            {
                for(int j=0; j<feature[i].size(); ++j)
                {
                    m_w[j] = m_w[j] + m_learing_rate * feature[i][j] * label[i]; 
                }
                m_b = m_b + m_learing_rate * label[i]; 
            }
        }
    }

    }

    template<typename U>
    const T predict(const std::vector<U>& sample)
    {
        assert(sample.size() == m_w.size()); 

        T result = 0; 
        for(int i=0; i<sample.size();++i)
        {
            result += sample[i] * m_w[i]; 
        }
        result += m_b; 

        return result; 
    }

    template <typename U>
    const float test(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
        int total = feature.size(); 
        int accurate = 0; 
        for(int i=0; i<feature.size();++i)
        {
            T r = predict(feature[i]); 
            if(r * label[i] > 0)
            {
                accurate += 1; 
            }
        }
        return float(1.0*accurate/total); 
    }

    template <typename U>
    void setTargetSize(const std::vector<std::vector<U>>& feature)
    {
    int len = feature[0].size(); 
    m_w = std::vector<T>(len, 0); 
    }

    void setMaxIter(const size_t& v) { m_max_iter = v; } 

    void setLearningStep(const float& v) { m_learing_rate = v; }

private:
    std::vector<T> m_w; 
    T m_b; 
    size_t sample_size; 

    // parameter 
    size_t m_max_iter; 
    float m_learing_rate; 
}; 


typedef Perception<float> PerceptionMachine; 


#endif // _PERCEPTRON_H_  
