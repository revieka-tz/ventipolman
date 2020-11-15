void btnOn_Press(void *ptr) {
  if (flag_on == 0) {
    hmi_press.Set_background_color_bco(34800);
    set_press = 0;
    flag_on = 1;
    hmi_press.setValue(set_press);
  }
}

void btnOff_Press(void *ptr) {
  if (flag_on == 1) {
    hmi_press.Set_background_color_bco(54970);
    set_press = 0;
    flag_on = 0;
    hmi_press.setValue(set_press);
  }
}

void btn5_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 5;
    hmi_press.setValue(set_press);
  }
}

void btn10_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 10;
    hmi_press.setValue(set_press);
  }
}

void btn15_Press(void *ptr) {
  if (flag_on == 1) {
    set_press = 15;
    hmi_press.setValue(set_press);
  }
}

void waiting() {
  for (bar = 0; bar < 100; bar++) {
    p0_j0.setValue(bar);
    delay(35);
  }
}
