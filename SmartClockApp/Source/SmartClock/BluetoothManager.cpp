#include "BluetoothManager.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif
#include <string>
#include "Misc/OutputDeviceDebug.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"


#if PLATFORM_WINDOWS
BLUETOOTH_DEVICE_INFO UBluetoothManager::DeviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
SOCKET UBluetoothManager::btSocket = INVALID_SOCKET;
#endif
bool UBluetoothManager::isInitialized = false;


//------------------------------------------------------
void UBluetoothManager::RequestPermissions()
{
#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
    {
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "requestPermissions", "(Landroid/app/Activity;)V");
        jobject Activity = FJavaWrapper::GameActivityThis;
        Env->CallStaticVoidMethod(Class, Method, Activity);
    }
#endif
}


//------------------------------------------------------
bool UBluetoothManager::InitializeBluetooth(FString bltName)
{
#if PLATFORM_WINDOWS
    BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(btfrp) };
    HANDLE hRadio;
    HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&btfrp, &hRadio);
    if (!hFind) {
        return false;
    }
    BluetoothFindRadioClose(hFind);

    BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(btsp) };
    btsp.hRadio = hRadio;
    btsp.fReturnAuthenticated = true;
    btsp.fReturnRemembered = true;
    btsp.fReturnConnected = true;
    btsp.fReturnUnknown = true;
    btsp.fIssueInquiry = true;
    btsp.cTimeoutMultiplier = 2;

    HBLUETOOTH_DEVICE_FIND hFindDevice = BluetoothFindFirstDevice(&btsp, &DeviceInfo);
    FString targetDeviceName = bltName;
    bool deviceFound = false;

    while (hFindDevice)
    {
        FString DeviceName(DeviceInfo.szName);

        if (DeviceName == targetDeviceName)
        {
            deviceFound = true;
            break;
        }

        if (!BluetoothFindNextDevice(hFindDevice, &DeviceInfo))
        {
            break;
        }
    }

    if (!deviceFound)
    {
        BluetoothFindDeviceClose(hFindDevice);
        CloseHandle(hRadio);
        return false;
    }

    btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (btSocket == INVALID_SOCKET) {
        BluetoothFindDeviceClose(hFindDevice);
        CloseHandle(hRadio);
        return false;
    }


    SOCKADDR_BTH btAddr = { 0 };
    btAddr.addressFamily = AF_BTH;
    btAddr.btAddr = DeviceInfo.Address.ullLong;
    btAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
    btAddr.port = 1; // Ensure this matches the port used by the ESP32

    if (connect(btSocket, (SOCKADDR *)&btAddr, sizeof(btAddr)) == SOCKET_ERROR) {
        closesocket(btSocket);
        BluetoothFindDeviceClose(hFindDevice);
        CloseHandle(hRadio);
        return false;
    }

    isInitialized = true;

    // Close the device handle
    BluetoothFindDeviceClose(hFindDevice);
    CloseHandle(hRadio);

    return true;
#endif

#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv()) {
        UE_LOG(LogTemp, Warning, TEXT("JNIEnv acquired"));
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        if (Class == nullptr) {
            UE_LOG(LogTemp, Warning, TEXT("Class BluetoothManager not found"));
            return false;
        }
        else {
            UE_LOG(LogTemp, Display, TEXT("Class BluetoothManager found!"));
        }

        // Now call the initializeBluetooth method
        jmethodID Method = Env->GetStaticMethodID(Class, "initializeBluetooth", "(Ljava/lang/String;Landroid/content/Context;)Z");
        jstring JavaDeviceName = Env->NewStringUTF(TCHAR_TO_UTF8(*bltName));
        jobject Context = FJavaWrapper::GameActivityThis;
        jboolean Result = Env->CallStaticBooleanMethod(Class, Method, JavaDeviceName, Context);
        Env->DeleteLocalRef(JavaDeviceName);
        return Result == JNI_TRUE;
    }
#endif

    return false;
}

//------------------------------------------------------
void UBluetoothManager::ReconnectBluetooth() {
#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
    {
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "reconnectBluetooth", "(Landroid/content/Context;)V");
        jobject Context = FJavaWrapper::GameActivityThis;
        Env->CallStaticVoidMethod(Class, Method, Context);
    }
#endif
}

