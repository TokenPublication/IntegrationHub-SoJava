/**
 * @file IntegrationHubJNI.cpp
 * @brief JNI implementation for TokenX Connect (Wired) integration
 * 
 * This file implements the Java Native Interface (JNI) layer for communicating with 
 * Token Financial Technologies X30TR and 300TR POS devices through a wired connection.
 * It bridges Java method calls from IntegrationHub.java to the native C/C++ SDK functions
 * defined in IntegrationHubWrapper.h.
 * 
 * Platform: MacOS/Linux JNI implementation for Java applications needing to interface
 * with Token Financial Technologies POS devices.
 */

#include <jni.h>
#include <string>
#include <functional>
#include <iostream>
#include "IntegrationHubWrapper.h"

/**
 * @brief Helper function to retrieve the native pointer from the Java object
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @return Pointer to the native ConnectionWrapper object
 */
ConnectionWrapper* getNativePointer(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativePointer", "J");
    jlong ptr = env->GetLongField(obj, fid);
    return reinterpret_cast<ConnectionWrapper*>(ptr);
}

extern "C" {

/**
 * @brief Creates a Communication object and links it to the Java object
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @param companyName Java string containing the company name
 * @return The Java object with initialized native pointer
 */
JNIEXPORT jobject JNICALL Java_com_sdksotestjava_IntegrationHub_createCommunication(JNIEnv *env, jobject obj, jstring companyName) {
    const char* companyNameChars = env->GetStringUTFChars(companyName, nullptr);
    std::string companyNameStr(companyNameChars);
    env->ReleaseStringUTFChars(companyName, companyNameChars);

    ConnectionWrapper* connection = createCommunication(companyNameStr);

    jclass cls = env->GetObjectClass(obj); 
    jfieldID fid = env->GetFieldID(cls, "nativePointer", "J");
    jlong ptr = reinterpret_cast<jlong>(connection);
    env->SetLongField(obj, fid, ptr);
    return obj;
}

/**
 * @brief Deletes the native Communication object
 * @param env JNI environment pointer
 * @param obj Java object reference
 */
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_deleteCommunication(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    deleteCommunication(ptr);
}

/**
 * @brief Attempts to reconnect to the POS device
 * @param env JNI environment pointer
 * @param obj Java object reference
 */
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_reconnect(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    reconnect(ptr);
}

/**
 * @brief Gets the active device index
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @return 0 for X30TR device, 1 for 300TR device, or negative value if no device is active
 */
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_getActiveDeviceIndex(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return getActiveDeviceIndex(ptr);
}

/**
 * @brief Sends basket information to the POS device
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @param jsonData Java string with JSON-formatted basket data
 * @return Status code indicating success (0) or specific error codes
 */
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_sendBasket(JNIEnv *env, jobject obj, jstring jsonData) {
    const char* jsonChars = env->GetStringUTFChars(jsonData, nullptr);
    std::string jsonStr(jsonChars);
    env->ReleaseStringUTFChars(jsonData, jsonChars);

    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return sendBasket(ptr, jsonStr);
}

/**
 * @brief Sends payment information to the POS device
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @param jsonData Java string with JSON-formatted payment data
 * @return Status code indicating success (0) or specific error codes
 */
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_sendPayment(JNIEnv *env, jobject obj, jstring jsonData) {
    const char* jsonChars = env->GetStringUTFChars(jsonData, nullptr);
    std::string jsonStr(jsonChars);
    env->ReleaseStringUTFChars(jsonData, jsonChars);

    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return sendPayment(ptr, jsonStr);
}

/**
 * @brief Retrieves fiscal information from the POS device
 * @param env JNI environment pointer
 * @param obj Java object reference
 * @return Java string with JSON-formatted fiscal information
 */
JNIEXPORT jstring JNICALL Java_com_sdksotestjava_IntegrationHub_getFiscalInfo(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    std::string fiscalInfo = getFiscalInfo(ptr);
    return env->NewStringUTF(fiscalInfo.c_str());
}

/** 
 * @brief Global function pointer to store the serial input callback
 */
std::function<void(int, std::string)> serialInCallback;

/**
 * @brief Wrapper function that routes native callbacks to the stored Java callback
 * @param code Integer tag identifying the message type
 * @param data String data received from the device
 */
void serialInCallbackWrapper(int code, std::string data) {
    if (serialInCallback != nullptr) serialInCallback(code, data);
}


/**
 * @brief Sets up the serial input callback to route events to Java
 * @param env JNI environment pointer
 * @param obj Java object reference
 */
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_setSerialInCallback(JNIEnv *env, jobject obj) {
    jclass callbackClass = env->FindClass("com/sdksotestjava/IntegrationHubTest");
    jmethodID methodID = env->GetStaticMethodID(callbackClass, "serialInCallback", "(ILjava/lang/String;)V");

    // Get Java VM instance
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    // Store the callback in a lambda
    serialInCallback = [jvm, callbackClass, methodID](int code, std::string data) {
        JNIEnv* jniEnv;
        bool shouldDetach = false;

        if (jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_8) != JNI_OK) {
            jvm->AttachCurrentThread(reinterpret_cast<void**>(&jniEnv), nullptr);
            shouldDetach = true;
        }

        jstring jData = jniEnv->NewStringUTF(data.c_str());
        jniEnv->CallStaticVoidMethod(callbackClass, methodID, code, jData);
        jniEnv->DeleteLocalRef(jData);

        if (shouldDetach) {
            jvm->DetachCurrentThread();
        }
    };
    
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    setSerialInCallback(ptr, serialInCallbackWrapper);
}

/**
 * @brief Global function pointer to store the device state callback
 */
std::function<void(int, std::string)> deviceStateCallback;

/**
 * @brief Wrapper function that routes native callbacks to the stored Java callback
 * @param state Boolean indicating device connection state
 * @param data String identifying the device
 */
void deviceStateCallbackWrapper(bool code, std::string data) {
    if (deviceStateCallback != nullptr) deviceStateCallback(code, data);
}

/**
 * @brief Sets up the device state callback to route events to Java
 * @param env JNI environment pointer
 * @param obj Java object reference
 */
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_setDeviceStateCallback(JNIEnv *env, jobject obj) {
    jclass callbackClass = env->FindClass("com/sdksotestjava/IntegrationHubTest");
    jmethodID methodID = env->GetStaticMethodID(callbackClass, "deviceStateCallback", "(ZLjava/lang/String;)V");

    // Get Java VM instance
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    // Store the callback in a lambda
    deviceStateCallback = [jvm, callbackClass, methodID](bool state, std::string data) {
        JNIEnv* jniEnv;
        bool shouldDetach = false;

        if (jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_8) != JNI_OK) {
            jvm->AttachCurrentThread(reinterpret_cast<void**>(&jniEnv), nullptr);
            shouldDetach = true;
        }

        jstring jData = jniEnv->NewStringUTF(data.c_str());
        jniEnv->CallStaticVoidMethod(callbackClass, methodID, state, jData);
        jniEnv->DeleteLocalRef(jData);

        if (shouldDetach) {
            jvm->DetachCurrentThread();
        }
    };
    
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    setDeviceStateCallback(ptr, deviceStateCallbackWrapper);
}

}
