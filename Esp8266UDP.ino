#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char* ssid = "SSID";
const char* pass = "PASS";
unsigned int localPort = 5000;
int status = WL_IDLE_STATUS;
char packetBuffer[255]; 
char ReplyBuffer[] = "OK\n";
int pwm = 0;

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.print(WiFi.localIP());
  
  Udp.begin(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    IPAddress remoteIp = Udp.remoteIP();

    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    if (strcmp(packetBuffer, "pwm\n") == 0) {
      char p[5];
      String str = String(pwm);
      str.toCharArray(p,5);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(p);
      Udp.endPacket();
    } else {
      pwm = atoi(packetBuffer);
      analogWrite(2, pwm);

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      Udp.endPacket();
    }
  }
  delay(10);
}
