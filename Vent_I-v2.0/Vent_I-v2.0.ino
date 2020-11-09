
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

   Arduino Nano - 38Pin
   Pin Connection:
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

// ========================= HMI Setting =============================
//Declare your Nextion objects , pageid, component id., component name
//Page 0
NexPage p0 = NexPage(0, 0, "page0");
NexProgressBar p0_j0 = NexProgressBar(0, 4, "j0");
NexText p0_t2 = NexText(0, 3, "t2");

//Page 1
NexPage p1 = NexPage(1, 0, "page1");
NexNumber hmi_press = NexNumber(1, 3, "n0");
NexNumber hmi_temp = NexNumber(1, 8, "n1");
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
======================================================================
// ========================= Variable Setting =============================
uint32_t bar = 0, pressValue = 0, tempValue = 0;

void setup() {
  //-------------SETUP COMMUNICATION---------------
  nexInit();
  Serial.begin(9600);

  //-----------------STARTUP SCREEN----------------
  p0.show();
  p0_t2.setText("Mohon Tunggu ...");
  waiting();
  p1.show();

  
}


void loop() {
  nexLoop(nex_listen_list);   // Listen HMI data
}
