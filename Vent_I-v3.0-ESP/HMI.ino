void btnOn_Press(void *ptr) {
  if (flag_on == 0) {
    hmi_press.Set_background_color_bco(34800);
    set_press = 0;
    flag_on = 1;
    hmi_press.setValue(set_press);
    json1.set("/vent_state", flag_on);
    Firebase.updateNode(firebaseData, "/data", json1);
    json1.set("/press_state", 0);
    Firebase.updateNode(firebaseData, "/data", json1);
  }
}

void btnOff_Press(void *ptr) {
  if (flag_on == 1) {
    hmi_press.Set_background_color_bco(54970);
    set_press = 0;
    flag_on = 0;
    hmi_press.setValue(set_press);
    json1.set("/vent_state", flag_on);
    Firebase.updateNode(firebaseData, "/data", json1);
    json1.set("/press_state", 0);
    Firebase.updateNode(firebaseData, "/data", json1);
  }
}

void btn5_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 5;
    hmi_press.setValue(set_press);
    json1.set("/press_state", 5);
    Firebase.updateNode(firebaseData, "/data", json1);
  }
}

void btn10_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 10;
    hmi_press.setValue(set_press);
    json1.set("/press_state", 10);
    Firebase.updateNode(firebaseData, "/data", json1);
  }
}

void btn15_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 15;
    hmi_press.setValue(set_press);
    json1.set("/press_state", 15);
    Firebase.updateNode(firebaseData, "/data", json1);
  }
}

void waiting() {
  for (bar = 0; bar < 100; bar++) {
    p0_j0.setValue(bar);
    delay(35);
  }
}
