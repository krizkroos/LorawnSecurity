#include "jsonparser.h"
#include <iostream>

JsonParser::JsonParser()
{

}
//jParser.parse("{\"Node\":\"9478149a08f9\",\"Address\":\"172.17.0.2\",\"ServiceID\":\"HSS\",\"ServiceName\":\"HSS\",\"ServiceTags\":[],\"ServiceAddress\":\"\",\"ServicePort\":6666,\"ServiceEnableTagOverride\":false,\"CreateIndex\":2855,\"ModifyIndex\":2855}");

void JsonParser::parse(std::string json)
{
    map.Parse(json.c_str());
}

Lorawan_result JsonParser::getValue(std::vector<std::string> key, std::string &value)
{

    if(key.size() > 2 || key.size() < 1)
        return Lorawan_result::NotSupportedFeature;

    value.clear(); // empty the value
    std::string currentKey = key.at(0);
    if(map.IsObject()) // in json string = {"key" : "value" }
    {
        std::cout << "object json type" <<std::endl;

        if(!map.HasMember(currentKey.c_str()))
        {
            std::cout << "no member with root key: " << key.at(0)<< std::endl;
            return Lorawan_result::NoValueAvailable;
        }

        if(key.size() == 1)
        {

            if(!map[currentKey.c_str()].IsString())
            {
               std::cout << "not a string but provided only one key" <<std::endl;
               return Lorawan_result::NoValueAvailable;
            }

           value = std::string(map[currentKey.c_str()].GetString());
           return Lorawan_result::Success;

        }
        else if(key.size() == 2)
        {
            if(!map[currentKey.c_str()].IsArray())
            {
               std::cout << "recurency not available" <<std::endl;
               return Lorawan_result::NoValueAvailable;
            }

            std::string rootKey = key.at(0);
            currentKey = key.at(1);
            return getValueFromArrayWithKey(rootKey,currentKey,value);

        }
    }
    else if(map.IsArray()) // in json string =  [{"key" : "value" }] //TODO support [{"key" :[ "key2" : "value" ]}]
    {
        std::cout << "array json type" <<std::endl;
        bool foundValue = false;
        for(Value::ConstValueIterator itr = map.Begin(); itr != map.End(); ++itr)
        {
            if(foundValue)
                break;

            const Value& obj = *itr;
            for(Value::ConstMemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
            {
                if(it->name.GetString() == currentKey.c_str())
                {
                    std::cout << "found value for name: " << it->name.GetString() << std::endl;
                    value = std::string(it->value.GetString());
                    foundValue = true;
                    break;
                }
            }
        }

        if(!value.empty())
            return Lorawan_result::Success;
        else
            return Lorawan_result::NoValueAvailable;

    }
    else
    {
        std::cout << "unsupported json type" <<std::endl;
        return Lorawan_result::NoValueAvailable;
    }

  return Lorawan_result::Error;
}

Lorawan_result JsonParser::getValueFromArrayWithKey(std::string root, std::string key, std::string &value)
{
    if(!map[root.c_str()].IsArray())
    {
        std::cout << "not an array type" <<std::endl;
        return Lorawan_result::NoValueAvailable;
    }
    std::cout << "looking for " << key << "in an array " << root <<std::endl;
    bool foundValue = false;
    for(Value::ConstValueIterator itr = map[root.c_str()].Begin(); itr != map[root.c_str()].End(); ++itr)
    {
        if(foundValue)
            break;

        const Value& obj = *itr;
        for(Value::ConstMemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
        {
            //std::cout << "next key " <<  it->name.GetString()  << " " << key << std::endl;

            if(it->name.GetString() == key)
            {
                std::cout << "found value for name: " << it->name.GetString() << std::endl;
                value = std::string(it->value.GetString());
                foundValue = true;
                break;
            }
        }
    }
    if(!value.empty())
        return Lorawan_result::Success;
    else
        return Lorawan_result::NoValueAvailable;
}


Lorawan_result JsonParser::getValue(std::vector<std::string> key, int &value)
{
    std::string currentKey = key.at(0);
    if(!map.HasMember(currentKey.c_str()))
        return Lorawan_result::NoValueAvailable;

    if(map[currentKey.c_str()].IsInt())
    {
        value = map[currentKey.c_str()].GetInt();
        return Lorawan_result::Success;
    }
    else
        return Lorawan_result::NoValueAvailable;

}

Lorawan_result getValueFromArrayType(std::string key, int &value)
{
    return Lorawan_result::Success;
}
