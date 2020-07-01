#include "jsonparser.h"

JsonParser::JsonParser()
{

}
//jParser.parse("{\"Node\":\"9478149a08f9\",\"Address\":\"172.17.0.2\",\"ServiceID\":\"HSS\",\"ServiceName\":\"HSS\",\"ServiceTags\":[],\"ServiceAddress\":\"\",\"ServicePort\":6666,\"ServiceEnableTagOverride\":false,\"CreateIndex\":2855,\"ModifyIndex\":2855}");
void JsonParser::parse(std::string json)
{
    map.Parse(json.c_str());
}

Lorawan_result JsonParser::getValue(std::string key, std::string &value)
{
    if(!map.HasMember(key.c_str()))
        return Lorawan_result::NoValueAvailable;

    if(map[key.c_str()].IsString())
    {
        value = std::string(map[key.c_str()].GetString());
        return Lorawan_result::Success;
    }
    else
        return Lorawan_result::NoValueAvailable;

}

Lorawan_result JsonParser::getValue(std::string key, int &value)
{
    if(!map.HasMember(key.c_str()))
        return Lorawan_result::NoValueAvailable;

    if(map[key.c_str()].IsInt())
    {
        value = map[key.c_str()].GetInt();
        return Lorawan_result::Success;
    }
    else
        return Lorawan_result::NoValueAvailable;

}
