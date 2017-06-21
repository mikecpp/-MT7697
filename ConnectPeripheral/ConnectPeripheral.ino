#include <LBLE.h>
#include <LBLECentral.h>

LBLEClient client;

void setup() 
{
    Serial.begin(115200);

    Serial.println("BLE begin");
    LBLE.begin();
    while (!LBLE.ready()) {
        delay(10);
    }
    Serial.println("BLE ready");

    LBLECentral.scan();
}

int searching = 1;

enum AppState
{
    SEARCHING,    // We scan nearby devices and provide a list for user to choose from
    CONNECTING,   // User has choose the device to connect to
    CONNECTED     // We have connected to the device
};

void loop() 
{
    static AppState state = SEARCHING;
    static LBLEAddress serverAddress;

    if(state == CONNECTED)
    {
        if(!client.connected())
        {
            Serial.println("disconnected from remote device");
            state = SEARCHING;
        }
    }
  
    switch(state)
    {
        case SEARCHING:
        {
            // wait for a while
            Serial.println("state=SEARCHING");
            for(int i = 0; i < 10; ++i)
            {
                delay(1000);
                Serial.print(".");
            }
            // enumerate advertisements found.
            Serial.print("Peripherals found = ");
            Serial.println(LBLECentral.getPeripheralCount());
            Serial.println("idx\taddress\t\t\tflag\tRSSI");
            for (int i = 0; i < LBLECentral.getPeripheralCount(); ++i) {
                printDeviceInfo(i);
            }

            // waiting for user input
            Serial.println("Select the index of device to connect to: ");
            while(!Serial.available())
            {
                delay(100);
            }

            const int idx = Serial.parseInt();

            if(idx < 0 || idx >= LBLECentral.getPeripheralCount()) {
                Serial.println("wrong index, keep scanning devices.");
            }
            else {
                serverAddress = LBLECentral.getBLEAddress(idx);
                Serial.print("Connect to device with address ");
                Serial.println(serverAddress);
                // we must stop scan before connecting to devices
                LBLECentral.stopScan();
                state = CONNECTING;
            }
        }
        break;
  
        case CONNECTING:
        {
            Serial.println("state=CONNECTING");
            client.connect(serverAddress);
            if(client.connected())
            {
                state = CONNECTED;
            }
            else
            {
                Serial.println("can't connect");
            }
        }
        break;
 
        case CONNECTED:
        {
            Serial.println("state=CONNECTED");

            // display all services of the remote device
            const int serviceCount = client.getServiceCount();
            Serial.println("available services = ");
            for(int i = 0; i < serviceCount; ++i)
            {
                Serial.print("\t - ");
                const String serviceName = client.getServiceName(i);
                if(serviceName.length())
                {
                    Serial.print("[");
                    Serial.print(serviceName);
                    Serial.print("] ");
                }
                Serial.println(client.getServiceUuid(i));
      
            }

            // read the device manufacture - 
            // first we check if "Device Information"(0x180A) service is available:
            if(client.hasService(0x180A))
            {
                const String name = client.readCharacteristicString(LBLEUuid(0x2A29));
                if(name.length() > 0)
                {
                    Serial.print("manufacturer=");
                    Serial.println(name);
                }
            }
            else
            {
                Serial.println("No device information found");
            }
    
            while(true)
            {
                delay(100);
            }
        }
        break;
    }
}

void printDeviceInfo(int i) {
    Serial.print(i);
    Serial.print("\t");
    Serial.print(LBLECentral.getAddress(i));
    Serial.print("\t");
    Serial.print(LBLECentral.getAdvertisementFlag(i), HEX);
    Serial.print("\t");
    Serial.print(LBLECentral.getRSSI(i));
    Serial.print("\t");
    const String name = LBLECentral.getName(i);
    Serial.print(name);
    if(name.length() == 0)
    {
        Serial.print("(Unknown)");
    }
    Serial.print(" by ");
    const String manu = LBLECentral.getManufacturer(i);
    Serial.print(manu);
    Serial.print(", service: ");
    if (!LBLECentral.getServiceUuid(i).isEmpty()) {
        Serial.print(LBLECentral.getServiceUuid(i));
    } 
    else {
        Serial.print("(no service info)");
    }

  Serial.println();
}

