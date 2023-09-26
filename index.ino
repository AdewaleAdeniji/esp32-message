#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

const char* ssid = "OLUWAFERANMI_5G";
const char* password = "JESU1213";

LiquidCrystal_I2C lcd(0x27, 16, 2); // Define the LCD's address, columns, and rows

int counter = 5; // Initial counter value

void makeAPICall() {
  HTTPClient http;
  lcd.clear(); // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("loading...... ");
  http.begin("https://esp32.up.railway.app/queue/single/esp");

  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    //lcd.clear(); // Clear the LCD
    lcd.setCursor(0, 0);
    // lcd.print("GOTTEN BODY ");
    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    int queueIndex = doc["queueIndex"];
    String name = doc["name"];
    String message = doc["message"];

    // Display information on LCD
    lcd.setCursor(0, 0);
    // lcd.print(counter);
    // lcd.print("-");
    lcd.print(queueIndex + 1);
    lcd.print(":");
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(message);
    delay(3000);
    scrollText(name, message);
    // Print to Serial
    Serial.print("Message: ");
    Serial.println(message);
    Serial.print("Name: ");
    Serial.println(name);
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}
void clearRow(int row) {
  lcd.setCursor(0, row); // Set the cursor to the beginning of the specified row
  for(int i=0; i<16; i++) {
    lcd.print(" "); // Print spaces to clear the row
  }
}
void scrollText(String topText, String text) {
  int textLength = text.length();
  for (int i = 0; i <= textLength - 16; i++) {
    lcd.clear();  // Clear the screen
    lcd.setCursor(0, 0);
    lcd.print(topText);
    lcd.setCursor(0, 1);  // Set cursor to start of first row
    lcd.print(text.substring(i, i+16));  // Print the substring of text
    delay(500);  // Adjust this delay to control scrolling speed
  }
  delay(1000);  // Pause at the end of scrolling
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting up....");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  blinkLed(2);
  makeAPICall();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    counter--;
    if (counter <= 0) {
      counter = 5;
      makeAPICall();
       // Reset the counter
    }
  }
  blinkLed(1);
}

void blinkLed(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}
