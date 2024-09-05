#include "myDB.h"
#include <libnvme.h>

bool MyStatus::IsNotFound()
{
    return (this->stat == Stat::NotFound);
}

bool MyStatus::ok()
{
    return (this->stat == Stat::OK);
}

std::string MyStatus::ToString()
{
    std::string retValue = "";

    switch(this->stat){
        case Stat::OK:{
            retValue = "OK";
        } break;
        case Stat::NotFound:{
            retValue = "Not Found";
        } break;
        case Stat::ERR:{
            retValue = "Error";
        } break;

        default:{
            throw new std::invalid_argument("Error in toString");
        }
    }

    return retValue;
}

MyStatus::MyStatus(Stat stat)
{
    this->stat = stat;
}

MyDB::MyDB()
{
    this->data = new std::unordered_map<KeyType, ValueType>();
}

MyDB::~MyDB()
{
    this->data->clear();
    delete this->data;
}

MyStatus MyDB::Get(const std::string &key, std::string *retValue)
{
    retValue = nullptr;
    int status = mnvme->mnvme_kvs_exists(key);
    if(status != 0){
        fprintf(log, "err in MyDB::Get, %s, %d", key, status);
        return MyStatus::ERR;
    }
    
    auto dataItr = data->find(key);
    if(dataItr == data->end()){
        return MyStatus(MyStatus::Stat::NotFound);
    } else{
        
        retValue = &(dataItr->second);
    }
    return MyStatus(MyStatus::Stat::OK);
}

MyStatus MyDB::Delete(const std::string &key)
{
    auto dataItr = data->find(key);
    if(dataItr == data->end()){
        return MyStatus(MyStatus::Stat::NotFound);
    } else{
        data->erase(dataItr);
    }
    return MyStatus(MyStatus::Stat::OK);
}

MyStatus MyDB::Put(const std::string &key, std::string &putValue)
{
    auto dataItr = data->find(key);
    if(dataItr == data->end()){
        data->insert({key, putValue});
    } else{
        data->at(key) = putValue;
    }
    return MyStatus(MyStatus::Stat::OK);
}
