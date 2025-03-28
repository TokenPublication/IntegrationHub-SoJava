/**
 * @file IntegrationHub.java
 * @brief Java interface for TokenX Connect (Wired) integration
 * 
 * This class provides the Java interface for interacting with Token Financial Technologies
 * X30TR and 300TR POS devices through a wired connection. It uses JNI to communicate with
 * the native C/C++ implementation of the TokenX Connect SDK.
 * 
 * Platform: Java application interface for TokenX Connect SDK on MacOS/Linux
 */
package com.sdksotestjava;


public class IntegrationHub {
    /**
     * Native pointer used to reference the C++ ConnectionWrapper object
     * This value is set by the native createCommunication method
     */
    public long nativePointer = 0;

    static {
        // Load the JNI library that implements the native methods
        System.loadLibrary("IntegrationHubJNI");
    }

    /**
     * Creates a communication object for connecting to POS devices
     * @param companyName The name of the company using the integration
     * @return An initialized IntegrationHub object
     */
    public native IntegrationHub createCommunication(String companyName);
    
    /**
     * Destroys the native communication object
     * Should be called when finished using the IntegrationHub to free resources
     */
    public native void deleteCommunication();
    
    /**
     * Attempts to reconnect to the POS device
     * Useful when a connection is lost or needs to be re-established
     */
    public native void reconnect();
    
    /**
     * Gets the index of the currently active device
     * @return 0 for X30TR device, 1 for 300TR device, or a negative value if no device is active
     */
    public native int getActiveDeviceIndex();
    
    /**
     * Sends basket information to the POS device
     * @param jsonData JSON string containing basket data (items, prices, taxes, etc.)
     * @return Status code indicating success (0) or specific error codes
     */
    public native int sendBasket(String jsonData);
    
    /**
     * Sends payment information to the POS device
     * @param jsonData JSON string containing payment data (amount, type, etc.)
     * @return Status code indicating success (0) or specific error codes
     */
    public native int sendPayment(String jsonData);
    
    /**
     * Retrieves fiscal information from the POS device
     * @return JSON string containing the fiscal information
     */
    public native String getFiscalInfo();
    
    /**
     * Registers a callback function for serial input events
     * Callbacks will be delivered to the static serialInCallback method in IntegrationHubTest
     */
    public native void setSerialInCallback();
    
    /**
     * Registers a callback function for device state change events
     * Callbacks will be delivered to the static deviceStateCallback method in IntegrationHubTest
     */
    public native void setDeviceStateCallback();
}