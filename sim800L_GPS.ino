#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "inetGSM.h"
#include "gps.h"

SoftwareSerial ss(11, 12);
TinyGPS gps;

InetGSM inet;
GPSGSM gpsSIM;

int numdata;
char msg[50];

void setup(){
  
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("starting SIM800L");
}

float flat, flon;
unsigned long fix_age;

void loop(){
  //delay(1999);
  //getTime();
  //sendPost();
     //Read for new byte on serial hardware,
     //and write them on NewSoftSerial.
     //serialhwread();
     //Read for new byte on NewSoftSerial.
     //serialswread();

   smartdelay(1000);
   gps.f_get_position(&flat, &flon, &fix_age);
   Serial.println(flon);
   Serial.println(flat);

  
   if (flon < 1000.00)
     sendPost();
   
};

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

//void getTime(){
//
//  
//}

void sendPost(){
  char str_flon[12];
  dtostrf(flon, 9, 7, str_flon);
  
  char str_flat[11];
  dtostrf(flat, 9, 7, str_flat);

  Serial.println(str_flat);
  Serial.println(str_flon);
  
  boolean started=false;
  if (gsm.begin(2400)) {
    Serial.println("\nstatus=READY");
    started=true;
  } else Serial.println("\nstatus=IDLE");
  char msgBuffer[100];
  memset(msgBuffer , 0 , 100);
  sprintf(msgBuffer , "[{ \"value\": {\"lng\": %s,\"lat\": %s},\"at\": \"2015-10-27T12:19:50.642+0000\"}]",str_flon,str_flat);
  Serial.println(msgBuffer);
  if(started) {
    
    //GPRS attach, put in order APN, username and password.
    //If no needed auth let them blank.
    if (inet.attachGPRS("active.bhmobile.ba", "", ""))
      Serial.println("status=ATTACHED");
    else Serial.println("status=ERROR");
    delay(1000);

    //Read IP address.
    gsm.SimpleWriteln("AT+CIFSR");
    delay(5000);
    //Read until serial buffer is empty.
    gsm.WhileSimpleRead();

    numdata=inet.httpPOST("api.gadgetkeeper.com", 80, "/v1/things/c30ffa13798a11e588351d22ae2d0d02/events/445ae266798b11e588351d22ae2d0d02/datapoints.json", msgBuffer,msgBuffer, 100);
    //Print the results.
    Serial.println("\nNumber of data received:");
    Serial.println(numdata);
    Serial.println("\nData received:");
    Serial.println(msgBuffer);
  }
}

