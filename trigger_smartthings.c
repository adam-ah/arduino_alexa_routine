/* Paste this into your Arduino IDE, change the below variables to correct values.
 * When the device is up and running, press the BOOT button to trigger your Alexa routine.
 * The code is for ESP32.
 */
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "YOUR_NETWORK_SSID";
const char *password = "YOUR_NETWORK_PASSWORD";

const char *personal_token = "YOUR_PERSONAL_SMARTTHINGS_TOKEN";
const char *device_id = "YOUR_SMARTTHINGS_DEVICE_GUID";

const char *url_pattern = "https://api.smartthings.com/v1/devices/%s/commands";
const char *bearer_pattern = "Bearer: %s";

const int BUTTON_PIN = 0;
const int LED_PIN = 2;

char *url;
char *bearer;

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    url = (char *)malloc(strlen(url_pattern) + strlen(device_id));
    bearer = (char *)malloc(strlen(bearer_pattern) + strlen(personal_token));

    sprintf(url, url_pattern, device_id);
    sprintf(bearer, bearer_pattern, personal_token);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(LED_PIN, HIGH);

        if (digitalRead(BUTTON_PIN) == LOW)
        {
            while (digitalRead(BUTTON_PIN) == LOW)
                ; // Wait for button to be released
            HTTPClient http;

            http.begin(url);

            http.addHeader("Content-Type", "application/json");
            http.addHeader("Authorization", bearer);
            String httpRequestData = "[{\"component\":\"main\",\"capability\":\"switch\",\"command\":\"on\"}]";

            int httpResponseCode = http.POST(httpRequestData);

            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);

            http.end();

            digitalWrite(LED_PIN, LOW);
            delay(100);
            digitalWrite(LED_PIN, HIGH);
        }
    }
    else
    {
        Serial.println("WiFi Disconnected");
        digitalWrite(LED_PIN, LOW);
        delay(5000);
    }
    delay(50);
}

