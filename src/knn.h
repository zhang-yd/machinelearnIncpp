
#ifndef _KNN_H_ 
#define _KNN_H_ 

#include <iostream> 
#include <vector> 
#include <string>  
#include <cmath> 
#include <cassert> 
#include <algorithm> 


template<typename T>
struct Node
{
    Node(T d, size_t i)
    {
        dist = d; idx = i; 
    }
    T dist; 
    size_t idx; 
}; 

template<typename T>
bool NodeCmp(Node<T> a, Node<T> b)
{
    return a.dist < b.dist; 
}


template<typename T>
class KNN 
{

public:
    KNN()
    {
        m_count = 0; 
        m_feature_size = 0; 
        m_top_k = 0; 
    }
    ~KNN()
    {

    }

    void setTopNum(const size_t& v)
    {
        m_top_k = v; 
    }
    
    template<typename U>
    void train(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
        m_count = feature.size(); 
        if(m_count == 0)
        {
            return; 
        }
        m_feature_size = feature[0].size();  

        m_feature.clear(); 
        for(int i=0; i<feature.size(); ++i)
        {
            std::vector<T> tmp; 
            for(int j=0; j<feature[i].size(); ++j)
            {
                tmp.push_back(static_cast<T>(feature[i][j])); 
            }
            m_feature.push_back(tmp); 
        }

        m_label.clear(); 
        for(int i=0; i<label.size();++i)
        {
            m_label.push_back(static_cast<T>(label[i])); 
        }
        return; 
    }

    template<typename U>
    float test(const std::vector<std::vector<U>>& feature, const std::vector<U>& label)
    {
        int acc = 0, total = feature.size(); 

        for(int i=0; i<feature.size(); ++i)
        {
            size_t result = predict(feature[i]); 
            size_t lab = static_cast<size_t>(label[i]); 
            if(result == lab)
            {
                acc += 1; 
            }
        }
        return static_cast<float>((1.0 * acc) / (1.0 * total)); 
    }


    template<typename U>
    size_t predict(const std::vector<U>& sample)
    {
        T val = 0; 
        std::vector<Node<T>> distList; 
        for(int i=0; i<m_feature.size(); ++i)
        {
            T dist = calcDist(m_feature[i], sample); 
            distList.push_back(Node<T>(dist, i)); 
        }
        sort(distList.begin(), distList.end(), NodeCmp<T>);

        std::vector<int> IndexMap(11, 0); 
        size_t topK = min(m_top_k, distList.size()); 
        for(int i=0; i<topK; ++i)
        {
            int idx = static_cast<int>(m_label[distList[i].idx]); 
            IndexMap[idx] += 1;  
        }

        size_t targetIdx = 1; int curCnt = IndexMap[1]; 
        for(int i=2; i<=10; ++i)
        {
            if(curCnt < IndexMap[i])
            {
                targetIdx = i; 
                curCnt = IndexMap[i]; 
            }
        }
        return targetIdx; 
    }


private:
    template<typename U, typename W>
    T calcDist(const std::vector<U>& a, const std::vector<W>& b) const 
    {
        assert(a.size() == b.size()); 
        T dist = 0; 
        for(int i=0; i<a.size(); ++i)
        {
            dist += static_cast<T>(a[i] - b[i])*(a[i] - b[i]); 
        }
        return static_cast<T>(sqrt(dist)); 
    }


private:
    size_t m_count; 
    size_t m_feature_size; 
    size_t m_top_k; 

    std::vector<std::vector<T>> m_feature; 
    std::vector<T> m_label; 
}; 

#endif // _KNN_H_ 
