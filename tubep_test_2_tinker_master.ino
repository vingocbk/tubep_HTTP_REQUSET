#include "tubep_test_2.h"

void handleOk() {
    server.sendHeader("Access-Control-Allow-Headers", "*");
    server.sendHeader("Access-Control-Allow-Methods", "*");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
}

void handleRoot() {
    server.send(200, "text/html", "<h1>You are connected</h1>");
    ECHOLN("done!");
}


void GoUp(){
    // server.stop();
    // test_time = millis();
    // Pul_Motor = 0;
    digitalWrite(PIN_DIR_MOTOR, HIGH);          //chieu quay thuan
    //tickerSetMotor.attach_ms(1, setPulMotor);  //every 1ms
    tickerSetMotor.start();
    server.send(200, "text/html", "{\"status\":\"success\"}");
    ECHOLN("go_up");
}

void GoDown(){
    // server.stop();
    // test_time = millis();
    // time_start_speed = millis();
    // Pul_Motor = 0;
    digitalWrite(PIN_DIR_MOTOR, LOW);           //chieu quay nghich
    //tickerSetMotor.attach_ms(1, setPulMotor);  //every 1ms
    //tickerSpeed.attach_ms(25, caculate_Speed);  //every 25
    tickerSetMotor.start();
    //tickerSpeed.start();
    server.send(200, "text/html", "{\"status\":\"success\"}");
    ECHOLN("go_down!");
}

void Stop(){
    ECHOLN("Stop");
    server.send(200, "text/html", "{\"status\":\"success\"}");
    tickerSetMotor.stop();
}

void Action(){
    ECHOLN("Action");
    digitalWrite(PIN_ENCODER_MOTOR, HIGH);
    delay(2000);
    digitalWrite(PIN_ENCODER_MOTOR, LOW);
    server.send(200, "text/html", "{\"status\":\"success\"}");
}

