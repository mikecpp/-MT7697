#include <LBLE.h>
#include <LBLECentral.h> 

#include "cli.h"

#define MAX_ADV_LEN 32

LBLEClient client;

uint32_t getAdvMsg(int index, char *data, uint32_t data_len)
{
    String  address = LBLECentral.getAddress(index);
    String  name    = LBLECentral.getName(index);
    int32_t rssi    = LBLECentral.getRSSI(index);
    char    buf[MAX_ADV_LEN];
    int     ret = 0;
    char    hex[16];
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
    
    sprintf(data, "%s\t%s\t%d\t%s", 
            address.c_str(), name.c_str(), rssi, manufacture.c_str());
       
    return 0;
}

int cli_scan(int argc, char *argv[])
{
    int  i;
    char msg[128];

    LBLECentral.clear();
    LBLECentral.scan();

    for(i=0; i<5; i++) { // scan 5 seconds
        Serial.print(".");
        delay(1000);
    }
    Serial.println();

    for(i=0; i<LBLECentral.getPeripheralCount(); ++i) {
        getAdvMsg(i, msg, 128);
        Serial.print(i); 
        Serial.print("\t");
        Serial.println(msg);    
    }

    LBLECentral.stopScan();    

    return 0;
}

int cli_connect(int argc, char *argv[])
{
    if(argc != 2) {
        Serial.println("Syntax: connect [id]");
        return -1;
    }

    int index = atoi(argv[1]);

    if(index <0 || index >= LBLECentral.getPeripheralCount()){
        Serial.print(index);
        Serial.println(" is out of range!");
        return -2;
    }

    Serial.print("Connect to ");
    Serial.println(index);

    static LBLEAddress serverAddress;
    
    serverAddress = LBLECentral.getBLEAddress(index);
    client.connect(serverAddress);
    Serial.print("Connect to ");
    Serial.println(serverAddress);
    
    if(!client.connected()){
        Serial.println("Connect fail !");
        return -1;
    }

    Serial.println("Connected !");

    LBLEValueBuffer hr_data;
    if(client.hasService(0x180D)) { // Heart Rate Service
        char cccd = client.readCharacteristicChar(0x2902);
        Serial.print("CCCD = ");
        Serial.println(cccd, HEX);
        client.writeCharacteristicChar(0x2902, 0x01);
    }
    
    return 0;
}

void setup() 
{
    Serial.begin(115200);
   
    // Start BLE
    LBLE.begin();
    while (!LBLE.ready()) {
        delay(10);
    }

    // Start Command Line Interface
    cli_init();
    cli_register(cli_scan, "scan", "scan ble device");
    cli_register(cli_connect, "connect", "connect [id]");
    cli_task();      
}

void loop()
{
    delay(1000);
}

