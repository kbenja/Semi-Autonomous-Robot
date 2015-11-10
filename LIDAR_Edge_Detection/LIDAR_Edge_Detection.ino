//**EDGE DETECTION PROGRAM**//
//By Pete Benjathatchaporn, Daniel Gorelick
//Created Nov 9th, 2015

int analogInput = A0;//Input from the range sensor
float currentDistance; //Current Reading of the range Sensor
float lastDistance; //Last Reading of the range Sensor
float delta = 0;

int state = 0; //Variable for state machine
bool printed = false; //Boolean for printing statement

float deltaDesired = 0.10; //Currently set at 0.15 meters - Less = More Sensitive
float distanceDesired = 0.30; 
float distanceError = 0.05; //Error of margin for distance

void setup() 
{
  pinMode(analogInput, INPUT); // Initialize A0 as INPUT
  Serial.begin(9600);
}

bool detectEdge()
{
  //Calculate the difference in Distance (delta)
  if (currentDistance < lastDistance)
  {
    //This ensures that delta is always Positive
    delta = lastDistance - currentDistance;
  }
  else 
  {
    delta = currentDistance - lastDistance;
  }
  
  //If delta is greater than X meters, EDGE is detected
  if (delta != currentDistance)
  {
    if(delta >= deltaDesired)
    {
      return true;
    }
  }  
}

bool detectDistance(float desired)
{
  if(currentDistance + distanceError/2 > desired && currentDistance - distanceError/2 < desired)
  {
    return true;
  }
}

void loop() 
{
  //Read the analog INPUT coming in from the range sensor
  int analogReading = analogRead(analogInput);

  //Convert Analog Reading to Distance in meters
  currentDistance = analogReading * ((5.0)/(1023));

  /*  State machine
   *  1: move forward within certain range
   *  2: pass first edge
   *  3: pass second edge
   *  4: go back into the middle
   *  5: line up between the edges 
   *  6: move forward to certain range
   */
   
  switch(state){
    case 0:
      if (!printed){
        Serial.println("Find first edge");
        printed = true;
      }
      if (detectEdge()) {
        Serial.println("EDGE DETECTED");
        state = 1;
        printed = false;
      } 
    case 1:
      if(!printed){
        Serial.println("Move to 0.30 meters");
        printed = true;
      }    
      if (detectDistance){
        Serial.println("CORRECT DISTANCE");
        state = 0;
        printed = false;
      } else {
        Serial.print("CURRENT DISTANCE = ");
        Serial.print(currentDistance);
        Serial.print("\r");
      }
  } 
  
  lastDistance = currentDistance;

  delay(100); //Delay dictates time limit before lastDistance gets overwrite
}
