#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define SERVER_PORT 9001    //กำหนด Port ใช้งาน
const char* ssid = "*****************";       //กำหนด SSID
const char* password = "**********************"; //กำหนด Password


WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port
WiFiClient client;     // THE SERVER CLIENTS
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED1      D0          // Connectivity With Client #1
  #define     OUT1      D4          // Connectivity With Client #1
//------------------------------------------------------------------------------------


String line,selector;

int address = 0;

int value;

void setup() {
  EEPROM.begin(512);


  // Setting The Mode Of Pins
  pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
  pinMode(OUT1, OUTPUT);
    value = EEPROM.read(address);

    if(value==1){
      digitalWrite(OUT1, LOW); // ให้ LED ติด
    }

    if(value==0){
      digitalWrite(OUT1, HIGH); // ให้ LED ติด
    }
  
  Serial.begin(115200); // เปิดใช้การ Debug ผ่าน Serial
     WiFi.begin(ssid, password); //เชื่อมต่อกับ AP
      IPAddress ip(192,168,1,42);   
      IPAddress gateway(192,168,1,1);   
      IPAddress subnet(255,255,255,0);   
     WiFi.config(ip, gateway, subnet);
     while (WiFi.status() != WL_CONNECTED)  //รอการเชื่อมต่อ
    {
            delay(500);
            Serial.print(".");
    }
            Serial.println("WiFi connected");   //แสดงข้อความเชื่อมต่อสำเร็จ  
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP());     //แสดงหมายเลข IP
            server.begin();             //เริ่มต้นทำงาน TCP Server
}

void loop() {


   if(server.hasClient())
   {
      WiFiClient client = server.available();
      digitalWrite(LED1, HIGH);
      while(client.connected())
     {
        //---------------------------------------------------------------
        // If Clients Are Connected 
        //---------------------------------------------------------------
        value = EEPROM.read(address);

        if(value==1){
          digitalWrite(OUT1, HIGH); // ให้ LED ติด
        }
      
        if(value==0){
          digitalWrite(OUT1, LOW); // ให้ LED ติด
        }
        
        if(client.available())
        {
            //char c = client.read(); // อ่านข้อมูลออกมา 1 ไบต์
            String line = client.readStringUntil('\r');

          
              Serial.println(line); // แสดงตัวแปร line ไปที่ Serial Monitor
              if (line == "LEDON") { // ถ้าสงข้อความเข้ามาว่า LEDON
               // digitalWrite(pin, LOW); // ให้ LED ติด
                EEPROM.write(address, 1);
                client.print("\nLED ON");
              }else if (line == "LEDOFF") { // ถ้าสงข้อความเข้ามาว่า LEDOFF
               // digitalWrite(pin, HIGH); // ให้ LED ดับ
                EEPROM.write(address, 0);
                client.print("LED OFF");
              }else if (line == "REFRESH") { // ถ้าสงข้อความเข้ามาว่า LEDOFF
                client.print(String(value));
              }
      
              EEPROM.commit();

        client.flush();
        }
          


        //---------------------------------------------------------------
        // If Clients Are Disconnected
        //---------------------------------------------------------------
        
        if(!client || !client.connected())
        {
          // Here We Turn Off The LED To Indicated The Its Disconnectted
           digitalWrite(LED1, LOW);
          // Here We Jump Out Of The While Loop
          break;
        }
     }
   }    
   else
   {
      // This LED Blinks If No Clients Where Available
      digitalWrite(LED1, HIGH);
      delay(250);
      digitalWrite(LED1, LOW);
      delay(250);
   }

}
