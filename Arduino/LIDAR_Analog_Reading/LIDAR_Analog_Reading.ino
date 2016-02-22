int(an_re) = A0;
int red = 7;
int orange = 8;
int yellow = 12;
int green = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(orange, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(an_re, INPUT);
  Serial.begin(9600);
}

void loop() {
    int anval = analogRead(an_re);
    float voltage = anval * ((5.0)/(1023));
    Serial.print(voltage);
    Serial.print('\n');
    delay (10);

    if(voltage > 0.0 && voltage <=0.5){
      digitalWrite(red, HIGH);
      digitalWrite(orange, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
    }
    else if(voltage >= 0.51 && voltage <= 1.0){
      digitalWrite(red, LOW);
      digitalWrite(orange, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
    }
    else if(voltage >= 1.01 && voltage <= 1.75){
      digitalWrite(red, LOW);
      digitalWrite(orange, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);
    }
    else if(voltage >= 1.76){
      digitalWrite(green, HIGH);
      digitalWrite(orange, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);
    }

}


