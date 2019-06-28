
#include <Arduino.h>
#include "Ultrasonic.h"
#include <Wire.h>
#include "rgb_lcd.h"
#include <WiFi.h>
#include <PubSubClient.h>

using namespace std;

Ultrasonic ultrasonic(14);
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

const char *ssid = "moxd-lab";
const char *password = "gf3heTS11c";

const char *mqttServer = "hivemq.dock.moxd.io";
const int mqttPort = 1883;

vector<char> mitbewohner;

WiFiClient espClient;
PubSubClient client(espClient);

//Server verbindung
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.print(topic);
  Serial.println();

  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    char str = (char)payload[i];
    mitbewohner.push_back(str);
  }

  Serial.println("----------------------");
}

void setup()
{
  Serial.begin(9600);
  Serial.println();

  pinMode(33, OUTPUT);

  // wifi connection
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // mqtt connection
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT");

    if (client.connect("ESP_Client"))
    {
      Serial.println("Connected!");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("webdeviot/bestboiz");
  client.publish("webdeviot/bestboiz", "123");

  Serial.begin(9600);
  lcd.begin(16, 2, 17, 16);

  lcd.setRGB(colorR, colorG, colorB);
  // pinMode(6, OUTPUT);

  // Schieberegler
  Serial.begin(9600); // init serial to 9600b/s
}
void loop()
{

  long RangeInCentimeters;

  Serial.println("The distance to obstacles in front is: ");

  RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  if (RangeInCentimeters < 10)
  {
    Serial.println("Müll raus bringen");
    // digitalWrite(6, HIGH);
    // delay(1000);
    // digitalWrite(6, LOW);
    delay(1000);
  }
  else
  {
    Serial.print(RangeInCentimeters); //0~400cm
    Serial.println(" cm Alles im Grünen Bereich");
    delay(1000);
  }

  if (RangeInCentimeters < 10)
  {
    lcd.clear();
    lcd.setRGB(255, 0, 0);
    lcd.println("Chris");
    lcd.setCursor(0, 1);
    lcd.println("Muell voll!");
    digitalWrite(33, HIGH);
    delay(1000);
    digitalWrite(33, LOW);
    delay(1000);
  }

  else if (RangeInCentimeters < 30)
  {

    lcd.clear();
    lcd.setRGB(255, 69, 0);
    lcd.println("Muell fast voll");
  }
  else
  {

    lcd.clear();
    lcd.setRGB(0, 255, 0);
    lcd.println("Alles OK");
  }
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  // print the number of seconds since reset:

  delay(100);

  client.loop();
}
