# LoRaWAN Security

This repository is used for testing possible vector attacks for [LoRaWAN](https://lora-alliance.org/about-lorawan) protocol which is a popular choice for communication between Internet of Things devices.
Performed attacks are based on packets that are collected from the interface of LoRaWAN gateway. The are intercepted on commonly known UDP port number 1700 (which is default in many LoRaWAN systems).

Library enables you to prepare valid LoRaWAN packet (calculate MIC based on used key, add packets data in base64 coding, put them in JSON object (as for TTN servers)).

## Used libraries

Below there is a list of used external software:

[RapidJSON](https://rapidjson.org/)

[LIBTINS](http://libtins.github.io/)

[OpenSSL](https://www.openssl.org/)

[Qt](https://www.qt.io/)


## How to use the Lorawan Security Library

Library provides two interfaces: Test prerequisite and actual testing

It can be widden with new attack vectors. Two add new attacks you have to provide all necessary actions in TestPrerequisite interface before the test. During the test you can use SendController class to send packets in specified port of Network LoRaWAN server you are testing.

