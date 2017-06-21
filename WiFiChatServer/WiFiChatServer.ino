#include <LWiFi.h>

char ssid[] = "SSR1100 (2.4G)"; //  your network SSID (name)
char pass[] = "ykdah48545";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
	//Initialize serial and wait for port to open:
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(ssid, pass);
	}

	// start the server:
	server.begin();
	// you're connected now, so print out the status:
	printWifiStatus();
}


void loop() {
	// wait for a new client:
	WiFiClient client = server.available();


	// when the client sends the first byte, say hello:
	if (client) {
		if (!alreadyConnected) {
			// clead out the input buffer:
			client.flush();
			Serial.println("We have a new client");
			client.println("Hello, client!");
			alreadyConnected = true;
		}

		if (client.available() > 0) {
			// read the bytes incoming from the client:
			char thisChar = client.read();
			// echo the bytes back to the client:
			server.write(thisChar);
			// echo the bytes to the server as well:
			Serial.write(thisChar);
		}
	}
}


void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}
