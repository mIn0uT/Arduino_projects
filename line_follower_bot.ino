#include <StackArray.h>

// VARIABLE DECLARATIONS --------------------------------------------------------------

// Color Sensor
#define s0 2
#define s1 3
#define s2 4
#define s3 5
#define sOut 12

// IR Sensor
#define f_IR A2
#define r_IR A1
#define l_IR A0

// Motor Speed (Manual Adjust)
#define MOTOR_SPEED_left 80
#define MOTOR_SPEED_right 102

//Right motor
int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
int enableLeftMotor=11;
int leftMotorPin1=9;
int leftMotorPin2=10;

// Short Path Array
int SPArray[100];

// Boolean Variables
bool isFirstLoopFinished, isStarted;

// StackArray Variables
StackArray <int> dirStack, SPStack;

// SPA (NEEDS TO BE ON TOP) --------------------------------------------------------------

StackArray<int> shorterPath(StackArray <int> &stack){
  StackArray <int> newStack, reverseStack;
  int errorCount=1;

  // reverse the original Stack
  while(!stack.isEmpty()){
    reverseStack.push(stack.pop());
  }

  // if error count is greater than zero, loop the algorithm until error count is zero
  while(errorCount>0){
      errorCount = 0;

      // if reverseStack is empty, reverse the newStack on it
      if (reverseStack.isEmpty()){
        while(!stack.isEmpty()){
          reverseStack.push(newStack.pop());
        }
      }

      newStack.push(reverseStack.pop());
      
      // While reverseStack is not empty, push the value into the new stack
      while (!reverseStack.isEmpty()){

        // If 2 consecutive numbers has the opposite direction, remove both values and add error count
        if(abs(reverseStack.peek()-newStack.peek())==2){
          reverseStack.pop();
          newStack.pop();
          errorCount += 1;
        }

        // push the last value in the reverse Stack
        newStack.push(reverseStack.pop());
     }
  }

  return newStack;
}

// MAIN CODE --------------------------------------------------------------

void setup() {
  
  // Color sensor setup -----------------------------------------------
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sOut, INPUT);

  // Pin Setup Run
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(f_IR, INPUT);
  pinMode(r_IR, INPUT);
  pinMode(l_IR, INPUT);
  
  // Color Frequency Setup
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);

   // Startup Declaration
  isFirstLoopFinished == false;
  isStarted == false;

  // Baud Rate
  Serial.begin(9600); 
}

void loop() {
  // Enable Motors
  enableMotors(MOTOR_SPEED_right, MOTOR_SPEED_left);
  
  // If color sensor detects red, declare started as true
  if (colorDetect('b')){
    isStarted = true;
    dirStack.push(0); // Push the first forward direction in the stack
    moveForward();
    delay(500);
  }
  
  if (isStarted){
    // if it is first loop, then run first loop code
    if (!isFirstLoopFinished){
      firstLoop();
    }
    // else run second loop code
    else { 
      secondLoop();
    }
  }

}

// MAIN LOOPS --------------------------------------------------------------
void firstLoop(){
  // if color sensor detects blue, stop and save stack
  if (colorDetect('r')){
    isFirstLoopFinished == true;
    isStarted == false;
    SPStack = shorterPath(dirStack);
    moveStop();
    delay(2000);
    return;
  }

  // if nothing detected on IRs, turn back
  if (!isLeftDetect() && !isRightDetect() && !isForwardDetect()){
    turnLeft();
    dirStack.push(abs((dirStack.peek()+2)%4)); //push the back dir (last element + 2) to the stack
    delay(200); // delay for 200 ms
    //While forward sensor doesn't detect anything, keep turning
    while(!isForwardDetect()){ 
      turnLeft();
    }
    
    // if it does detect, move forward
    forwardAdjust();
    return;
  }

  // if left IR detects, turn Left
  if(isLeftDetect()){
    turnLeft();
    dirStack.push(abs((dirStack.peek()+3)%4)); //push the left dir (last element - 1) to the stack
    delay(200); // delay for 200 ms
    //While forward sensor doesn't detect anything, keep turning
    while(!isForwardDetect()){ 
      turnLeft();
    }
    
    forwardAdjust();
    return;
  }

  // if forward and right
  if (isForwardDetect() && isRightDetect()){
    moveForward();
    dirStack.push(abs(dirStack.peek()%4)); //push the left dir (last element - 1) to the stack
    delay(200); // delay for 200 ms
    return;
  }

  // if right IR detects, turn right
  if(isRightDetect()){
    turnRight();
    dirStack.push(abs((dirStack.peek()+1)%4)); //push the right dir (last element + 1) to the stack
    delay(200); // delay for 200 ms
    //While forward sensor doesn't detect anything, keep turning
    while(!isForwardDetect()){ 
      turnRight();
    }
    
    forwardAdjust();
    return;
  }

  moveForward();
}

