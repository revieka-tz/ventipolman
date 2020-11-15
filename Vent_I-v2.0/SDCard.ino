void setupSDcard() {
  Wire.begin(); //SDA SCL RTC
  rtc.begin();
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

//====================Writing to SD Card==========================
void logdata() {


//  doc = SD.open("log.txt", FILE_WRITE);
//  if (doc) {
//    doc.print(int(now.day()));
//    doc.print("/");
//    doc.print(int(now.month()));
//    doc.print("/");
//    doc.print(int(now.year()));
//    doc.print(",");
//    doc.print(int(now.hour()));
//    doc.print(":");
//    doc.print(int(now.minute()));
//    doc.print(":");
//    doc.print(int(now.second()));
//    doc.print(",");
//    doc.println(valTemp);
//    doc.close(); // close the file
//  }
//  // if the file didn't open, print an error:
//  else {
//    Serial.println("error opening test.txt");
//  }
}
