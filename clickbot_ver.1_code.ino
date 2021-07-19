

// https://www.youtube.com/watch?v=hvQzixrDLPo&ab_channel=Engineer1.0

//https://youyouyou.pixnet.net/blog/post/120260848-%E7%AC%AC%E5%85%AD%E7%AF%87-esp32%E9%A1%9E%E6%AF%94%E8%AE%80%E5%8F%96%28analogread%29%EF%BC%9A%E5%B0%8F%E5%A4%9C%E7%87%88

//https://examples.blynk.cc/?board=ESP32&shield=ESP32%20WiFi&example=GettingStarted%2FServo

//https://create.arduino.cc/projecthub/kiranpaul/light-magic-using-lm393-and-arduino-uno-14eadc

//https://github.com/jkb-git/ESP32Servo

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>


char auth[] = "1KMICAgOn8IWoTWBrrXbDRCrpxUD6Ji8";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ouhk_steam"; // Enter Your WiFi Name
char pass[] = "27686804"; // Enter Your Passwword

// GPIO where the DS18B20 is connected to
const int oneWireBus = 0;   

int light_state;

#define DHTTYPE DHT11
#define DHTPIN 15 // Output Pin Of DHT  
#define LightPin 17 // Input pin of light sensor


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensor()
{
  sensors.requestTemperatures(); 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  float body_temp = sensors.getTempCByIndex(0);
  
  if (isnan(h) || isnan(t) || isnan(body_temp))
    {
    Serial.println("Failed to read Data from sensors!");
    return;
    }
    else
    {
      
      Serial.print(t);
      Serial.println("ºC");
      Serial.println("----------------------------------");
      Serial.print(h);
      Serial.println("----------------------------------");
      Serial.println("ºC");
      Serial.print(body_temp);
      
    }
     // or dht.readTemperature(true) for Fahrenheit

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t); // Virtual Pin V5 for Temprature
  Blynk.virtualWrite(V6, h); // Virtual Pin V6 for Humidity
}

WidgetLED led1(V0);

void buttonLedWidget()
{
  // Read button
  light_state = digitalRead(LightPin);

  // If state has changed...
  if (light_state == LOW) {
      led1.on();
    } else {
      led1.off();
    }
}

Servo servo_switch;

BLYNK_WRITE(V2)
{
  servo_switch.write(param.asInt());
}


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  dht.begin();
  sensors.begin();
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, buttonLedWidget);
  Blynk.begin(auth, ssid, pass);
  servo_switch.attach(13);

}

  void loop()
{ 
  Serial.print("Light - "); 
  Serial.println(light_state);
  delay(1000);
    
  Blynk.run();
  timer.run();
}
