#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>

const char* ssid = "YourWifiSSID";
const char* password = "YourWifiPassword";

const long utcOffsetInSeconds = 8 * 60 * 60;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(10);

  connectWifi();
  setNTPtoRTC();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  
  printRTCtime();
}

void connectWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int resetCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    resetCount++;
    if (resetCount > 10) {
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void printNTPTime() {
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.println(timeClient.getFormattedTime());
}

void setNTPtoRTC(){
  timeClient.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  timeClient.update();
  rtc.adjust(DateTime(2022, 8, 8, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
  printNTPTime();
}

void printRTCtime(){
  DateTime DT = rtc.now();
  Serial.print(DT.hour(), DEC); Serial.print(':'); Serial.print(DT.minute(), DEC); Serial.print(':'); Serial.print(DT.second(), DEC);
  Serial.println();
}