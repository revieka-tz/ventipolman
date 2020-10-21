/*
   ===========================================
   AutoREV
   ===========================================
   Project  : Ventilator Upgrade
   Date     : 13/08/2020
   Author   : REF
   ===========================================
   Note: Test
         1. Communication
         2. Button Function
         3. Alert Indicator
         4. Firebase Connection

   ESP WROOM32 - 38Pin
   Pin Connection:
   1. Pin28/GPIO17/TX2  ->  RX HMI
   2. Pin27/GPIO16/RX2  ->  TX HMI
   3. Pin3 /GPIO36      ->  Buzzer
   4. Pin5 /GPIO34      ->  Sensor Suhu
   5. Pin6 /GPIO35      ->  Sensor Pressure
   6. Pin37/GPIO23/MOSI ->  MOSI-SD Card
   8. Pin31/GPIO19/MISO ->  MISO-SD Card
   9. Pin30/GPIO18/SCK  ->  SCK-SD Card
   10.Pin29/GPIO5 /SS   ->  CS-SD Card
   11.Pin36/GPIO22/SCL  ->  SCL-RTC DS3231
   12.Pin33/GPIO21/SDA  ->  SDA-RTC DS3231
   13.Pin/GPIO32/SDA  ->  SDA-RTC DS3231
   ===========================================
    Bismillah
*/
#include <ESP32_Servo.h>
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

// ========================= WiFi Setting =============================
#include <FirebaseESP32.h>
#include <WiFi.h>

#define FIREBASE_HOST "https://venti-polman.firebaseio.com/"
#define FIREBASE_AUTH "jPfiu6OSKpUHuu81Lqbyjoz6BWv3LqbvrAzkbBWT"
#define ssid "REF"
#define pass "repirepi"

FirebaseData firebaseData;

// ========================= HMI Setting =============================
//Declare your Nextion objects , pageid, component id., component name
//Page 0
NexPage p0 = NexPage(0, 0, "page0");
NexProgressBar p0_j0 = NexProgressBar(0, 4, "j0");
NexText p0_t2 = NexText(0, 3, "t2");

//Page 1
NexPage p1 = NexPage(1, 0, "page1");
NexSlider sliderPress = NexSlider(1, 4, "h0");
NexNumber p1_n0 = NexNumber(1, 3, "n0");
NexNumber p1_n1 = NexNumber(1, 8, "n1");
NexButton btnPress = NexButton(1, 6, "b0");
NexPicture flagWifi = NexPicture(1, 10, "p2");
NexPicture flagSD = NexPicture (1, 11, "p3");
NexWaveform wave = NexWaveform(1, 12, "s0");
NexNumber nHour = NexNumber(1, 16, "n2");
NexNumber nMinute = NexNumber(1, 17, "n3");
NexNumber nSecond = NexNumber(1, 18, "n4");
NexNumber nYear = NexNumber(1, 21, "n7");
NexNumber nMonth = NexNumber(1, 19, "n5");
NexNumber nDate = NexNumber(1, 20, "n6");

//Register a button object to the touch event list
NexTouch *nex_listen_list[] = {
  &btnPress, &sliderPress, NULL
};
// ===================== SD Card & RTC Setting ===========================
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

char t[32];
int i = 0, j = 100;
char buff[32];
String stringClock = "", stringDate = "";
// ========================= Control Setting =============================
uint32_t bar = 0, pressValue = 1000, tempValue = 30;
const int buzz = 36, pinTemp = 34, pinPress = 35;
int flagBtnPress = 0, onColor = 3, offColor = 2, alert = 9,
    sPress = 0, sTemp = 0; // alert 9 on - 8 off
double Voltage = 0;
float tempC = 0;
int speedESC, graph = 0;
Servo esc;
unsigned long previousMillis = 0;
const long interval = 100;

void setup() {
  flagSD.Set_background_image_pic(4);
  setupSDcard();
  nexInit();
  p0.show();
  p0_t2.setText("Mohon Tunggu ...");
  waiting();
  p1.show();
  btnPress.attachPush(btnPress_Release, &btnPress);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz, HIGH);
  esc.attach(32);
  // Wifi setup
  wifiConnect();
  Firebase.setInt(firebaseData, "btnPress", 0);
}


void loop() {

  // Listen data HMI
  nexLoop(nex_listen_list);

  // Wifi connection
  if (WiFi.status() != WL_CONNECTED)
  { flagWifi.Set_background_image_pic(7);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //I/O output set
  sPress = analogRead(pinPress);
  sTemp = analogRead(pinTemp);

  Voltage = (sTemp / 4095.0) * 3300; // 3300 to get millivots.
  tempC = Voltage / 5.421686;
  Serial.print(sPress);
  Serial.print("---");
  Serial.println(tempC);


  speedESC = map(pressValue, 1000, 1980, 52, 179);
  graph = map(pressValue, 1000, 1980, 0, 100);

  // Sensor Pressure
  if (sPress > 1000 && alert == 9) {
    digitalWrite(buzz, LOW);
    alert = 8;
    p1_n0.Set_background_color_bco(63488);
  }
  else if (sPress < 1000 && alert == 8) {
    digitalWrite(buzz, HIGH);
    alert = 9;
    p1_n0.Set_background_color_bco(54970);
  }

  // Update graph log data HMI
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (flagBtnPress == 1) {
      esc.write(speedESC);
      wave.addValue(0, graph);
      sliderPress.getValue(&pressValue);
      p1_n0.setValue(pressValue);
      p1_n1.setValue(tempC);
      btnPress.Set_background_image_pic(onColor);
      btnPress.Set_press_background_image_pic2(offColor);
    }
    else if (flagBtnPress == 0) {
      esc.write(52);
      wave.addValue(0, 0);
      sliderPress.getValue(&pressValue);
      p1_n0.setValue(pressValue);
      p1_n1.setValue(tempC);
      btnPress.Set_background_image_pic(offColor);
      btnPress.Set_press_background_image_pic2(onColor);
    }
    logdata();

  }

}
