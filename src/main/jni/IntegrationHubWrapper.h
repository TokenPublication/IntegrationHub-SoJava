#pragma once
#include <string>

typedef void ConnectionWrapper;
typedef void(*SerialInCallback)(int, std::string);
typedef void(*DeviceStateCallback)(bool, std::string);

extern "C" ConnectionWrapper* createCommunication(std::string companyName);
extern "C" void deleteCommunication(ConnectionWrapper* ptr);
extern "C" void reconnect(ConnectionWrapper* ptr);
extern "C" int getActiveDeviceIndex(ConnectionWrapper* ptr);

extern "C" int sendBasket(ConnectionWrapper* ptr, std::string jsonData);
extern "C" int sendPayment(ConnectionWrapper* ptr, std::string jsonData);
extern "C" std::string getFiscalInfo(ConnectionWrapper* ptr);
extern "C" void setSerialInCallback(ConnectionWrapper* ptr, SerialInCallback callback);
extern "C" void setDeviceStateCallback(ConnectionWrapper* ptr, DeviceStateCallback callback);
