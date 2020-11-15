void printResultVent(FirebaseData &data)
{
  if (data.dataType() == "int")
  {
    Serial.print("venstate");
    Serial.println(data.intData());
    flag_on = data.intData();
    if (flag_on == 1) {
      hmi_press.Set_background_color_bco(34800);
    }
    else if (flag_on == 0) {
      hmi_press.Set_background_color_bco(54970);
      json1.set("/press_state", 0);
      Firebase.updateNode(firebaseData, "/data", json1);
    }
  }
}
void printResultPress(FirebaseData &data)
{
  if (data.dataType() == "int")
  {

    Serial.print("venstate");
    Serial.println(data.intData());
    set_press = data.intData();
    if (flag_on == 1) {
      hmi_press.setValue(set_press);
    }
  }
}
