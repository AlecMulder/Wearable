#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

Adafruit_MLX90614 temp = Adafruit_MLX90614();
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(5, 6, NEO_GRB + NEO_KHZ800);
const int vib = 2;

float objectTemp;
void setup() {
  pinMode(vib, OUTPUT);
  Serial.begin(112500);
  temp.begin();

  pixels.begin();
  pixels.show();

}

void loop() {
  //read temp
  objectTemp = temp.readObjectTempC();
  
  //set all pixles to proper color
  for (int i = 0; i < 5; i++) {
    pixels.setPixelColor(i, tempColor(objectTemp));
  }
  pixels.show();
  
  //vibrations only if the temperature is >50 or <-15
  if (objectTemp > 50 || objectTemp < -15)vibration(true);
  else vibration(false);
  
  //debug
  Serial.print(objectTemp);
  Serial.print("    ");
  Serial.print(Red(pixels.getPixelColor(0)));
  Serial.print("|");
  Serial.print(Green(pixels.getPixelColor(0)));
  Serial.print("|");
  Serial.println(Blue(pixels.getPixelColor(0)));
}

////FUNCTIONS////

//color calculation based on the temperature
uint32_t tempColor (float temp) {
  uint32_t color = pixels.Color(0, 0, 0);//off
  if (temp > 100) {
    color = pixels.Color(255, 0, 0);//red
  } else if (temp <= 100 && temp > 50) {
    color = pixels.Color(255, map(temp, 50, 100, 255, 0), 0);//red to yellow
  } else if (temp <= 50 && temp > 30) {
    color = pixels.Color(map(temp, 30, 50, 0, 255), 255, 0);//yellow to green
  } else if (temp <= 30 && temp > 0) {
    color = pixels.Color(map(temp, 0, 30, 255, 0), 255, map(temp, 0, 30, 255, 0));//green to white
  } else if (temp <= 0 && temp > -15) {
    color = pixels.Color(map(temp, -15, 0, 255, 0), 255, 255);//white to teal
  } else if (temp <= -15 && temp > -30) {
    color = pixels.Color(0, map(temp, -30, -15, 0, 255), 255);//teal to blue
  } else {
    color = pixels.Color(0, 0, 255);//blue
  }
  return color;
}

//slightly faster way of changing vibration state
void vibration(boolean on) {
  if (on)digitalWrite(vib, HIGH);
  else digitalWrite(vib, LOW);
}

//get color values out of 32bit color
uint8_t Red(uint32_t color)
{
  return (color >> 16) & 0xFF;
}

uint8_t Green(uint32_t color)
{
  return (color >> 8) & 0xFF;
}

uint8_t Blue(uint32_t color)
{
  return color & 0xFF;
}

