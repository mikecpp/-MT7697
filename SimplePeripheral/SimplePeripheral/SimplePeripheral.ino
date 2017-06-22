#include <LBLE.h>
#include <LBLEPeriphral.h>

LBLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");
LBLECharacteristicInt switchCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", LBLE_READ | LBLE_WRITE);
LBLECharacteristicString uartCharacteristic("19B1012-E8F2-537E-4F6C-D104768A1214", LBLE_READ | LBLE_WRITE);

void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(115200);

    LBLE.begin();
    while (!LBLE.ready()) {
        delay(100);
    }
    Serial.println("BLE ready");

    Serial.print("Device Address = [");
    Serial.print(LBLE.getDeviceAddress());
    Serial.println("]");

    LBLEAdvertisementData advertisement;
    advertisement.configAsConnectableDevice("BLE LED");

    LBLEPeripheral.setName("BLE LED");
    ledService.addAttribute(switchCharacteristic);
    ledService.addAttribute(uartCharacteristic);
    LBLEPeripheral.addService(ledService);

    LBLEPeripheral.begin();
    LBLEPeripheral.advertise(advertisement);
}

void loop() 
{
    delay(100);

    if(switchCharacteristic.isWritten()) {
        const char value = switchCharacteristic.getValue();
        switch (value) {
            case 1:
                digitalWrite(LED_BUILTIN, HIGH);
                break;
            case 0:
                digitalWrite(LED_BUILTIN, LOW);
                break;
            default:
                Serial.println("Unknown value written");
                break;
        }
    }
    
    if(uartCharacteristic.isWritten()) {
        Serial.println(uartCharacteristic.getValue());
    }
}