//------------------------------------------------------
FString UBluetoothManager::ReceiveBluetoothData()
{
#if PLATFORM_WINDOWS
    if (!isInitialized)
    {
        return FString("Bluetooth not initialized");
    }

    char recvbuf[512];
    int recvbuflen = 512;
    int iResult = recv(btSocket, recvbuf, recvbuflen, 0);

    if (iResult > 0) {
        recvbuf[iResult] = '\0';

        // Parse the JSON data
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(recvbuf);

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            float Temperature = JsonObject->GetNumberField(TEXT("temp"));
            return FString::Printf(TEXT("%.2f"), Temperature); // Return the temperature as a string
        }
        
    }
    else {
        int error = WSAGetLastError();
        strcpy_s(recvbuf, "Failed to receive data");
    }

    return FString("");
#endif


#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
    {
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "receiveBluetoothData", "()Ljava/lang/String;");
        jstring JavaData = (jstring)Env->CallStaticObjectMethod(Class, Method);
        const char *NativeData = Env->GetStringUTFChars(JavaData, 0);
        FString Result(NativeData);
        Env->ReleaseStringUTFChars(JavaData, NativeData);
        Env->DeleteLocalRef(JavaData);
        return Result;
    }
#endif
    return FString("");
}


//------------------------------------------------------
bool UBluetoothManager::SendBluetoothData(const FString &Data)
{
#if PLATFORM_WINDOWS
    if (!isInitialized)
    {
        return false;
    }


    std::string dataStr = TCHAR_TO_UTF8(*Data);
    int iResult = send(btSocket, dataStr.c_str(), dataStr.length(), 0);

    if (iResult == SOCKET_ERROR) {
        return false;
    }

    return true;
#endif

#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
    {
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "sendBluetoothData", "(Ljava/lang/String;)Z");
        jstring JavaData = Env->NewStringUTF(TCHAR_TO_UTF8(*Data));

        if (Method == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Method sendBluetoothData not found"));
            return false;
}

        jboolean Result = Env->CallStaticBooleanMethod(Class, Method, JavaData);
        Env->DeleteLocalRef(JavaData);

        return Result == JNI_TRUE;
    }
#endif
    return false;
}


//------------------------------------------------------
FString UBluetoothManager::CreateAlarmJson(int32 Hours, int32 Minutes, const TArray<FString> &Days)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> TimeObject = MakeShareable(new FJsonObject);
    TimeObject->SetNumberField(TEXT("hours"), Hours);
    TimeObject->SetNumberField(TEXT("minutes"), Minutes);
    JsonObject->SetObjectField(TEXT("time"), TimeObject);

    TArray<TSharedPtr<FJsonValue>> DaysArray;
    for (const FString &Day : Days)
    {
        DaysArray.Add(MakeShareable(new FJsonValueString(Day)));
    }
    JsonObject->SetArrayField(TEXT("day"), DaysArray);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}


//------------------------------------------------------
FString UBluetoothManager::ParseTemperatureJson(const FString &JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        float TemperatureFloat = JsonObject->GetNumberField(TEXT("temp"));
        int32 Temperature = FMath::FloorToInt(TemperatureFloat);
        return FString::Printf(TEXT("%d F"), Temperature);
    }

    return FString("-- F");
}


//------------------------------------------------------
bool UBluetoothManager::IsBluetoothConnected() {
#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv()) {
        UE_LOG(LogTemp, Display, TEXT("Checking if is Connected"));
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "isBluetoothConnected", "()Z");
        jboolean Result = Env->CallStaticBooleanMethod(Class, Method);
        return Result == JNI_TRUE;
    }
#endif
    return isInitialized;
}


//------------------------------------------------------
void UBluetoothManager::DisconnectBluetooth() {
#if PLATFORM_WINDOWS
    if (isInitialized) {
        closesocket(btSocket);
        btSocket = INVALID_SOCKET;
        isInitialized = false;
    }
#endif

#if PLATFORM_ANDROID
    if (JNIEnv *Env = FAndroidApplication::GetJavaEnv()) {
        jclass Class = FAndroidApplication::FindJavaClass("com/rykostudios/smartclock/BluetoothManager");
        jmethodID Method = Env->GetStaticMethodID(Class, "disconnectBluetooth", "(Landroid/content/Context;)V");
        jobject Context = FJavaWrapper::GameActivityThis;
        Env->CallStaticVoidMethod(Class, Method, Context);
    }
#endif
}