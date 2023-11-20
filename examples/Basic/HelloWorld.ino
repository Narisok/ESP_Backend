
#include <Backend.hpp>

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(myfun());
  delay(1);  
}