void ControlLed(){
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    ECHOLN(server.arg("plain"));
    JsonObject& rootData = jsonBuffer.parseObject(server.arg("plain"));
    ECHOLN("--------------");
    
    if (rootData.success()) {
        // server.sendHeader("Access-Control-Allow-Headers", "*");
        // server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
        server.stop();
        String powerStr = rootData["power"];
        String redStr = rootData["red"];
        String greenStr = rootData["green"];
        String blueStr = rootData["blue"];
        float out_led_red, out_led_green, out_led_blue;


        if(powerStr == "off"){
            for(int i = 1; i<=255; i++){
                out_led_red = (float)red_before + (((float)0 - (float)red_before)/255)*i;
                out_led_red = abs(out_led_red);
                analogWrite(PIN_LED_RED, uint8_t(out_led_red));
                out_led_green = (float)green_before + (((float)0 - (float)green_before)/255)*i;
                out_led_green = abs(out_led_green);
                analogWrite(PIN_LED_GREEN, uint8_t(out_led_green));
                out_led_blue = (float)blue_before + (((float)0 - (float)blue_before)/255)*i;
                out_led_blue = abs(out_led_blue);
                analogWrite(PIN_LED_BLUE, uint8_t(out_led_blue));
                delay(10);
                
            }
            // red_before = 0;
            // green_before = 0;
            // blue_before = 0;   
            flag_led = false;
            server.begin();
            return;
        }else if(powerStr == "on"){
            for(int i = 1; i<=255; i++){
                out_led_red = (float)0 + (((float)red_after - (float)0)/255)*i;
                out_led_red = abs(out_led_red);
                analogWrite(PIN_LED_RED, uint8_t(out_led_red));
                out_led_green = (float)0 + (((float)green_after - (float)0)/255)*i;
                out_led_green = abs(out_led_green);
                analogWrite(PIN_LED_GREEN, uint8_t(out_led_green));
                out_led_blue = (float)0 + (((float)blue_after - (float)0)/255)*i;
                out_led_blue = abs(out_led_blue);
                analogWrite(PIN_LED_BLUE, uint8_t(out_led_blue));
                delay(10);
            }
            flag_led = true;
            server.begin();
            return;
        }else if(powerStr == "change" && flag_led == true){
            red_after = atoi(redStr.c_str());
            green_after = atoi(greenStr.c_str());
            blue_after = atoi(blueStr.c_str());
            for(int i = 1; i<=255; i++){
                out_led_red = (float)red_before + (((float)red_after - (float)red_before)/255)*i;
                out_led_red = abs(out_led_red);
                analogWrite(PIN_LED_RED, uint8_t(out_led_red));
                out_led_green = (float)green_before + (((float)green_after - (float)green_before)/255)*i;
                out_led_green = abs(out_led_green);
                analogWrite(PIN_LED_GREEN, uint8_t(out_led_green));
                out_led_blue = (float)blue_before + (((float)blue_after - (float)blue_before)/255)*i;
                out_led_blue = abs(out_led_blue);
                analogWrite(PIN_LED_BLUE, uint8_t(out_led_blue));
                delay(10);
            }
        }else{
            ECHOLN("turn on flag_led!");
            return;
        }

        red_before = red_after;
        green_before = green_after;
        blue_before = blue_after;
        
        ECHO("RED: ");
        ECHOLN(uint8_t(out_led_red));
        ECHO("GREEN: ");
        ECHOLN(uint8_t(out_led_green));
        ECHO("BLUE: ");
        ECHOLN(uint8_t(out_led_blue));
        //clear data EEPROM Led RGB
        for (int i = EEPROM_WIFI_LED_RED_START; i <= EEPROM_WIFI_LED_BLUE_END; ++i){
            EEPROM.write(i, 0);
        }
        ECHOLN("writing eeprom LED RED:");
        ECHO("Wrote: ");
        for (int i = 0; i < redStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_RED_START, redStr[i]);
            ECHO(redStr[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom LED GREEN:");
        ECHO("Wrote: ");
        for (int i = 0; i < greenStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_GREEN_START, greenStr[i]);
            ECHO(greenStr[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom LED BLUE:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < blueStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_BLUE_START, blueStr[i]);             
            ECHO(blueStr[i]);
        }
        ECHOLN("");
        EEPROM.commit();
        ECHOLN("Done writing!");
        server.begin();
        return;
    }
    ECHOLN("Wrong data!!!");
}

void clearEeprom(){
    ECHOLN("clearing eeprom");
    for (int i = 0; i < EEPROM_WIFI_MAX_CLEAR; ++i){
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    ECHOLN("Done writing!");
}

void detachIP(String ip){
    String first_octetStr = "";
    String second_octetStr = "";
    String third_octetStr = "";
    String fourth_octetStr = "";
    for(int i = 0, j = 0; i <= sizeof(ip)+1; i++){
        char c = ip[i];
        if(c == '.'){
            j++;
            continue;
        }
        switch(j){
            case 0:
                first_octetStr += c;
                break;
            case 1:
                second_octetStr += c;
                break;
            case 2:
                third_octetStr += c;
                break;
            case 3:
                fourth_octetStr += c;
                break;
        }

    }

    first_octet = atoi(first_octetStr.c_str());
    second_octet = atoi(second_octetStr.c_str());
    third_octet = atoi(third_octetStr.c_str());
    fourth_octet = atoi(fourth_octetStr.c_str());
}

void ConfigMode(){
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    ECHOLN(server.arg("plain"));
    JsonObject& rootData = jsonBuffer.parseObject(server.arg("plain"));
    ECHOLN("--------------");
    tickerSetApMode.stop();
    if (rootData.success()) {
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
        //server.stop();
        String nssid = rootData["ssid"];
        String npass = rootData["password"];
        String ip = rootData["set_ip"];

        detachIP(ip); 
        ECHO("Wifi new name: ");
        ECHOLN(nssid);
        ECHO("Wifi new password: ");
        ECHOLN(npass);
        ECHO("Wifi new IP: ");
        ECHO(first_octet);
        ECHO(".");
        ECHO(second_octet);
        ECHO(".");
        ECHO(third_octet);
        ECHO(".");
        ECHOLN(fourth_octet);
        if (connectToWifi(nssid, npass, ip)) {
            esid = nssid;
            epass = npass;
            StartNormalSever();
            Flag_Normal_Mode = true;
            return;
        }

        ECHOLN("Wrong wifi!!!");
        SetupConfigMode();
        StartConfigServer();
        return;
    }
    ECHOLN("Wrong data!!!");
}

void setupIP(){
    // config static IP
    IPAddress ip(first_octet, second_octet, third_octet, fourth_octet); // where xx is the desired IP Address
    IPAddress gateway(first_octet, second_octet, third_octet, 1); // set gateway to match your network
    IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
    WiFi.config(ip, gateway, subnet);
}

bool connectToWifi(String nssid, String npass, String ip) {
    ECHOLN("Open STA....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    delay(100);
    setupIP();
    //WiFi.begin(nssid.c_str(), npass.c_str());

    if (testWifi(nssid, npass)) {
        ECHOLN("clearing eeprom");
        for (int i = 0; i < EEPROM_WIFI_MAX_CLEAR; ++i){ 
            EEPROM.write(i, 0); 
        }
        ECHOLN("writing eeprom ssid:");
        ECHO("Wrote: ");
        for (int i = 0; i < nssid.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_SSID_START, nssid[i]);             
            ECHO(nssid[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom pass:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < npass.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_PASS_START, npass[i]);
            ECHO(npass[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom IP:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < ip.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_IP_START, ip[i]);             
            ECHO(ip[i]);
        }
        ECHOLN("");
        EEPROM.commit();
        ECHOLN("Done writing!");
        return true;
    }

    return false;
}

bool testWifi(String esid, String epass) {
    ECHO("Connecting to: ");
    ECHOLN(esid);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    delay(1000);
    setupIP();      //cai dat ip theo quy dinh
    WiFi.mode(WIFI_STA);        //bat che do station
    WiFi.begin(esid.c_str(), epass.c_str());
    int c = 0;
    ECHOLN("Waiting for Wifi to connect");
    while (c < 20) {
        if (WiFi.status() == WL_CONNECTED) {
            ECHOLN("\rWifi connected!");
            ECHO("Local IP: ");
            ECHOLN(WiFi.localIP());
            return true;
        }
        delay(500);
        ECHO(".");
        c++;
        if(digitalRead(PIN_CONFIG) == LOW){
            break;
        }
    }
    ECHOLN("");
    ECHOLN("Connect timed out");
    return false;
}

void setLedApMode() {
    digitalWrite(LED_TEST, !digitalRead(LED_TEST));
}

// void caculate_Speed(){
//     speed = (Pul_Encoder - pre_Pul_Encoder)/(12*25*0.001);  //vong/s
//     pre_Pul_Encoder = Pul_Encoder;
//     if(abs(speed)<=(SPEED_DEFAUT-ERROR_SPEED) && millis()>=time_start_speed+1000){
//         // tickerSetMotor.detach();
//         // tickerSpeed.detach();
//         tickerSetMotor.stop();
//         tickerSpeed.stop();
//         TurnOn();
//         ECHOLN("CO VAT CAN!!!!!!");
//         return;
//     }
//     ECHO("vantoc:");
//     ECHOLN(speed);
// }


void setPulMotor() {
    //digitalWrite(LED_TEST_MOTOR, !digitalRead(LED_TEST_MOTOR));
    digitalWrite(PIN_PUL_MOTOR, !digitalRead(PIN_PUL_MOTOR));
    // Pul_Motor ++;
    // if(millis() >= test_time + 10000){
    //     //tickerSetMotor.detach();
    //     //tickerSpeed.detach();
    //     tickerSetMotor.stop();
    //     tickerSpeed.stop();
    //     server.begin();
    //     ECHO("so xung phat ra: ");
    //     ECHOLN(Pul_Motor);
    // }
}

String GetFullSSID() {
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    String macID;
    WiFi.mode(WIFI_AP);
    WiFi.softAPmacAddress(mac);
    macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    macID = SSID_PRE_AP_MODE + macID;
    ECHO("[Helper][getIdentify] Identify: ");
    ECHOLN(macID);
    return macID;
}

void checkButtonConfigClick(){
    if (digitalRead(PIN_CONFIG) == LOW && (ConfigTimeout + CONFIG_HOLD_TIME) <= millis()) { // Khi an nut
        ConfigTimeout = millis();
        //tickerSetMotor.attach(0.2, setLedApMode);  //every 0.2s
        Flag_Normal_Mode = false;
        tickerSetApMode.start();
        SetupConfigMode();
        StartConfigServer();
    } else if(digitalRead(PIN_CONFIG) == HIGH) {
        ConfigTimeout = millis();
    }
}


void SetupConfigMode(){
    ECHOLN("[WifiService][setupAP] Open AP....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    delay(1000);
    WiFi.mode(WIFI_AP_STA);
    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);
    String SSID_AP_MODE = GetFullSSID();
    WiFi.softAP(SSID_AP_MODE.c_str(), PASSWORD_AP_MODE);
    ECHOLN(SSID_AP_MODE);

    ECHOLN("[WifiService][setupAP] Softap is running!");
    IPAddress myIP = WiFi.softAPIP();
    ECHO("[WifiService][setupAP] IP address: ");
    ECHOLN(myIP);
}
void StartConfigServer(){    
    ECHOLN("[HttpServerH][startConfigServer] Begin create new server...");
//    server = new ESP8266WebServer(HTTP_PORT);
    server.on("/", HTTP_GET, handleRoot);
    server.on("/cleareeprom", HTTP_GET, clearEeprom);
    server.on("/config", HTTP_POST, ConfigMode);
    server.on("/", HTTP_OPTIONS, handleOk);
    server.on("/cleareeprom", HTTP_OPTIONS, handleOk);
    server.on("/config", HTTP_OPTIONS, handleOk);
    server.begin();
    ECHOLN("[HttpServerH][startConfigServer] HTTP server started");
}



void SetupNomalMode(){
    SetupNetwork();
    if (WiFi.status() == WL_CONNECTED){
        StartNormalSever();
    }    
}


void SetupNetwork() {
    ECHOLN("Reading EEPROM ssid");
    esid = "";
    for (int i = EEPROM_WIFI_SSID_START; i < EEPROM_WIFI_SSID_END; ++i){
        esid += char(EEPROM.read(i));
    }
    ECHO("SSID: ");
    ECHOLN(esid);
    ECHOLN("Reading EEPROM pass");
    epass = "";
    for (int i = EEPROM_WIFI_PASS_START; i < EEPROM_WIFI_PASS_END; ++i){
        epass += char(EEPROM.read(i));
    }
    ECHO("PASS: ");
    ECHOLN(epass);
    ECHOLN("Reading EEPROM IP");
    eip = "";
    for (int i = EEPROM_WIFI_IP_START; i < EEPROM_WIFI_IP_END; ++i){
        eip += char(EEPROM.read(i));
    }
    ECHO("IP: ");
    ECHOLN(eip);
    //lay du lieu thong tin den led
    ECHOLN("Reading EEPROM LED RED");
    String redStr = "";
    for (int i = EEPROM_WIFI_LED_RED_START; i < EEPROM_WIFI_LED_RED_END; ++i){
        redStr += char(EEPROM.read(i));
    }
    ECHO("LED RED: ");
    ECHOLN(redStr);
    ECHOLN("Reading EEPROM LED GREEN");
    String greenStr = "";
    for (int i = EEPROM_WIFI_LED_GREEN_START; i < EEPROM_WIFI_LED_GREEN_END; ++i){
        greenStr += char(EEPROM.read(i));
    }
    ECHO("LED GREEN: ");
    ECHOLN(greenStr);
    ECHOLN("Reading EEPROM LED BLUE");
    String blueStr = "";
    for (int i = EEPROM_WIFI_LED_BLUE_START; i < EEPROM_WIFI_LED_BLUE_END; ++i){
        blueStr += char(EEPROM.read(i));
    }
    ECHO("LED BLUE: ");
    ECHOLN(blueStr);
    red_before = atoi(redStr.c_str());
    green_before = atoi(greenStr.c_str());
    blue_before = atoi(blueStr.c_str());
    analogWrite(PIN_LED_RED, red_before);
    analogWrite(PIN_LED_GREEN, green_before);
    analogWrite(PIN_LED_BLUE, blue_before);    
    detachIP(eip);  //tach ip thanh 4 kieu uint8_t
    testWifi(esid, epass);
}

void StartNormalSever(){
    server.on("/", HTTP_GET, handleRoot);
    server.on("/go_up", HTTP_GET, GoUp);
    server.on("/go_down", HTTP_GET, GoDown);
    server.on("/stop", HTTP_GET, Stop);
    server.on("/action", HTTP_GET, Action);
    server.on("/control_led", HTTP_POST, ControlLed);
    server.on("/", HTTP_OPTIONS, handleOk);
    server.on("/go_up", HTTP_OPTIONS, handleOk);
    server.on("/go_down", HTTP_OPTIONS, handleOk);
    server.on("/stop", HTTP_OPTIONS, handleOk);
    server.on("/action", HTTP_OPTIONS, handleOk);
    server.on("/control_led", HTTP_OPTIONS, handleOk);
    server.begin();
    ECHOLN("HTTP server started");
}

void handleInterruptMotor(){
    if(digitalRead(PIN_DIR_MOTOR) == 0){
        Pul_Encoder++;
    }
    else{
        Pul_Encoder--;
    }
    // ECHOLN(Pul_Encoder);
}

void tickerupdate(){
    tickerSetApMode.update();
    //tickerSpeed.update();
    tickerSetMotor.update();
}

void setup() {
    pinMode(LED_TEST, OUTPUT);
    pinMode(LED_TEST_MOTOR, OUTPUT);
    pinMode(PIN_CONFIG, INPUT);
    pinMode(PIN_PUL_MOTOR, OUTPUT);
    pinMode(PIN_DIR_MOTOR, OUTPUT);
    pinMode(PIN_ENCODER_MOTOR, OUTPUT);
    // pinMode(PIN_ENCODER_MOTOR, INPUT_PULLUP);
    analogWriteRange(255);      //max gia tri PWM la 255
    Serial.begin(115200);
    EEPROM.begin(512);
    SetupNomalMode();     //khi hoat dong binh thuong

    //external interrupt doc tin hieu encoder
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_MOTOR), handleInterruptMotor, CHANGE);

}


void loop() {
    if (Flag_Normal_Mode == true && WiFi.status() != WL_CONNECTED){
        if (testWifi(esid, epass)){
            StartNormalSever();
        } 
    } 
    checkButtonConfigClick();
    tickerupdate();
    server.handleClient();
}
