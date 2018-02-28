

int fsrPin0 = 0;     
int fsrPin1 = 1;
int fsrPin2 = 2;
int fsrReading0;   
int fsrReading1;
int fsrReading2;

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  Serial.println("Analog reading fsr0 = ");
  Serial.println(fsrReading0);
  Serial.println("");     // the raw analog reading

  Serial.println("Analog reading fsr1 = ");
  Serial.println(fsrReading1);
  Serial.println("");     // the raw analog reading

  Serial.println("Analog reading fsr2 = ");
  Serial.println(fsrReading2);
  Serial.println("");     // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if ((fsrReading0 < 10) || (fsrReading1 < 10) || (fsrReading2 < 10)) {
    Serial.println(" - No pressure");
  } else if ((fsrReading0 < 200) || (fsrReading1 < 200) || (fsrReading2 < 200)) {
    Serial.println(" - Light touch");
  } else if ((fsrReading0 < 500) || (fsrReading1 < 500) || (fsrReading2 < 500)) {
    Serial.println(" - Light squeeze");
  } else if ((fsrReading0 < 800) || (fsrReading1 < 800) || (fsrReading2 < 800)) {
    Serial.println(" - Medium squeeze");
  } else {
    Serial.println(" - Big squeeze");
  }
  delay(2000);
}
