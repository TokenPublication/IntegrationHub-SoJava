/**
 * @file IntegrationHubTest.java
 * @brief Test implementation for TokenX Connect (Wired) integration
 * 
 * This class provides test methods and callback handlers for demonstrating the capabilities
 * of the TokenX Connect SDK with Token Financial Technologies X30TR and 300TR POS devices.
 * It implements static callback methods referenced by the JNI layer and contains test methods
 * that show how to interact with different device types.
 * 
 * Platform: Java test interface for TokenX Connect SDK on MacOS/Linux
 */
package com.sdksotestjava;

public class IntegrationHubTest {
    /**
     * Callback handler for serial communication from POS device
     * Called by the JNI layer when serial data is received from the device
     * 
     * @param tag An integer tag identifying the message type or source
     * @param data String data received from the device
     */
    public static void serialInCallback(int tag, String data) {
        System.out.println("Serial In Callback Result In Test:\n" + data);
    }

    /**
     * Callback handler for device connection state changes
     * Called by the JNI layer when device connection state changes
     * 
     * @param state Boolean indicating if the device is connected (true) or disconnected (false)
     * @param deviceId String identifying the connected device
     */
    public static void deviceStateCallback(boolean state, String deviceId) {
        System.out.println("Device State Callback Result In Test:\n" + deviceId);
    }

    /**
     * Test method for sending basket information to different POS device types
     * Demonstrates the different JSON formats required for X30TR vs 300TR devices
     * 
     * @param communication IntegrationHub instance to use for communication
     * @param activeDevice Device index (0 for X30TR, 1 for 300TR)
     */
    public static void sendBasketTest(IntegrationHub communication, int activeDevice) {
        if (activeDevice == 0) { // X30TR
            int basketResult = communication.sendBasket("{"
                    + "\"basketID\": \"a123ca24-ca2c-401c-8134-f0de2ec25c25\","
                    + "\"documentType\": 9008,"
                    + "\"customerInfo\": { \"taxID\": \"11111111111\" },"
                    + "\"items\": ["
                    + "{ \"name\": \"İLAÇ\", \"price\": 1000, \"quantity\": 1000, \"sectionNo\": 2, \"taxPercent\": 2000, \"type\": 0 }"
                    + "],"
                    + "\"taxFreeAmount\": 5000,"
                    + "\"paymentItems\": ["
                    + "{ \"amount\": 6000, \"description\": \"Cash\", \"type\": 1 }"
                    + "]"
                    + "}");
            System.out.println("basketResult: " + basketResult);
        } else if (activeDevice == 1) { // 300TR
            int basketResult = communication.sendBasket("{"
                    + "\"basketID\": \"a123ca24-ca2c-401c-8134-f0de2ec25c25\","
                    + "\"documentType\": 0,"
                    + "\"customerInfo\": { \"taxID\": \"11111111111\" },"
                    + "\"items\": ["
                    + "{ \"name\": \"İLAÇ\", \"price\": 1000, \"quantity\": 1000, \"sectionNo\": 1, \"taxPercent\": 1000, \"type\": 0 }"
                    + "],"
                    + "\"taxFreeAmount\": 5000"
                    + "}");
            System.out.println("basketResult: " + basketResult);
        }
    }

    /**
     * Test method for sending payment information to the POS device
     * Currently implemented only for 300TR device type
     * 
     * @param communication IntegrationHub instance to use for communication
     * @param activeDevice Device index (0 for X30TR, 1 for 300TR)
     */
    public static void sendPaymentTest(IntegrationHub communication, int activeDevice) {
        if (activeDevice == 1) { // 300TR
            int paymentResult = communication.sendPayment("{\"amount\":6000,\"description\":\"Nakit\",\"type\":1}");
            System.out.println("paymentResult: " + paymentResult);
        }
    }

    /**
     * Test method for retrieving fiscal information from the POS device
     * Works with both device types
     * 
     * @param communication IntegrationHub instance to use for communication
     */
    public static void getFiscalInfoTest(IntegrationHub communication) {
        String fiscalInfo = communication.getFiscalInfo();
        System.out.println("Get Fiscal Info Result In Test:\n" + fiscalInfo);
    }
}
