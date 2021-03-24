

#ifndef _CONSTSTRING_H_ 
#define _CONSTSTRING_H_ 

#include <string> 
#include <map> 
#include <vector> 

#include "mutex.h" 


class ConstString; 


class ConstStringTable
{
public:
    ConstStringTable()
    { 
    }

    ~ConstStringTable()
    {
        for(unsigned int i =0; i<m_string.size(); ++i)
        {
            delete[] m_string[i]; 
        }
    }

    int GetIndex(const char* name)
    {
        int result = 0; 
        m_lock.LockRead(); 
        StringMap::const_iterator iter = m_map.find(name); 
        if(iter == m_map.end())
        {
            m_lock.UnLockRead(); 
            m_lock.LockWrite(); 

            iter = m_map.find(name); 
            if(iter == m_map.end())
            {
                char *str = new char[strlen(name) + 1]; 
                strcpy(str, name); 
                m_string.push_back(str); 
                result = (int)m_string.size() - 1; 
                m_map[name] = result;
            }
            else
            {
                result = iter->second;             
            }
            m_lock.UnLockWrite(); 
        }
        else
        {
            result = iter->second; 
            m_lock.UnLockRead(); 
        }
        return result; 
    }

    const char* GetString(int idx) const 
    {
        return m_string[idx];
    }

    const std::size_t GetSize() const 
    {
        return m_string.size(); 
    }

private:
    std::vector<char*> m_string; 
    typedef std::map<std::string, int> StringMap; 
    StringMap m_map; 
    ReadWriteLock m_lock; 
}; 


class ConstString
{
public:
    ConstString(): m_table(NULL), m_idx(-1), m_str(NULL)
    { 
    }

    ConstString(ConstStringTable &table, int idx): m_table(&table), m_idx(idx), m_str(table.GetString(idx))
    {
    }

    ConstString(ConstStringTable &table, const char *str): m_table(&table) 
    {
        m_idx = table.GetIndex(str); 
        m_str = table.GetString(m_idx); 
    }

    const char* c_str() const 
    {
        return m_str; 
    }

    bool operator==(const ConstString& other) const 
    {
        return (m_table == other.m_table && m_idx == other.m_idx); 
    }

    bool operator <(const ConstString& other) const 
    {
        return (m_table < other.m_table || (m_table == other.m_table && m_idx < other.m_idx)); 
    }

private:
    ConstStringTable *m_table; 
    int m_idx; 
    const char *m_str; 
}; 




#endif // _CONSTSTRING_H_ 
