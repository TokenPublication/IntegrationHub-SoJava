/**
 * @file IntegrationHubWrapper.h
 * @brief Header file defining the C API interface for TokenX Connect (Wired) integration
 * 
 * This header defines the interface for communicating with Token Financial Technologies
 * X30TR and 300TR POS devices through a wired connection. It provides function declarations
 * for device communication, transaction handling, and callback registration.
 * 
 * Platform: This wrapper is designed for use in JNI (Java Native Interface) implementations
 * to enable Java applications to interface with the native C/C++ TokenX Connect SDK.
 */

#pragma once
#include <string>

/**
 * @typedef ConnectionWrapper
 * @brief Opaque pointer type for the connection object that maintains device communication
 */
typedef void ConnectionWrapper;

/**
 * @typedef SerialInCallback
 * @brief Callback function type for handling serial input from the device
 * @param tag An integer tag identifying the message type or source
 * @param data String data received from the device
 */
typedef void(*SerialInCallback)(int, std::string);

/**
 * @typedef DeviceStateCallback
 * @brief Callback function type for monitoring device connection state
 * @param state Boolean indicating if the device is connected (true) or disconnected (false)
 * @param deviceId String identifying the connected device
 */
typedef void(*DeviceStateCallback)(bool, std::string);

/**
 * @brief Creates a communication object for connecting to POS devices
 * @param companyName The name of the company using the integration
 * @return A pointer to a newly created ConnectionWrapper object
 */
extern "C" ConnectionWrapper* createCommunication(std::string companyName);

/**
 * @brief Destroys a previously created communication object
 * @param ptr Pointer to the ConnectionWrapper object to delete
 */
extern "C" void deleteCommunication(ConnectionWrapper* ptr);

/**
 * @brief Attempts to reconnect to the POS device
 * @param ptr Pointer to the ConnectionWrapper object
 */
extern "C" void reconnect(ConnectionWrapper* ptr);

/**
 * @brief Gets the index of the currently active device
 * @param ptr Pointer to the ConnectionWrapper object
 * @return 0 for X30TR device, 1 for 300TR device, or a negative value if no device is active
 */
extern "C" int getActiveDeviceIndex(ConnectionWrapper* ptr);

/**
 * @brief Sends basket information to the POS device
 * @param ptr Pointer to the ConnectionWrapper object
 * @param jsonData JSON string containing basket data (items, prices, taxes, etc.)
 * @return Status code indicating success (0) or specific error codes
 */
extern "C" int sendBasket(ConnectionWrapper* ptr, std::string jsonData);

/**
 * @brief Sends payment information to the POS device
 * @param ptr Pointer to the ConnectionWrapper object
 * @param jsonData JSON string containing payment data (amount, type, etc.)
 * @return Status code indicating success (0) or specific error codes
 */
extern "C" int sendPayment(ConnectionWrapper* ptr, std::string jsonData);

/**
 * @brief Retrieves fiscal information from the POS device
 * @param ptr Pointer to the ConnectionWrapper object
 * @return JSON string containing the fiscal information
 */
extern "C" std::string getFiscalInfo(ConnectionWrapper* ptr);

/**
 * @brief Registers a callback function for serial input events
 * @param ptr Pointer to the ConnectionWrapper object
 * @param callback Function to be called when serial data is received
 */
extern "C" void setSerialInCallback(ConnectionWrapper* ptr, SerialInCallback callback);

/**
 * @brief Registers a callback function for device state change events
 * @param ptr Pointer to the ConnectionWrapper object
 * @param callback Function to be called when device connection state changes
 */
extern "C" void setDeviceStateCallback(ConnectionWrapper* ptr, DeviceStateCallback callback);
