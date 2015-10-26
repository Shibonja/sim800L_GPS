//#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"
#include <TinyGPS.h>
//#include "sms.h"
//#include "call.h"

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to start a connection as client.

InetGSM inet;
//CallGSM call;
//SMSGSM sms;

//char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

char msgBuffer[150];
char msg[50];

void setup()
{

  memset(msgBuffer , 0 , 150);
  sprintf(msgBuffer , "[{ \"value\": {\"lng\": 30.3396,\"lat\": 22.3904},\"at\": \"2015-10-26T15:19:50.642+0000\"}]");
     //Serial connection.
     Serial.println(msgBuffer);
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

//     if(started) {
//          //GPRS attach, put in order APN, username and password.
//          //If no needed auth let them blank.
//          if (inet.attachGPRS("active.bhmobile.ba", "", ""))
//               Serial.println("status=ATTACHED");
//          else Serial.println("status=ERROR");
//          delay(1000);
//
//          //Read IP address.
//          gsm.SimpleWriteln("AT+CIFSR");
//          delay(5000);
//          //Read until serial buffer is empty.
//          gsm.WhileSimpleRead();
//
//          //TCP Client GET, send a GET request to the server and
//          //msg = "[{ "value": {"lng": 18.3396522721951355,"lat": 42.39042726625922386},"at": "2015-10-25T18:34:50.642+0000"}]"
//         // c2lib25qaWMuaGFyaXNAZ21haWwuY29tOkZVTExIRDEwODA=
//          //save the reply.
//          numdata=inet.httpPOST("api.gadgetkeeper.com", 80, "/v1/things/c30ffa13798a11e588351d22ae2d0d02/events/445ae266798b11e588351d22ae2d0d02/datapoints.json", msgBuffer,msg, 100);
//          //Print the results.
//          Serial.println("\nNumber of data received:");
//          Serial.println(numdata);
//          Serial.println("\nData received:");
//          Serial.println(msgBuffer);
//     }
};

void loop()
{
  //sendPost();
     //Read for new byte on serial hardware,
     //and write them on NewSoftSerial.
     serialhwread();
     //Read for new byte on NewSoftSerial.
     serialswread();
};

void sendPost(){
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

          //TCP Client GET, send a GET request to the server and
          //msg = "[{ "value": {"lng": 18.3396522721951355,"lat": 42.39042726625922386},"at": "2015-10-25T18:34:50.642+0000"}]"
         // c2lib25qaWMuaGFyaXNAZ21haWwuY29tOkZVTExIRDEwODA=
          //save the reply.
          numdata=inet.httpPOST("api.gadgetkeeper.com", 80, "/v1/things/c30ffa13798a11e588351d22ae2d0d02/events/445ae266798b11e588351d22ae2d0d02/datapoints.json", msgBuffer,msg, 100);
          //Print the results.
          Serial.println("\nNumber of data received:");
          Serial.println(numdata);
          Serial.println("\nData received:");
          Serial.println(msgBuffer);
     }
}

void serialhwread()
{
     i=0;
     if (Serial.available() > 0) {
          while (Serial.available() > 0) {
               inSerial[i]=(Serial.read());
               delay(10);
               i++;
          }

          inSerial[i]='\0';
          if(!strcmp(inSerial,"/END")) {
               Serial.println("_");
               inSerial[0]=0x1a;
               inSerial[1]='\0';
               gsm.SimpleWriteln(inSerial);
          }
          //Send a saved AT command using serial port.
          if(!strcmp(inSerial,"TEST")) {
               Serial.println("SIGNAL QUALITY");
               gsm.SimpleWriteln("AT+CSQ");
          }
          //Read last message saved.
          if(!strcmp(inSerial,"MSG")) {
               Serial.println(msgBuffer);
          } else {
               Serial.println(inSerial);
               gsm.SimpleWriteln(inSerial);
          }
          inSerial[0]='\0';
     }
}

void serialswread()
{
     gsm.SimpleRead();
}
