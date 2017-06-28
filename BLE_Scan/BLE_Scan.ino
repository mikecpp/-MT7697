#include <LWiFi.h>
#include <LBLE.h>
#include <LBLECentral.h> 

#define MAX_ADV_LEN 32

char ssid[] = "Mike iPhone 6 plus"; 
char pass[] = "mikechen";

int keyIndex = 0;            
int status   = WL_IDLE_STATUS;
WiFiServer server(8888);
boolean alreadyConnected = false; 

void setup() 
{
	Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); 
    
    // Start BLE
    LBLE.begin();
    while (!LBLE.ready()) {
        delay(10);
    }

    // Start WiFi
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		status = WiFi.begin(ssid, pass);
        digitalWrite(LED_BUILTIN, LOW); 
	}

	server.begin();
 
	printWifiStatus();
}

void loop() 
{
	WiFiClient client = server.available();

    LBLECentral.stopScan();

    char msg[128];

    for (int i = 0; i < LBLECentral.getPeripheralCount(); ++i) {
        getAdvMsg(i, msg, 128);
        Serial.println(msg);
    
    	if (client) {
    	    if (!alreadyConnected) {
    			client.flush();
    			client.println("BLE/WiFi Gateway Start...");
    			alreadyConnected = true;
    		}
    	}
        server.println(msg);               
    }
    
    LBLECentral.clear();
    LBLECentral.scan();

    delay(1000);   
}

void printWifiStatus() 
{
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
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

    if(name.indexOf("BTag") != -1) {
        sprintf(data, "2017-06-28,17:21:18,%s,%s,%d,%d,%s", 
                address.c_str(), name.c_str(), rssi, manufacture.length()/2,manufacture.c_str());
    }            
       
    return 0;
}

