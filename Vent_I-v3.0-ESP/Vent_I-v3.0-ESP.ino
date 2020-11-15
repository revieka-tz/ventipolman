
/*
   ===========================================
   AutoREV
   ===========================================
   Project  : Ventilator Upgrade
   Date     : 15/11/2020
   Author   : REF
   ===========================================
   Note: Test
         1. Communication
         2. Button Function
         3. Alert Indicator
         4. Firebase Connection (Setdata success) (Getdata (?))

   ESP WROOM32 - 38Pin
   Pin Connection:
   1. Pin28/GPIO17/TX2  ->  RX HMI
   2. Pin27/GPIO16/RX2  ->  TX HMI
   3. Pin35/GPIO1 /TX0  ->  RX Nano
   4. Pin34/GPIO3 /RX0  ->  TX Nano

   ===========================================

*/

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

//// ========================= WiFi & Firebase Setting =============================
#include <FirebaseESP32.h>
#include <FirebaseESP32HTTPClient.h>
#include <FirebaseJson.h>
#include <WiFi.h>

#define FIREBASE_HOST "https://ventilator-by-polman.firebaseio.com/"
#define FIREBASE_AUTH "jGmlJR2zkmjwckZEe14zMTqFyEudVBqrnUVozkt8"
#define ssid "Golden Rooftop"
#define pass "feellikehome"

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json1, json2;
String jsonStr = "";
String pathVent = "/data/vent_state", pathPress = "/data/press_state";
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
NexText txt = NexText(1, 5, "t2");
//Register a button object to the touch event list
NexTouch *nex_listen_list[] = {
  &btn_on, &btn_off, &btn_5, &btn_10, &btn_15, NULL
};
// ========================= Variable Setting =============================
uint32_t bar = 0, set_press = 0, set_temp = 0, out_temp = 0;
int flag_on = 0, delay10;
int pinTemp, valTemp, valPress = 0;
unsigned long previousMillis = 0; //using millis for updating wave
const long interval = 100;
String dataIn, dt[10], setData;
boolean parsing = false;
// ========================= Control Setting =============================

void setup() {
  //-------------COMMUNICATION SETUP---------------
  nexInit();
  Serial.begin(9600);
  wifiConnect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  json1.set("/press_state", 0);
  Firebase.updateNode(firebaseData, "/data", json1);
  json2.set("/vent_state", 0);
  Firebase.updateNode(firebaseData, "/data", json2);
  //  if (WiFi.status() == WL_CONNECTED)
  //  { Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //    Firebase.setInt("/press_state", set_press);
  //    Firebase.setInt("/vent_state", flag_on);
  //  }

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
  if (WiFi.status() == WL_CONNECTED) {
    icon_Wifi.Set_background_image_pic(6);
    icon_Wifi.setPic(6);
  }
}

void loop() {
  nexLoop(nex_listen_list);   // Listen HMI data
  //-------------- Update log data graph HMI-----------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //update waveform HMI
    //    logdata();
    out_graph.addValue(0, set_press);
    out_graph.addValue(1, valPress);
    if (delay10 == 0) {
      delay10 = 1;
    }
    else if (delay10 == 1) {
      delay10 = 2;
    }
    else if (delay10 == 2) {
      delay10 = 3;
    }
    else if (delay10 == 3) {
      delay10 = 4;
    }
    else if (delay10 == 4) {
      delay10 = 5;
    }
    else if (delay10 == 5) {
      delay10 = 6;
    }
    else if (delay10 == 6) {
      delay10 = 7;
    }
    else if (delay10 == 7) {
      delay10 = 8;
    }
    else if (delay10 == 8) {
      delay10 = 9;
    }
    else if (delay10 == 9) {
      delay10 = 0;
      if (WiFi.status() == WL_CONNECTED)
      {
        if (Firebase.get(firebaseData, pathVent))
        {
          jsonStr = firebaseData.jsonString();
          printResultVent(firebaseData);
        }
        if (Firebase.get(firebaseData, pathPress))
        {
          jsonStr = firebaseData.jsonString();
          printResultPress(firebaseData);
        }
      }
    }
  }
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

}
