
/*
   ===========================================
   AutoREV
   ===========================================
   Project  : Ventilator Upgrade - v2
   Date     : 14/11/2020
   Author   : REF
   ===========================================
   Note: Test
         1. HMI Communication
         2. Sensor Suhu
         3. RTC
         4. SD Card

   Arduino Nano
   Pin Connection:
   1. A4 /SDA  -
   2. A5 /SCL  -
   3. D11/MOSI -
   4. D12/MISO -
   5. D13/SCK  -
   6. D10/CS   -
   7. D0/RX    - TX HMI
   8. D1/TX    - RX HMI

*/

//------------------------LIBRARY--------------------------

#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include <Wire.h>

// ===================== SD Card & RTC Setting ===========================
const int chipSelect = 4;

RTC_DS3231 rtc;
int i = 0, j = 100;
// ========================= HMI Setting =============================
//Declare your Nextion objects , pageid, component id., component name
//Page 0

//=========================================================================
// =========================== I/O Setting ================================

//=========================================================================
// ========================= Variable Setting =============================
uint32_t bar = 0, set_press = 0, out_temp = 0;
int flag_on = 0, flagSD = 0;
int pinTemp, valTemp, pinPress ,valPress = 0;
unsigned long previousMillis = 0; //using millis for updating wave
const long interval = 1000;
String dataforESP = "", dataforSD = "";
String dataIn, dt[2];
boolean parsing = false;
void setup() {
  //-------------COMMUNICATION SETUP---------------
  //  nexInit();
  Wire.begin();
  rtc.begin();
  Serial.begin(9600);
  //------------------ SD Card Check --------------------
  if (!SD.begin(chipSelect)) {
    flagSD = 1;
  }

  //--------------------SET I/O--------------------

}


void loop() {

  //------------- Temperature Reading ---------
  pinTemp = analogRead(A0);
  valTemp = (pinTemp * 500) / 1023;
  if (valTemp < 0) {
    valTemp = valTemp * -1;
  }

  //------------- Pressure Reading ---------
  pinPress = analogRead(A4);
  pinPress = (pinTemp * 500) / 1023;
  if (valTemp < 0) {
    valTemp = valTemp * -1;
  }
  //-------------------Preparing datapack---------------------
  String dataforESP = "";  // make a string for assembling the data to log:
  String dataforSD = "";
  DateTime now = rtc.now();       //Menampilkan RTC pada variable now
  dataforESP = "*" + String(now.day()) + "," + String(now.month()) + "," + String(now.year()) +
               "," + String(now.hour()) + "," + String(now.minute()) + "," + String(now.second())
               + "," + valTemp + "," + valPress + "," + flagSD + "#";

  dataforSD = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) +
              "," + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second())
              + "," + valTemp + "," + valPress + "," + flagSD;
  Serial.print(dataforESP);
  delay(200);


  //------------------Parsing Received Data----------------------------
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    //    Serial.println(inChar);

    dataIn += inChar;
    if (inChar == '#') {
      parsing = true;
    }
  }

  if (parsing) {
    parsingData();
    parsing = false;
    dataIn = "";
  }
  //-------------- Update log data graph HMI-----------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataforSD);
      dataFile.close();
    }

  }
}
