void calcBatteryMinMax(float &batteryMinVoltage, float &batteryMaxVoltage) {
  batteryMinVoltage = lipoCellCount * lipoCellVoltMin;
  batteryMaxVoltage = lipoCellCount * lipoCellVoltMax;
}

void getBatteryDetails(int &batteryVoltage, float &batteryPercent, int batteryMinVoltage, int batteryMaxVoltage) {

  int batteryVoltage_pin_raw;

  batteryVoltage_pin_raw = analogRead(batteryVoltage_pin);  // read battery voltage pin

  batteryVoltage = mapfloat(batteryVoltage_pin_raw, analogReadMin, analogReadMax, 0, batteryMaxVoltage);

  // Need to do two maps for batterPercent
  batteryPercent = mapfloat(batteryVoltage_pin_raw, analogReadMin, analogReadMax, batteryMinVoltage, batteryMaxVoltage);
  batteryPercent = mapfloat(batteryPercent, batteryMinVoltage, batteryMaxVoltage, 0, 100);

}
