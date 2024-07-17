#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#if PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <bluetoothapis.h>
#include <ws2bth.h>
#endif
#include "BluetoothManager.generated.h"

UCLASS()
class SMARTCLOCK_API UBluetoothManager : public UBlueprintFunctionLibrary 
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static void RequestPermissions();

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static bool InitializeBluetooth(FString deviceName);

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static void ReconnectBluetooth();

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static FString ReceiveBluetoothData();

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static bool SendBluetoothData(const FString &data);

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static FString CreateAlarmJson(int32 Hours, int32 Minutes, const TArray<FString> &Days);

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static FString ParseTemperatureJson(const FString &JsonString);

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static bool IsBluetoothConnected();

    UFUNCTION(BlueprintCallable, Category = "Bluetooth")
    static void DisconnectBluetooth();


private:
#if PLATFORM_WINDOWS
    static BLUETOOTH_DEVICE_INFO DeviceInfo;
    static SOCKET btSocket;
#endif
    static bool isInitialized;
};