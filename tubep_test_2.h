#include <Arduino.h>
#include "AppDebug.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"


#define LED_TEST D4     // D0 on board
#define LED_TEST_MOTOR  D8 // D4 onchip GPIO2
#define PIN_CONFIG D3       // D3 flash GPIO0
#define PIN_PUL_MOTOR D0    //GPIO16 D0
#define PIN_DIR_MOTOR D5    //GPIO14
#define PIN_ENCODER_MOTOR D6 //1 TX    //D6 GPIO12    
#define PIN_LED_RED D2  //GPIO4
#define PIN_LED_GREEN D7    //GPIO13
#define PIN_LED_BLUE D1     //GPIO5
#define sample_time 25      //thoi gian lay mau la 25 ms de tinh van toc
#define SPEED_DEFAUT 120    //van toc dong co khi hoat dong binh thuong
#define ERROR_SPEED 50      //sai so cho phep cua van toc, neu nam ngoai khoang nay thi coi nhu co vat can

#define RESPONSE_LENGTH 512     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 32
#define EEPROM_WIFI_MAX_CLEAR 512
#define EEPROM_WIFI_PASS_START 33
#define EEPROM_WIFI_PASS_END 64
#define EEPROM_WIFI_IP_START 65
#define EEPROM_WIFI_IP_END 95
#define EEPROM_WIFI_LED_RED_START 100
#define EEPROM_WIFI_LED_RED_END 105
#define EEPROM_WIFI_LED_GREEN_START 106
#define EEPROM_WIFI_LED_GREEN_END 110
#define EEPROM_WIFI_LED_BLUE_START 111
#define EEPROM_WIFI_LED_BLUE_END 115


#define SSID_PRE_AP_MODE "AvyInterior-"
#define PASSWORD_AP_MODE "123456789"

#define HTTP_PORT 80

#define CONFIG_HOLD_TIME 3000

ESP8266WebServer server(HTTP_PORT);



// Ticker tickerSetMotor;
// Ticker tickerSpeed;
// Ticker tickerSetApMode;


//normal mode
void handleOk();
void handleRoot();
void GoUp();
void GoDown();
void Stop();
void Action();
void ControlLed();
void clearEeprom();
void SetupNomalMode();
void SetupNetwork();
void StartNormalSever();
void setPulMotor();
void caculate_Speed();
void tickerupdate();

//Config Mode
void checkButtonConfigClick();      //kiem tra button
void SetupConfigMode();             //phat wifi
void StartConfigServer();           //thiet lap sever
void ConfigMode();                  //nhan data tu app
void setLedApMode();                //hieu ung led
String GetFullSSID();
bool connectToWifi(String nssid, String npass, String ip);
bool testWifi();
void detachIP(String ip);
void setupIP();
void handleInterruptMotor();
unsigned long ConfigTimeout;
uint8_t first_octet;
uint8_t second_octet;
uint8_t third_octet;
uint8_t fourth_octet;
uint8_t red_before, red_after;
uint8_t green_before, green_after;
uint8_t blue_before, blue_after;
bool flag_led = true;
int Pul_Encoder = 0;
int pre_Pul_Encoder = 0;
float speed;

// unsigned long Pul_Motor;
// unsigned long test_time, time_start_speed;


Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
//Ticker tickerSpeed(caculate_Speed, 25, 0);  //every 25ms
Ticker tickerSetMotor(setPulMotor, 500, 0, MICROS_MICROS);  //every 0.5ms
