// Header files
#include <WiFi.h>
#include <ThingSpeak.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Network credentials
const char* ssid = "PLDTHOMEFIBRgBH8D";
const char* password = "PLDTWIFIfyT37";
WiFiClient client;

// ThingSpeak credentials
unsigned long channelID = 2177826;
const char* writeAPIKey = "TEIIUYGSJQ35FCOG";

// Sensors pin
#define tempPin 33
#define tdsPin 32
#define phPin 35
#define ammoniaPin 34

// LCD setup
LiquidCrystal_I2C lcd(0x27 ,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

// DS18B20 temperature setup
OneWire oneWire(tempPin);
DallasTemperature DS18B20(&oneWire);
float tempC; // temperature in Celsius

// PH-4502C setup
int samples = 10;
float adc_resolution = 4095.0; //ESP 32 ADC Resolution

float ph (float voltage) {
    return 7 + ((2.50 - voltage) / 0.18);
}

// DO setup
float DOX_DATA;

// Ammonia setup
#define RL 47
#define m -0.243
#define b 0.323
#define Ro 30

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectToWiFi();
  
  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize LCD
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  String message = "IOT-Based Smart Fish Cultivation System";
  lcd.print("Welcome!");
  // Scroll the text to the left
  for (int i = 0; i < message.length() + 16; i++) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(message.substring(i, i + 16)); // Display 16 characters at a time
    delay(200); // Adjust this value to control scrolling speed
  }
  lcd.clear();
  lcd.print("Sensor values");
  lcd.setCursor(0, 1);
  lcd.print("updating...");
  
  // Initialize Temperature
  DS18B20.begin();
  
  // Initialize TDS
  pinMode(tdsPin,INPUT);
}

void loop() {
  // Read sensors value
  // Temperature
  DS18B20.requestTemperatures();
  tempC = DS18B20.getTempCByIndex(0);
  
  // TDS
  float tdsValue = analogRead(tdsPin);

  // pH
  int measurings=0;
  for (int i = 0; i < samples; i++){
      measurings += analogRead(phPin);
      delay(10);
  }
  float phVoltage = 3.3 / adc_resolution * measurings/samples;
  float phValue = ph(phVoltage);

  // Ammonia
  float VRL; //Voltage drop across the MQ sensor
  float RS_value; //Sensor resistance at gas concentration 
  float ratio; //Define variable for ratio
  VRL = analogRead(ammoniaPin)*(3.3/4095.0); //Measure the voltage drop and convert to 0-5V
  RS_value = ((5.0*RL)/VRL)-RL; //Use formula to get Rs value
  ratio = RS_value/Ro;  // find ratio Rs/Ro
  float ppm = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm

  // DO 
  float power = pow((0.053714 * (tempC) - 1.838331),2);
  DOX_DATA = 2 * power + ph(phVoltage);
    
  // Print sensor value to serial monitor for debugging purposes
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");
  Serial.print("TDS Value: ");
  Serial.println(tdsValue);
  Serial.print("pH Level: ");
  Serial.println(phValue);
  Serial.print("Ammonia ppm: ");
  Serial.println(ppm);
  Serial.print("Calculated DO: ");
  Serial.println(DOX_DATA);
  delay(2000);

  // Update ThingSpeak channel
  updateThingSpeak(tempC, tdsValue, phValue, ppm, DOX_DATA);

  // Update LCD display
  updateLCD(tempC, tdsValue, phValue, ppm, DOX_DATA);
  
  delay(5000);
}

void connectToWiFi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void updateThingSpeak(float tempC, float tdsValue, float phValue, float ppm, float DOX_DATA) {
  // Set the fields with sensor values
  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, tdsValue);
  ThingSpeak.setField(3, phValue);
  ThingSpeak.setField(4, ppm);
  ThingSpeak.setField(5, DOX_DATA);

  // Write to ThingSpeak
  int response = ThingSpeak.writeFields(channelID, writeAPIKey);

  // Check if write was successful
  if (response == 200) {
    Serial.println("Channel update successful");
    //digitalWrite(ledPin, HIGH);  // Turn on LED
  } else {
    Serial.println("Error updating channel");
    //digitalWrite(ledPin, LOW);   // Turn off LED
  }
}

void updateLCD(float tempC, float tdsValue, float phValue, float ppm, float DOX_DATA) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.setCursor(0,1);
  lcd.print(tempC);
  lcd.print(" C");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TDS: ");
  lcd.setCursor(0,1);
  lcd.print(tdsValue);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ammonia: ");
  lcd.setCursor(0,1);
  lcd.print(ppm);
  lcd.print(" ppm");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("pH level: ");
  lcd.setCursor(0,1);
  lcd.print(phValue);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DO level: ");
  lcd.setCursor(0,1);
  lcd.print(DOX_DATA);
  delay(2000);
}
