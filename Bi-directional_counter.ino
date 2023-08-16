#include <NewPing.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

NewPing ultrasonicSensor(2,3,400);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#define SENSOR_A 7
#define SENSOR_B 8
#define LED_PIN 6
#define BUZZER_PIN 5
#define CUT_OFF_TEMPERATURE 33  //This is temperature in degree celsius
#define DISTANCE_TO_CHECK 10    //This is in cm
int overcrowded = 10;

int count = 0;

// Sensor A states
boolean sensorATriggered = false;
unsigned long sensorATime;
byte currStateA;
byte prevStateA = HIGH;

// Sensor B states
boolean sensorBTriggered  = false;
unsigned long sensorBTime;
byte currStateB ;
byte prevStateB = HIGH;


void setup () 
{
  mlx.begin();
  lcd.begin(16,2);
  Serial.begin (9600);
  
  lcd.backlight();
  lcd.setCursor(0,0);
  //lcd.print("Temp.: ");
  //lcd.setCursor(0,1);
  lcd.print("Visitor Count");
  
  pinMode (SENSOR_A, INPUT);
  pinMode (SENSOR_B, INPUT);
  pinMode (LED_PIN, OUTPUT);
  analogWrite(BUZZER_PIN, 0);

}

void tempHighTone() 
{ 
  for (int i = 0; i < 10; i++)
  {
    analogWrite(BUZZER_PIN,100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    analogWrite(BUZZER_PIN,0);
    digitalWrite(LED_PIN, LOW);
    delay(100);    
  }
}

void tempOkTone()
{
  analogWrite(BUZZER_PIN, 100);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
}

void loop () 
{
  // Temperature detection with Ultrasonic sensor
  int distance = ultrasonicSensor.ping_cm();
  if (distance > 0 && distance < DISTANCE_TO_CHECK)
  {
    delay(1000);
    float temperature = mlx.readObjectTempC();
    int distance = ultrasonicSensor.ping_cm();
    Serial.println(temperature);
    delay(500);
    if (distance > 0 && distance < DISTANCE_TO_CHECK && temperature <= CUT_OFF_TEMPERATURE) // low temp, can enter
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp.: ");
      lcd.print(float(temperature));
      tempOkTone();
      lcd.setCursor(0,1);
      lcd.print("You can enter");
      delay(2000);
      lcd.clear();
    }
    else if (distance > 0 && distance < DISTANCE_TO_CHECK && temperature > CUT_OFF_TEMPERATURE) // high temp, cannot enter
    {
      lcd.clear();  
      lcd.setCursor(0, 0);
      lcd.print("Temp.: ");
      lcd.print(float(temperature));
      tempHighTone();
      lcd.setCursor(0,1);
      lcd.print("You cannot enter");
      delay(2000);
      lcd.clear();
    }
    else
    {
      //tempHighTone();
      //delay(2000);
    }
    
    // re-display bottom line after temp detection
    lcd.setCursor(0,0);
    lcd.print("Visitor Count");
    lcd.setCursor(8,1);
    Serial.println(count);
    delay(500);
  }

  // IR sensor detection logic
  currStateA = digitalRead(SENSOR_A);
  if (currStateA != prevStateA) // execute code if there is reading in sensorA 
  {
       if (currStateA == LOW)
       {
          sensorATriggered = true;
          sensorATime = millis ();
       }
  }

// prevStateA = currStateA;

  lcd.setCursor(8,1);
 currStateB = digitalRead(SENSOR_B);
 if (currStateB != prevStateB)// execute code if there is reading in sensorB 
 {
   if (currStateB == LOW)
   {
      sensorBTriggered = true;
      sensorBTime = millis ();
   }
 }

// prevStateB = currStateB;

 if (sensorBTriggered && sensorATriggered)
  {
    if (sensorATime > sensorBTime) // sensor A is first to trigger -- enter mode -- add count
    {
      count ++;
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(LED_PIN, HIGH);
        analogWrite(BUZZER_PIN,100);
        delay(50);
        analogWrite(BUZZER_PIN,0);
        delay(50);
        digitalWrite(LED_PIN, LOW);
      }
      Serial.println(count);
      delay(500);
    }
    if (sensorBTime > sensorATime) // sensor B is first to trigger -- exit mode -- minus count
    {
      count--;
      for (int i = 0; i < 1; i++)
      {
        digitalWrite(LED_PIN, HIGH);
        analogWrite(BUZZER_PIN,100);
        delay(500);
        analogWrite(BUZZER_PIN,0);
        digitalWrite(LED_PIN, LOW);
      };
      Serial.println(count);
      delay(500);
    }
    sensorATriggered = false;
    sensorBTriggered  = false;
  }
  
  if (count <= overcrowded) 
  {
    lcd.setCursor(0,0);
    lcd.print("Visitor Count");
    lcd.setCursor(8,1);
    lcd.print(count);
    lcd.print(" ");
  }
  else if( count > overcrowded)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WARNING");
    lcd.setCursor(0,1);
    lcd.print("OVERCOWDED!");
    for (int i = 0; i < 3; i++)
      {
        digitalWrite(LED_PIN, HIGH);
        analogWrite(BUZZER_PIN,100);
        delay(50);
        analogWrite(BUZZER_PIN,0);
        digitalWrite(LED_PIN, LOW);
        delay(50);
      }
     lcd.clear();
  }
  else
  {}
}
  
