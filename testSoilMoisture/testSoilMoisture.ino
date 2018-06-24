int ASignal1 = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ASignal1, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(ASignal1));
  delay(5000);
}
