#include <SPI.h>
#include <SD.h>

File myFile;

void setup() 
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    Serial.print("Initializing SD card...");

    if (!SD.begin()) {
        Serial.println("initialization failed!");
        return;
    }
    
    Serial.println("initialization done.");

    myFile = SD.open("test.txt", FILE_WRITE);

    // Write to file
    if (myFile) 
    {
        Serial.print("Writing to test.txt...");
        myFile.println("testing 1, 2, 3.");
        myFile.close();
        Serial.println("done.");
    } 
    else 
    {
        Serial.println("error opening test.txt");
    }

    // Read from file
    myFile = SD.open("test.txt");
    
    if (myFile) {
        Serial.println("test.txt:");

        while (myFile.available()) {
            Serial.write(myFile.read());
        }
        myFile.close();
    } 
    else 
    {
        Serial.println("error opening test.txt");
    }
}

void loop()
{

}


