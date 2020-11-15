void parsingData() {
  int j = 0;
  dt[j] = "";
  //proses parsing data
  for (int i = 1; i < dataIn.length(); i++) {     //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    { //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = "";     //inisialisasi variabel array dt[j]
    }
    else
    { //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }
  }
nDate.setValue(dt[0].toInt());
nMonth.setValue(dt[1].toInt());
nYear.setValue(dt[2].toInt());
nHour.setValue(dt[3].toInt());
nMinute.setValue(dt[4].toInt());
nSecond.setValue(dt[5].toInt());
hmi_temp.setValue(dt[6].toInt());
valPress = dt[7].toInt();
if(dt[8].toInt() == 0){
  icon_SD.Set_background_image_pic(10);
}
else if(dt[8].toInt() == 1){
  icon_SD.Set_background_image_pic(4);
}
}
