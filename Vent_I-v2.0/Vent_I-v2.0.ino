
/*
   ===========================================
   AutoREV
   ===========================================
   Project  : Ventilator Upgrade - v2
   Date     : 06/11/2020
   Author   : REF
   ===========================================
   Note: Test
         1. HMI Communication
         2. Sensor Suhu
         3. RTC
         4. SD Card xx

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
#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexVariable.h>
#include <NexWaveform.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX
// ===================== SD Card & RTC Setting ===========================
RTC_DS3231 rtc;

File doc;
char t[32];
int i = 0, j = 100;
char buff[32];
String stringClock = "", stringDate = "";
// ========================= HMI Setting =============================
//Declare your Nextion objects , pageid, component id., component name
//Page 0
NexPage p0 = NexPage(0, 0, "page0");
NexProgressBar p0_j0 = NexProgressBar(0, 4, "j0");
NexText p0_t2 = NexText(0, 3, "t2");

//Page 1
NexPage p1 = NexPage(1, 0, "page1");
NexNumber hmi_press = NexNumber(1, 3, "n0");
NexNumber hmi_temp = NexNumber(1, 6, "n1");
NexButton btn_off = NexButton(1, 20, "b0");
NexButton btn_on = NexButton(1, 21, "b1");
NexButton btn_5 = NexButton(1, 22, "b2");
NexButton btn_10 = NexButton(1, 23, "b3");
NexButton btn_15 = NexButton(1, 24, "b4");
NexPicture icon_Wifi = NexPicture(1, 8, "p2");
NexPicture icon_SD = NexPicture (1, 9, "p3");
NexWaveform out_graph = NexWaveform(1, 10, "s0");
NexNumber nHour = NexNumber(1, 14, "n2");
NexNumber nMinute = NexNumber(1, 15, "n3");
NexNumber nSecond = NexNumber(1, 16, "n4");
NexNumber nYear = NexNumber(1, 19, "n7");
NexNumber nMonth = NexNumber(1, 17, "n5");
NexNumber nDate = NexNumber(1, 18, "n6");

//Register a button object to the touch event list
NexTouch *nex_listen_list[] = {
  &btn_on, &btn_off, &btn_5, &btn_10, &btn_15, NULL
};
//=========================================================================
// =========================== I/O Setting ================================

//=========================================================================
// ========================= Variable Setting =============================
uint32_t bar = 0, set_press = 0, out_temp = 0;
int flag_on = 0, destroyDelay;
int pinTemp, valTemp;
unsigned long previousMillis = 0; //using millis for updating wave
const long interval = 100;

void setup() {
  //-------------COMMUNICATION SETUP---------------
  nexInit();
  Serial.begin(115200);
  mySerial.begin(9600);
  setupSDcard();

  //---------------HMI BUTTON SETUP----------------
  btn_on.attachPush(btnOn_Press, &btn_on);
  btn_off.attachPush(btnOff_Press, &btn_off);
  btn_5.attachPush(btn5_Press, &btn_5);
  btn_10.attachPush(btn10_Press, &btn_10);
  btn_15.attachPush(btn15_Press, &btn_15);

  //-----------------STARTUP SCREEN----------------
  p0.show();
  p0_t2.setText("Mohon Tunggu ...");
  waiting();
  p1.show();
  //--------------------SET I/O--------------------
  pinMode(A1, INPUT);
}


void loop() {
  nexLoop(nex_listen_list);   // Listen HMI data
  //------------- Temperature Reading ---------
  pinTemp = analogRead(A0);
  valTemp = (pinTemp * 500) / 1023;
  DateTime now = rtc.now();          //Show RTC on variable now



  //-------------- Update log data graph HMI-----------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //update waveform HMI
//    logdata();

    //    Update temp data HMI

    out_graph.addValue(0, set_press);

  }
}
