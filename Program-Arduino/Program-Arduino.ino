/*
 * Title: Test receiver and sender SMS
 * 
 * Author: Romain SCHNOEBELEN
 * Date: 21/04/2018
 * 
 * Description: When SMS coming, the sender number and the message payload are print on moniteur.
 *              After that, an reply SMS are printing on moniteur and sending to the last sender number.
 * 
 * Any help: Make sure the serial monitor is set to send a newline when you press return.
 *            Arduino GSM shield uses digital pin 2 and 3 for the serial communication.
 *            The pin 7, is used for PWRKEY function.
 *            If you use an Arduino Yun please bridge pin 8 and 2. For more explain : https://www.arduino.cc/en/Guide/GSMShieldLeonardoMega
*/

// Include library
#include <GSM.h>

// Used pin in sketch
#define relais1 8
#define relais2 9

// PIN Number for the SIM
#define PINNUMBER "1918"

// Initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Publics variables
char msgFromNumber[20];

/* ----- ----- ----- ----- FUNCTION SETUP() ----- ----- ----- ----- */
void setup() {
  // Initialize serial communications
  Serial.begin(9600);

  //  Wait for port to open
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("*****************************************************");
  Serial.println("*************Test receives and sends SMS*************");
  Serial.println("*****************************************************");

  // Setting pin used in sketch
  pinMode(relais1, OUTPUT);
  pinMode(relais2, OUTPUT);
  digitalWrite(relais1, HIGH);
  digitalWrite(relais2, LOW);
  Serial.println("STATUS SETUP: Digital pin setting");
  
  // Connection status
  boolean notConnected = true;

  // Start GSM Shield
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } 
    else {
      Serial.println("STATUS SETUP: GSM not connected...retry in 1s");
      delay(1000);
      Serial.println("STATUS SETUP: Retry to connect...");
    }
  }
  
  Serial.println("STATUS SETUP: GSM connected");
  
  Serial.println("STATUS SETUP: Waiting for message");
  Serial.println("*****************************************************");
}
/* ----- ----- ----- ----- FUNCTION LOOP() ----- ----- ----- ----- */
void loop() {  
  // If there are any SMSs available
  if (sms.available()) {
    // Function start
    receiveSMS();
  }
}

/* ----- ----- ----- ----- FUNCTION SENDSMS() ----- ----- ----- ----- */
void sendSMS(char number[20], char payloadSend[50]){
  Serial.println("*****************************************************");
  Serial.print("STATUS SEND MESSAGE: Message to ");
  Serial.println(number);

  Serial.print("TEXT MESSAGE: ");
  Serial.println(payloadSend);

  Serial.println("STATUS SEND MESSAGE: Preparing");
  sms.beginSMS(number);
  Serial.println("STATUS SEND MESSAGE: Writing");
  sms.print(payloadSend);
  sms.endSMS();
  Serial.println("STATUS SEND MESSAGE: Complete");
  Serial.println("*****************************************************");
}

/* ----- ----- ----- ----- FUNCTION RECEIVESMS() ----- ----- ----- ----- */
void receiveSMS(){
  Serial.println("*****************************************************");
  char payloadReceive;
  Serial.print("STATUS RECEIVE MESSAGE: Message from ");

  // Get sender number
  sms.remoteNumber(msgFromNumber, 20);
  Serial.println(msgFromNumber);

  // An example of message delete
  // Any messages starting with # should be discarded
  if (sms.peek() == '#') {
    Serial.println("STATUS RECEIVE MESSAGE: Message discarded");
    sms.flush(); // Delete message from modem memory
  }

  Serial.print("TEXT MESSAGE: ");
  // Read message bytes and print them
  while (payloadReceive = sms.read()) {
    Serial.print(payloadReceive);
  }
  
  Serial.println();
  Serial.println("STATUS RECEIVE MESSAGE: End of message");

  sms.flush(); // Delete message from modem memory
  Serial.println("STATUS RECEIVE MESSAGE: Message deleted");
  Serial.println("*****************************************************");
  
  // Function start
  sendSMS(msgFromNumber, "HELLO WORLD !");
}
