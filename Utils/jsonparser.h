#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "defineLorawan.h"

using namespace rapidjson;

class JsonParser
{
private:
    Document map;
public:
    JsonParser();
    void parse(std::string json);
    Lorawan_result getValue(std::vector<std::string> key, std::string &value);
    Lorawan_result getValue(std::vector<std::string> key, int &value);
    Lorawan_result getValueFromArrayWithKey(std::string root, std::string key, std::string &value);
    Lorawan_result getValueFromArrayWithKey(std::string key, int &value);
};

#endif // JSONPARSER_H
