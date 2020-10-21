void btnPress_Release(void *ptr) {
  if (flagBtnPress == 0) {
    flagBtnPress = 1;
  }
  else {

    flagBtnPress = 0;
  }
  //  Firebase.setInt(firebaseData, "btnPress", flagBtnPress);
}

void waiting() {
  for (bar = 0; bar < 100; bar++) {
    p0_j0.setValue(bar);
    delay(35);
  }
}
