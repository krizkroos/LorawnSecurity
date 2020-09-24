# LoRaWAN Security

This repository is used for testing possible vector attacks for [LoRaWAN](https://lora-alliance.org/about-lorawan) protocol which is a popular choice for communication between Internet of Things devices.

Performed attacks are based on packets that are collected from the interface of LoRaWAN gateway. The are intercepted on UDP port. In the lab the test was performed directly on the gateway but it can be launched in the same LAN network as the gateway. 


Generally, **LoRaWAN Security** library enables you to prepare valid LoRaWAN packet (calculate MIC based on used key, add packets data in base64 coding, put them in JSON object (as for TTN servers)).

## External Used libraries

Below there is a list of used external software/ libraries:

[RapidJSON](https://rapidjson.org/) - working with JSON data (updating, serializing)

[LIBTINS](http://libtins.github.io/) - sniffing interface and deserializing packets

[OpenSSL](https://www.openssl.org/) - base64, AES-CMAC mechanisms

[Qt](https://www.qt.io/) - building with qmake


## How to Use :fire:

When building library, you need to specify macros in config.pri. They contain info where are stored objects from OpenSSL library, your SDK folder or destination folder for building remotely onto your LoRaWAN gateway.

### Library requires two interfaces to implement: 
* TestPrerequisite 
* Test

LibrarySecurity is the main class where you are adding required steps (prerequisites) and Test instance to perform.

The MiTMAttack prerequisite allows you to intercept packets based on sniffed results with LibTINS library. As specified in the TestPrerequisite interface we begin with `start()` method.
In there is called the constructor for `Tins::Sniffer` object. We are launching sniffing by calling the `sniff_loop()`. It requires the static function which returns bool value (when returns `true` it means that we are still sniffing). In the MiTMAttack class we can sniffed more than one type of wanted packet. The line that checks if we have all needed packets from specified vector is shown below:
```cpp
    if(MiTMAttack::_nextWantedPacket != MiTMAttack::_wantedPacket.end())
```
Attacks are based on previous intercepted packets. They are stored in the singleton class `PacketStorage`. This class has three overloads for method `addPacket()` to add each specified LoRaWAN packet into the storage. Possible LoRaWAN packets'type are:
- JoinAccept
- JoinRequest
- MACPayload (Uplink and Downlink)

When the prerequisite for the attack is finished, and we have packets in the Storage, we can launch main class `Test`. At this point there are three possible vectors to test with LoRaWAN devices:

* Frame Counter Mechanism Reply Attak with uplink packets
* Denial of Service with JoinRequest packets
* Battery depletion with downlink packets

Before we create a new packet based on captured one, we must call a method `deserialize()` and afterwards before sending it we should launch `serialize()` method.

During the test you can use `SendController` class to send packets in specified port of network LoRaWAN server you are testing.

Library can be extended with new attack vectors. To add new attacks you have to provide all necessary actions in TestPrerequisite interface before the test. You can of course use the MiTMAttack prerequisite. 

## Examples
To perform f.e. DoS test with JoinRequest packet, all you need to do, is to define `LorawanTester` object and then prepare parameters specified for you environment. The example for this attack is shown below:
```cpp
    LorawanTester tester;
    
    TestParams paramsDoSRequest;
    paramsDoSRequest.setLogLevel(Logger::JSON | Logger::RawData | Logger::MiTM | Logger::LorawanTest | Logger::Common | Logger::RequestDoS);
    paramsDoSRequest.setLogFileName("logs-req/test-request-" + Common::getTime() + ".log");
    paramsDoSRequest.setInterfaceName("wlan0"); //your interface for LAN network
    paramsDoSRequest.setFilter("udp dst port 1700"); //filter UDP packets with LoRaWAN data inside
    paramsDoSRequest.setRequestCounter(240);  //how many packets send to NS server
    paramsDoSRequest.setAppKey(bytes({0x00})); // put required AppKey here
    
    if(tester.testDoSRequest(paramsDoSRequest) != Lorawan_result::Success)
    {
        std::cout << "testing DoS of JoinRequest packet failed" <<std::endl;
    }
```