void secondLoop(){
  // declare current IR index value
  int c_IR = 1;
  
  // Convert stack to array to read the array elements by order
  convertStackToArray(SPStack,SPArray);

  // if color sensor detects blue, stop
  if (colorDetect('r')){
    isStarted == false;
    moveStop();
    // SPStack = shorterPath(dirStack);
    return;
  }

  // if left or right sensor detects
  if (isLeftDetect() || isRightDetect()){

    // Subtract the current element of the array from the previous element
    switch(SPArray[c_IR]-SPArray[c_IR-1]){
      // Case 1: Turn Right
      case 1:
        turnRight();
        c_IR++;
        forwardAdjust();
        break;
        
      // Case 0: Move Forward
      case 0:
        moveForward();
        c_IR++;
        forwardAdjust();
        break;
        
      // Case -1: Turn Left
      case -1:
        turnLeft();
        c_IR++;
        forwardAdjust();
        break;
        
      // Default: Just Forward, No Special Powers needed
      default:
        moveForward();
        break;
    }
  }
}



// MOVEMENT/DIRECTIONS --------------------------------------------------------------

void moveForward(){
  digitalWrite(rightMotorPin1,HIGH);
  digitalWrite(rightMotorPin2,LOW);    
  digitalWrite(leftMotorPin1,HIGH);
  digitalWrite(leftMotorPin2,LOW); 
}

void turnLeft(){
  digitalWrite(rightMotorPin1,HIGH);
  digitalWrite(rightMotorPin2,LOW);    
  digitalWrite(leftMotorPin1,LOW);
  digitalWrite(leftMotorPin2,HIGH); 
}

void turnRight(){
  digitalWrite(rightMotorPin1,LOW);
  digitalWrite(rightMotorPin2,HIGH);    
  digitalWrite(leftMotorPin1,HIGH);
  digitalWrite(leftMotorPin2,LOW);
}

void moveStop(){
  digitalWrite(rightMotorPin1,LOW);
  digitalWrite(rightMotorPin2,LOW);    
  digitalWrite(leftMotorPin1,LOW);
  digitalWrite(leftMotorPin2,LOW); 
}

void enableMotors(int rightMotorSpeed, int leftMotorSpeed){
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed)); 
}

// SENSOR FUNCTIONS --------------------------------------------------------------

bool isForwardDetect(){
  if (f_IR < 500){
    return true;
  }
  return false;
}

bool isRightDetect(){
  if (r_IR < 500){
    return true;
  }
  return false;
}

bool isLeftDetect(){
  if (l_IR < 500){  
    return true;
  }
  return false;
  
}


// OTHER FUNCTIONS --------------------------------------------------------------

bool colorDetect(char str1){
  int r,b;
   
   digitalWrite(s2,LOW);        //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
   digitalWrite(s3,LOW);
   r=pulseIn(sOut,LOW);
    
   digitalWrite(s2,LOW);
   digitalWrite(s3,HIGH);
   b=pulseIn(sOut,LOW);

    if (str1 == 'r'){
      if(r<b&r>5) return true;
      return false;
    }
    else if(str1 == 'b'){
      if(b<r&b>5) return true;
      return false;
    }
}

void forwardAdjust(){
    moveForward();
    delay(500);
}

void convertStackToArray(StackArray <int> stack, int f_array[]){
  StackArray <int> reverseStack;
  int i = 0;

  // reverse the stack
  while(!stack.isEmpty()){
    reverseStack.push(stack.pop());
  }

  while(!reverseStack.isEmpty()){
    f_array[i] = reverseStack.pop();
    i++;
  }
}
