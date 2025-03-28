#include <jni.h>
#include <string>
#include <functional>
#include <iostream>
#include "IntegrationHubWrapper.h"

// Helper function to retrieve the native pointer from the Java object
ConnectionWrapper* getNativePointer(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativePointer", "J");
    jlong ptr = env->GetLongField(obj, fid);
    return reinterpret_cast<ConnectionWrapper*>(ptr);
}

extern "C" {

// Create a Communication object
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

// Delete the Communication object
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_deleteCommunication(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    deleteCommunication(ptr);
}

// Reconnect the communication
JNIEXPORT void JNICALL Java_com_sdksotestjava_IntegrationHub_reconnect(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    reconnect(ptr);
}

// Get the active device index
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_getActiveDeviceIndex(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return getActiveDeviceIndex(ptr);
}

// Send a basket
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_sendBasket(JNIEnv *env, jobject obj, jstring jsonData) {
    const char* jsonChars = env->GetStringUTFChars(jsonData, nullptr);
    std::string jsonStr(jsonChars);
    env->ReleaseStringUTFChars(jsonData, jsonChars);

    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return sendBasket(ptr, jsonStr);
}

// Send payment
JNIEXPORT jint JNICALL Java_com_sdksotestjava_IntegrationHub_sendPayment(JNIEnv *env, jobject obj, jstring jsonData) {
    const char* jsonChars = env->GetStringUTFChars(jsonData, nullptr);
    std::string jsonStr(jsonChars);
    env->ReleaseStringUTFChars(jsonData, jsonChars);

    ConnectionWrapper* ptr = getNativePointer(env, obj);
    return sendPayment(ptr, jsonStr);
}

// Get fiscal information
JNIEXPORT jstring JNICALL Java_com_sdksotestjava_IntegrationHub_getFiscalInfo(JNIEnv *env, jobject obj) {
    ConnectionWrapper* ptr = getNativePointer(env, obj);
    std::string fiscalInfo = getFiscalInfo(ptr);
    return env->NewStringUTF(fiscalInfo.c_str());
}

std::function<void(int, std::string)> serialInCallback;
void serialInCallbackWrapper(int code, std::string data) {
    if (serialInCallback != nullptr) serialInCallback(code, data);
}


// Set SerialInCallback
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

std::function<void(int, std::string)> deviceStateCallback;
void deviceStateCallbackWrapper(bool code, std::string data) {
    if (deviceStateCallback != nullptr) deviceStateCallback(code, data);
}

// Set DeviceStateCallback
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
