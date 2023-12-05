/*
Google Apps Script:

var sheetID = '';  // Replace with your actual spreadsheet ID

function doGet(e) {
  var temperature = e.parameter.temperature;
  var humidity = e.parameter.humidity;
  var sheet = SpreadsheetApp.openById(sheetID).getActiveSheet();
  
  var dateTime = new Date();
  var currentDate = Utilities.formatDate(dateTime, "Europe/London", "dd/MM/yyyy");  // Replace Timezone ID and format
  var currentTime = Utilities.formatDate(dateTime, "Europe/London", "HH:mm:ss");  // Replace Timezone ID and format

  sheet.appendRow([currentDate, currentTime, temperature, humidity]);

  return ContentService.createTextOutput('Data logged successfully');
}


IMPORTANT for wokwi.com simulation!!
add "attrs": { "fastTLS": "1" } in diagram.json for esp32 board
*/

#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 4          // DHT22 sensor pin
#define DHTTYPE DHT22     // DHT22 sensor type

const char* ssid = "Wokwi-GUEST"; // Wifi name
const char* password = "";        // Wifi password
const char* webAppUrl = "https://script.google.com/macros/s/.../exec";  // Replace with your actual web app URL

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to Google Apps Script
  sendDataToScript(temperature, humidity);

  delay(5000); // Delay for 5 seconds
}

void sendDataToScript(float temperature, float humidity) {
  HTTPClient http;

  // Your server address
  String serverPath = String(webAppUrl) + "?temperature=" + String(temperature) + "&humidity=" + String(humidity);

  Serial.print("Connecting to server: ");
  Serial.println(serverPath);

  // Send HTTP GET request
  if (http.begin(serverPath)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.print("Server response code: ");
      Serial.println(httpCode);
    } else {
      Serial.print("HTTP GET request failed with error code: ");
      Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("Unable to connect to the server");
  }
}
