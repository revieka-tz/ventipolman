//====================Writing to SD Card==========================
void logdata() {
  DateTime now = rtc.now();          //Show RTC on variable now

  nHour.setValue(int(now.hour()));
  nMinute.setValue(int(now.minute()));
  nSecond.setValue(int(now.second()));
  nYear.setValue(int(now.year()));
  nMonth.setValue(int(now.month()));
  nDate.setValue(int(now.day()));
  appendFile(SD, "/log.txt", itoa(int(now.day()), buff, 10));
  appendFile(SD, "/log.txt", "/");
  appendFile(SD, "/log.txt", itoa(int(now.month()), buff, 10));
  appendFile(SD, "/log.txt", "/");
  appendFile(SD, "/log.txt", itoa(int(now.year()), buff, 10));
  appendFile(SD, "/log.txt", ",");
  appendFile(SD, "/log.txt", itoa(int(now.hour()), buff, 10));
  appendFile(SD, "/log.txt", ":");
  appendFile(SD, "/log.txt", itoa(int(now.minute()), buff, 10));
  appendFile(SD, "/log.txt", ":");
  appendFile(SD, "/log.txt", itoa(int(now.second()), buff, 10));
  appendFile(SD, "/log.txt", ",");
  appendFile(SD, "/log.txt", itoa(tempValue, buff, 10));
  appendFile(SD, "/log.txt", ",");
  appendFile(SD, "/log.txt", itoa(pressValue, buff, 10));
  appendFile(SD, "/log.txt", "\n");
}
