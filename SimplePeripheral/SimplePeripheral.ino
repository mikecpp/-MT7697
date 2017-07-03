#include <LBLE.h>
#include <LBLECentral.h> 
#include <LBLEPeriphral.h>

#define MAX_ADV_LEN 32

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
    LBLECentral.stopScan();
    
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

    char msg[128];

    for (int i = 0; i < LBLECentral.getPeripheralCount(); ++i) {
        getAdvMsg(i, msg, 128);
        Serial.println(msg);
        
    }
    
    LBLECentral.clear();
    LBLECentral.scan();
    
    delay(1000);   
}

uint32_t getAdvMsg(int index, char *data, uint32_t data_len)
{
    String  address = LBLECentral.getAddress(index);
    String  name    = LBLECentral.getName(index);
    int32_t rssi    = LBLECentral.getRSSI(index);
    char buf[MAX_ADV_LEN];
    int  ret = 0;
    char hex[16];
    String manufacture;

    ret = LBLECentral.getRawManufacturerData(index, (uint8_t*) buf, MAX_ADV_LEN);
    
    for(int i=0; i<ret; i++) 
    {
        sprintf(hex, "%02X", buf[i]); 
        manufacture += hex;
    } 
    
    if(name.length() == 0) {
        name = "----";
    }

    // if(name.indexOf("BTag") != -1) 
    {
        sprintf(data, "2017-06-28,17:21:18,%s,%s,%d,%d,%s", 
                address.c_str(), name.c_str(), rssi, manufacture.length()/2,manufacture.c_str());
    }            
       
    return 0;
}

