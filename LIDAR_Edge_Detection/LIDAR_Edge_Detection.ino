//**EDGE DETECTION PROGRAM**//
//By Pete Benjathatchaporn 
//Created Nov 9th, 2015

int analogInput = A0;//Input from the range sensor
float currentDistance = 0; //Current Reading of the range Sensor
float lastDistance = 0; //Last Reading of the range Sensor
float delta = 0;

void setup() 
{
  pinMode(analogInput, INPUT); // Initialize A0 as INPUT
  Serial.begin(9600);
}

void loop() 
{
  //Read the analog INPUT coming in from the range sensor
  int analogReading = analogRead(analogInput);

  //Convert Analog Reading to Distance in meters
  currentDistance = analogReading * ((5.0)/(1023));

  //Calculate the difference in Distance (delta)
  if (currentDistance < lastDistance)
  {
    //This ensures that delta is always Positive
    delta = lastDistance - currentDistance;
  }
  else 
  {
    delta = currentDistance -lastDistance;
  }
  
  //If delta is greater than 0.20 meters, EDGE is detected
  if(delta >= 0.20)
  {
    Serial.println("!!!!!EDGE DETECTED!!!!!");
  }

  delay(50);

}

//Serial.println(Distance);
