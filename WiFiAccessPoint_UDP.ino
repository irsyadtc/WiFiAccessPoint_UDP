/*
    CAREM UTP 06 June 2020
    
    - Create a WiFI access point
    - Listen to incoming message
    - Reply in return
    
    Author: Muhammad Irsyad
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef APSSID
#define APSSID "ESPap"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;

unsigned int localPort = 4445;      // local port to listen on
unsigned int remotePort = 4445;   // remote port for sending

WiFiUDP server;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "received\r\n";       // a string to send back


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);


  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin(localPort);
  Serial.println("Setup ends");

}

void loop() {

  //receive packet
  int packetSize = server.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n",
                  packetSize,
                  server.remoteIP().toString().c_str(), server.remotePort());
 
    // read the packet into packetBufffer
    int n = server.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address that sent us the packet we received
    server.beginPacket(server.remoteIP(), remotePort);
    server.write(ReplyBuffer);
    server.endPacket();
  }
  delay(1);
}
