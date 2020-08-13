#include "jsonparser.h"
#include "logger.h"

using namespace rapidjson;

std::string JsonParser::getJson()
{
    called(Logger::JSON);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    map.Accept(writer);

    return std::string(buffer.GetString());
}
//easier test
//jParser.parse("{\"Node\":\"9478149a08f9\",\"Address\":\"172.17.0.2\",\"ServiceID\":\"HSS\",\"ServiceName\":\"HSS\",\"ServiceTags\":[],\"ServiceAddress\":\"\",\"ServicePort\":6666,\"ServiceEnableTagOverride\":false,\"CreateIndex\":2855,\"ModifyIndex\":2855}");

//final test
//"{\"rxpk\":[{\"tmst\":2536840083,\"time\":\"2020-07-10T09:29:36.255849Z\",\"chan\":1,\"rfch\":1,\"freq\":868.300000,\"stat\":1,\"modu\":\"LORA\",\"datr\":\"SF7BW125\",\"codr\":\"4/5\",\"lsnr\":8.5,\"rssi\":-31,\"size\":23,\"data\":\"AKbtAtB+1bNwgdYfAAujBACIJ6iTMRQ=\"}]}";

//
//"{\"txpk\":{\"imme\":false,\"tmst\":1292149667,\"freq\":868.1,\"rfch\":0,\"powe\":14,\"modu\":\"LORA\",\"datr\":\"SF7BW125\",\"codr\":\"4/5\",\"ipol\":true,\"size\":33,\"ncrc\":true,\"data\":\"IGdvKRC7I5CtcvV+tYi30VLgOlHHPGINKwBV8SHFuQrp\"}}"


Lorawan_result JsonParser::parse(std::string json)
{
    called(Logger::JSON);
    if(map.Parse(json.c_str()).HasParseError())
    {
        writeLog(Logger::JSON, "parsing has error: " + std::to_string(map.GetParseError()));
        return Lorawan_result::ErrorParsing;
    }
    return Lorawan_result::Success;
}

