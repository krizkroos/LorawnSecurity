#include "jsonparser.h"


#include <iostream>
using namespace rapidjson;
JsonParser::JsonParser()
{

}

std::string JsonParser::getJson()
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    map.Accept(writer);

    return std::string(buffer.GetString());
}
//easier test
//jParser.parse("{\"Node\":\"9478149a08f9\",\"Address\":\"172.17.0.2\",\"ServiceID\":\"HSS\",\"ServiceName\":\"HSS\",\"ServiceTags\":[],\"ServiceAddress\":\"\",\"ServicePort\":6666,\"ServiceEnableTagOverride\":false,\"CreateIndex\":2855,\"ModifyIndex\":2855}");

//final test
//"{\"rxpk\":[{\"tmst\":2536840083,\"time\":\"2020-07-10T09:29:36.255849Z\",\"chan\":1,\"rfch\":1,\"freq\":868.300000,\"stat\":1,\"modu\":\"LORA\",\"datr\":\"SF7BW125\",\"codr\":\"4/5\",\"lsnr\":8.5,\"rssi\":-31,\"size\":23,\"data\":\"AKbtAtB+1bNwgdYfAAujBACIJ6iTMRQ=\"}]}";


Lorawan_result JsonParser::parse(std::string json)
{
    if(map.Parse(json.c_str()).HasParseError())
        return Lorawan_result::ErrorParsing;

    return Lorawan_result::Success;
}

Lorawan_result JsonParser::changeValue(jsonKeys key, const std::string value)
{

    if(key.size() > 2 || key.size() < 1)
        return Lorawan_result::NotSupportedFeature;
    std::cout << "value to input: " << value << std::endl;
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

            map[currentKey.c_str()].SetString(value.c_str(), static_cast<SizeType>(value.size()));
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
            return changeValueInArray(rootKey,currentKey,value);

        }
    }
    else if(map.IsArray()) // in json string =  [{"key" : "value" }] //TODO support [{"key" :[ "key2" : "value" ]}]
    {
        std::cout << "array json type" <<std::endl;
        bool foundValue = false;
        for(Value::ValueIterator itr = map.Begin(); itr != map.End(); ++itr)
        {
            if(foundValue)
                break;
            Document::AllocatorType& allocator = map.GetAllocator();
            Value& obj = *itr;
            for(Value::MemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
            {
                if(it->name.GetString() == currentKey.c_str())
                {
                    std::cout << "changing value for " << it->name.GetString() << std::endl;
                    Value input(value.c_str(),allocator);

                    return Lorawan_result::Success;
                }
            }
        }

    }
    else
    {
        std::cout << "unsupported json type" <<std::endl;
        return Lorawan_result::NoValueAvailable;
    }

  return Lorawan_result::Error;

}

Lorawan_result JsonParser::changeValueInArray(std::string root, std::string key, const std::string value)
{

    std::cout << "value to change in array: " << value << std::endl;
    if(!map[root.c_str()].IsArray())
    {
        std::cout << "not an array type" <<std::endl;
        return Lorawan_result::NoValueAvailable;
    }
    std::cout << "looking for " << key << "in an array " << root <<std::endl;
    bool foundValue = false;
    for(Value::ValueIterator itr = map[root.c_str()].Begin(); itr != map[root.c_str()].End(); ++itr)
    {
        if(foundValue)
            break;

        Value& obj = *itr;
        Document::AllocatorType& allocator = map.GetAllocator();
        for(Value::MemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
        {
            //std::cout << "next key " <<  it->name.GetString()  << " " << key << std::endl;

            if(it->name.GetString() == key)
            {
                    if(!it->value.IsString())
                    {
                        std::cout << "not a string but provided only one key" <<std::endl;
                        return Lorawan_result::NoValueAvailable;
                    }

                    it->value.SetString(value.c_str(),allocator);
                    return Lorawan_result::Success;

            }
        }
    }
    return Lorawan_result::Error;
}

Lorawan_result JsonParser::changeValue(std::vector<std::string> key, int value)
{

    return Lorawan_result::NotSupportedFeature;
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
    UNUSED(key);
    UNUSED(value);
    return Lorawan_result::Success;
}
