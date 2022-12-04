//  AutomaticSoapDispenser, Automatic Soap Dispenser made with Arduino MKR1000
//  Copyright (C) 2022 BurgersMcFly

//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <WiFi101.h>
#include <RTCZero.h>
#include <LiquidCrystal.h>

RTCZero rtc;

char ssid[] = "1234";      // network SSID (name) between the " "
char pass[] = "1234";  // network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int flag = 0;
int status = WL_IDLE_STATUS;  //connection status
const int GMT = 2;            //change this to adapt it to your time zone
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
WiFiServer server(80);  //server socket

WiFiClient client = server.available();

void setup() {

  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("Starting");
  lcd.setCursor(7, 1);
  lcd.print("up");
  enable_WiFi();
  connect_WiFi();
  printWifiStatus();
  rtc.begin();

  unsigned long epoch;

  int numberOfTries = 0, maxTries = 6;

  do {

    epoch = WiFi.getTime();

    numberOfTries++;

  }

  while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {

    Serial.print("NTP unreachable!!");

    while (1)
      ;

  }

  else {

    Serial.print("Epoch received: ");

    Serial.println(epoch);

    rtc.setEpoch(epoch);

    Serial.println();
  }
}

void loop() {

  if (status = WL_CONNECTED) {
    printWEB();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void enable_WiFi() {

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
}

void printWEB() {

  printTime();
  printDate();
  delay(1000);
}

void printTime() {

  lcd.setCursor(4, 1);
  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  lcd.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  lcd.print(":");
  print2digits(rtc.getSeconds());
  Serial.println();
}

void printDate() {

  lcd.setCursor(4, 0);
  Serial.print(rtc.getDay());
  lcd.print(rtc.getDay());
  Serial.print("/");
  lcd.print("/");
  Serial.print(rtc.getMonth());
  lcd.print(rtc.getMonth());
  Serial.print("/");
  lcd.print("/");
  Serial.print(rtc.getYear());
  lcd.print(rtc.getYear());
  Serial.print(" ");
}

void print2digits(int number) {

  if (number < 10) {

    Serial.print("0");
    lcd.print("0");
  }

  Serial.print(number);
  lcd.print(number);
}