Lorawan_result JsonParser::changeValue(jsonKeys key, const std::string value)
{
    called(Logger::JSON);
    if(key.size() > 2 || key.size() < 1)
        return Lorawan_result::NotSupportedFeature;
    writeLog(Logger::JSON,"value to input: " + value);
    writeLog(Logger::JSON, "provided keys: ");
    for(auto k : key)
    {
        writeLog(Logger::JSON, k);
    }


    Document::AllocatorType& allocator = map.GetAllocator();
    std::string currentKey = key.at(0);
    if(map.IsObject()) // in json string = {"key" : "value" }
    {
        writeLog(Logger::JSON, "object json type");

        if(!map.HasMember(currentKey.c_str()))
        {
            writeLog(Logger::JSON,"no member with root key: " + currentKey);
            return Lorawan_result::NoValueAvailable;
        }

        if(key.size() == 1)
        {

            if(!map[currentKey.c_str()].IsString())
            {
                writeLog(Logger::JSON,"not a string but provided only one key");
                return Lorawan_result::NoValueAvailable;
            }

            map[currentKey.c_str()].SetString(value.c_str(),allocator);
            return Lorawan_result::Success;

        }
        else if(key.size() == 2)
        {
            std::string rootKey = key.at(0);
            currentKey = key.at(1);

            if(map[rootKey.c_str()].IsObject())
            {
                writeLog(Logger::JSON, "Inside Document is object type");

                if(!map[rootKey.c_str()][currentKey.c_str()].IsString())
                {
                    writeLog(Logger::JSON,"not a string value / two keys");
                    return Lorawan_result::NoValueAvailable;
                }

                map[rootKey.c_str()][currentKey.c_str()].SetString(value.c_str(), allocator);

                return Lorawan_result::Success;
            }
            else
            {
                writeLog(Logger::JSON,"Looking for value with provided two keys: " + rootKey + " " + currentKey);
                return changeValueInArray(rootKey,currentKey,value);
            }

        }
    }
    else if(map.IsArray()) // in json string =  [{"key" : "value" }] //TODO support [{"key" :[ "key2" : "value" ]}]
    {
        writeLog(Logger::JSON,"array json type");
        bool foundValue = false;
        for(Value::ValueIterator itr = map.Begin(); itr != map.End(); ++itr)
        {
            if(foundValue)
                break;

            Value& obj = *itr;
            for(Value::MemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
            {
                if(it->name.GetString() == currentKey.c_str())
                {
                    writeLog(Logger::JSON,"changing value for ");
                    Value input(value.c_str(),allocator);

                    return Lorawan_result::Success;
                }
            }
        }

    }
    else
    {
        writeLog(Logger::JSON,"unsupported json type");
        return Lorawan_result::NoValueAvailable;
    }

    return Lorawan_result::Error;

}

Lorawan_result JsonParser::changeValueInArray(std::string root, std::string key, const std::string value)
{
    called(Logger::JSON);
    writeLog(Logger::JSON,"value to change in array: ");
    Document::AllocatorType& allocator = map.GetAllocator();
    if(!map[root.c_str()].IsArray())
    {
        writeLog(Logger::JSON,"not an array type");
        return Lorawan_result::NoValueAvailable;
    }
    writeLog(Logger::JSON,"looking for " + key + "in an array " +root);
    bool foundValue = false;
    for(Value::ValueIterator itr = map[root.c_str()].Begin(); itr != map[root.c_str()].End(); ++itr)
    {
        if(foundValue)
            break;

        Value& obj = *itr;

        for(Value::MemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
        {
            //std::cout << "next key " <<  it->name.GetString()  << " " << key << std::endl;

            if(it->name.GetString() == key)
            {
                if(!it->value.IsString())
                {
                    writeLog(Logger::JSON,"not a string but provided only one key");
                    return Lorawan_result::NoValueAvailable;
                }

                it->value.SetString(value.c_str(),allocator);
                return Lorawan_result::Success;

            }
        }
    }
    return Lorawan_result::Error;
}

Lorawan_result JsonParser::changeValue(jsonKeys key, int value)
{
    called(Logger::JSON);
    UNUSED(key);
    UNUSED(value);
    return Lorawan_result::NotSupportedFeature;
}

Lorawan_result JsonParser::getValue(jsonKeys key, std::string &value)
{
    called(Logger::JSON);
    if(key.size() > 2 || key.size() < 1)
        return Lorawan_result::NotSupportedFeature;

    value.clear(); // empty the value
    std::string currentKey = key.at(0);
    if(map.IsObject()) // in json string = {"key" : "value" }
    {
        writeLog(Logger::JSON, "object json type");

        if(!map.HasMember(currentKey.c_str()))
        {
            writeLog(Logger::JSON,"no member with root key: ");
            return Lorawan_result::NoValueAvailable;
        }

        if(key.size() == 1)
        {

            if(!map[currentKey.c_str()].IsString())
            {
                writeLog(Logger::JSON,"not a string value but provided only one key");
                return Lorawan_result::NoValueAvailable;
            }

            value = std::string(map[currentKey.c_str()].GetString());
            return Lorawan_result::Success;

        }
        else if(key.size() == 2)
        {
            std::string rootKey = key.at(0);
            currentKey = key.at(1);

            if(map[rootKey.c_str()].IsObject())
            {
                writeLog(Logger::JSON, "Inside Document is object type");

                if(!map[rootKey.c_str()][currentKey.c_str()].IsString())
                {
                    writeLog(Logger::JSON,"not a string value / two keys");
                    return Lorawan_result::NoValueAvailable;
                }
                value = std::string(map[rootKey.c_str()][currentKey.c_str()].GetString());
                return Lorawan_result::Success;
            }

            return getValueFromArrayWithKey(rootKey,currentKey,value);

        }
    }
    else if(map.IsArray()) // in json string =  [{"key" : "value" }] //TODO support [{"key" :[ "key2" : "value" ]}]
    {
        writeLog(Logger::JSON,"array json type");
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
                    writeLog(Logger::JSON,"found value for name: " + std::string(it->name.GetString()));
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
        writeLog(Logger::JSON,"unsupported json type" + std::to_string(map.GetType()));
        return Lorawan_result::NoValueAvailable;
    }

    return Lorawan_result::Error;
}

Lorawan_result JsonParser::getValueFromArrayWithKey(std::string root, std::string key, std::string &value)
{
    called(Logger::JSON);
    if(!map[root.c_str()].IsArray())
    {
        writeLog(Logger::JSON,"not an array type");
        return Lorawan_result::NoValueAvailable;
    }
    writeLog(Logger::JSON,"looking for " + key + "in an array " +  root);
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
                writeLog(Logger::JSON,"found value for name: " + std::string(it->name.GetString()));
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


Lorawan_result JsonParser::getValue(jsonKeys key, int &value)
{
    called(Logger::JSON);
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
    return Lorawan_result::NotSupportedFeature;
}
