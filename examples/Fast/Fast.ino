#include <ESP8266_NII_DRIVER.hpp>

const char *ssid = "Wifi-ssid";
const char *passPhrase = "Wifi-pass";

void setup() {

  nii::backend::setup(ssid, passPhrase, [] () {

    nii::Router::builder()->path("/")->call([] () {
        return new nii::PlainTextResponse("Hello world!");
    });

  });

}

void loop() {
  nii::backend::update();
}




