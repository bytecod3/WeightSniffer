#include <Arduino.h>
#include "HX711.h"

#define GPIO_BUTTON             12 
#define GPIO_BUZZ               4
#define BUZZ_INTERVAL           100
#define HX711_DT                23              
#define HX711_SCK               18    

HX711 scale;

unsigned long long current_time = 0;
unsigned long long previous_time = 0;
long interval = 200;

void init_hw();
void buzz();
void read_load_cell();
void callibrate_load_cell();
uint8_t read_button();

void init_hw() {
  pinMode(GPIO_BUTTON, INPUT);
  pinMode(GPIO_BUZZ, OUTPUT);

  scale.begin(HX711_DT, HX711_SCK);
  
}

/**
 * buzz
 */
void buzz(void) {
    digitalWrite(GPIO_BUZZ, 1);
    delay(BUZZ_INTERVAL);
    digitalWrite(GPIO_BUZZ, 0);
    delay(BUZZ_INTERVAL);
}

/**
 * Button press reads the weight and sends to USB UART
 */
uint8_t read_button() {
    current_time = millis();
    uint8_t bv = 1;
    if ((current_time - previous_time) > interval ) {
        bv = digitalRead(GPIO_BUTTON);
        printf("%d\n", bv);
        previous_time = current_time;
    }

    return bv;
}

/**
 * callibrate ad cell
 */
void callibrate_load_cell() {
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000)
}

void setup() {
  init_hw();
}

void loop() {
  if(!read_button()) {
    buzz();
    printf("%.2f\n", 898.09);
 }
}

