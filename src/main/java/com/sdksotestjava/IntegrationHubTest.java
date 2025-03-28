package com.sdksotestjava;

public class IntegrationHubTest {
    public static void serialInCallback(int tag, String data) {
        System.out.println("Serial In Callback Result In Test:\n" + data);
    }

    public static void deviceStateCallback(boolean state, String deviceId) {
        System.out.println("Device State Callback Result In Test:\n" + deviceId);
    }

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

    public static void sendPaymentTest(IntegrationHub communication, int activeDevice) {
        if (activeDevice == 1) { // 300TR
            int paymentResult = communication.sendPayment("{\"amount\":6000,\"description\":\"Nakit\",\"type\":1}");
            System.out.println("paymentResult: " + paymentResult);
        }
    }

    public static void getFiscalInfoTest(IntegrationHub communication) {
        String fiscalInfo = communication.getFiscalInfo();
        System.out.println("Get Fiscal Info Result In Test:\n" + fiscalInfo);
    }
}
