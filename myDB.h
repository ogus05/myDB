#include <iostream>
#include <string>
#include <unordered_map>

#include "mnvme_kvs.h"

class MyStatus{
public:
    enum Stat{
        OK = 0,
        NotFound = 1,
        ERR = 2,
    };
    bool IsNotFound();
    bool ok();
    std::string ToString();

    MyStatus(Stat stat);
private:
    Stat stat;
};

class MyDB{
    typedef std::string KeyType;
    typedef std::string ValueType;
private:
    std::unordered_map<KeyType, ValueType>* data;

    FILE* log;

    MNVME* mnvme;

public:
    MyDB();
    ~MyDB();
    MyStatus Get(const std::string& key, std::string* retValue);
    MyStatus Delete(const std::string& key);
    MyStatus Put(const std::string& key, std::string& putValue);
    
};
