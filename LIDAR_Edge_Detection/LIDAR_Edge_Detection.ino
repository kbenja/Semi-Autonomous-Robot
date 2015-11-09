//**EDGE DETECTION PROGRAM**//
//By Pete Benjathatchaporn 
//Created Nov 9th, 2015

int analogInput = A0;//Input from the range sensor
float currentDistance; //Current Reading of the range Sensor
float lastDistance; //Last Reading of the range Sensor
float delta = 0;

//This can be change depending on the size of the Breaker
float deltaDesired = 0.15; //Currently set at 0.20 meters - Less = More Sensitive

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

  //Serial.println(currentDistance);

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
  
  //If delta is greater than X meters, EDGE is detected
  if (delta != currentDistance)
  {
    if(delta >= deltaDesired)
    {
      Serial.println("!!!!!EDGE DETECTED!!!!!");
    }
  }

  delay(100); //Delay dictates time limit before lastDistance gets overwrite

  lastDistance = currentDistance;

}
