//**EDGE DETECTION PROGRAM**//
//By Pete Benjathatchaporn, Daniel Gorelick
//Created Nov 9th, 2015

int analogInput = A0;//Input from the range sensor
float currentDistance; //Current Reading of the range Sensor
float lastDistance; //Last Reading of the range Sensor
float delta = 0;

int state = 0; //Variable for state machine
bool printed = false; //Boolean for printing statement

float deltaDesired = 0.05; //Currently set at 0.15 meters - Less = More Sensitive
float distanceDesired = 0.30; 
float distanceError = 0.05; //Error of margin for distance

bool edgeDetected = false;
bool correctDistance = false;

int count = 0;
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
      edgeDetected = true;
    }
  }  
  else{
    edgeDetected = false;
  }
}

bool detectDistance(float desired)
{
  if(currentDistance + distanceError/2 > desired && currentDistance - distanceError/2 < desired)
//  if(currentDistance <= desired)
  {
    correctDistance = true;
  }
}

void space(){
  Serial.print("\n\n");
}

void loop() 
{
  //Read the analog INPUT coming in from the range sensor
  int analogReading = analogRead(analogInput);

  //Convert Analog Reading to Distance in meters
  currentDistance = analogReading * ((5.0)/(1023));
  
  /*  State machine
   *  1: move forward within certain range (0.15)
   *  2: pass first edge
   *  3: pass second edge
   *  4: go back into the middle
   *  5: line up between the edges 
   *  6: move forward to certain range
   */
  count = 1;
//  Serial.print(count);
//  Serial.println("Hello\r");
  switch(state){
    case 0:
      if (!printed){
        space();
        Serial.print("//////////////////WELCOME//////////////////\n\nFollow the instructions and wait for\nfeedback from the program.\n\n//////////////////STEP ONE//////////////////\n\nALIGN SENSOR with the black \"X\" then MOVE SENSOR to 0.1 meters. \n[CURRENT DISTANCE = ");
        Serial.print(currentDistance);
        Serial.print(" METERS]");
        printed = true;
      }
      detectDistance(0.1);
      if (correctDistance || count%6 == 0){
        space();
        Serial.print("\n\n[CORRECT DISTANCE]\n");
        state = 1;
        printed = false;
        correctDistance = false;
      }
      break;
    case 1:
      if (!printed){
        space();
        Serial.print("//////////////////STEP TWO//////////////////\n\nMOVE RIGHT and detect right side edge\n\n");
        space();
        printed = true;
      }
      detectEdge();
      if (edgeDetected || count%6 == 0){
        Serial.print("[CHECK] EDGE DETECTED\n");
        state = 2;
        printed = false;
        edgeDetected = false;
        }
        break;
    case 2:
      if (!printed){
        space();
        Serial.print("//////////////////STEP THREE//////////////////\n\nMOVE LEFT and detect left side edge");
        printed = true;
      }
      detectEdge();
      if (edgeDetected || count%6 == 0){
        space();
        Serial.print("\n\n[CHECK] EDGE DETECTED\n");
        state = 3;
        printed = false;
        edgeDetected = false;
        }
        break;
    case 3:
      if (!printed){
        space();
        Serial.print("//////////////////STEP FOUR//////////////////\n\nMOVE LEFT to pass left side edge");
        printed = true;
      }
      detectEdge();
      if (edgeDetected || count%6 == 0){
        space();
        Serial.print("[CHECK] EDGE DETECTED");
        state = 4;
        printed = false;
        edgeDetected = false;
        }
        break;
    case 4:
      if(!printed){
        space();
        Serial.print("//////////////////STEP FIVE//////////////////\n\nMove to 0.30 meters \n[CURRENT DISTANCE = ");
        Serial.print(currentDistance);
        Serial.print(" METERS]\n");
        printed = true;
      }    
      detectDistance(distanceDesired);
      if (correctDistance || count%6 == 0){
        space();
        Serial.println("///////////////////////////////////////////////////////////");
        Serial.println("\n[SUCCESS] You have successfully aligned with the breaker cabinet!\n");
        Serial.println("///////////////////////////////////////////////////////////\n");
        space();
        state = 5;
        printed = false;
        correctDistance = false;
      }
      break;
    default:
      break;
  } 
  
  lastDistance = currentDistance;

  delay(200); //Delay dictates time limit before lastDistance gets overwrite
}
