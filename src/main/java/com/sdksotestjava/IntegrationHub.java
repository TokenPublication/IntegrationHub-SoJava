package com.sdksotestjava;


public class IntegrationHub {
    public long nativePointer = 0;

    static {
        System.loadLibrary("IntegrationHubJNI");
    }

    public native IntegrationHub createCommunication(String companyName);
    public native void deleteCommunication();
    public native void reconnect();
    public native int getActiveDeviceIndex();
    public native int sendBasket(String jsonData);
    public native int sendPayment(String jsonData);
    public native String getFiscalInfo();
    public native void setSerialInCallback();
    public native void setDeviceStateCallback();
}