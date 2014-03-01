/*
 * Allows control of an Electronic Speed Controller (ESC) via a web server.
 * The request happens through a GET http method.
 */

#include <avr/pgmspace.h>
#include <Servo.h>
#include <WiServer.h>

byte thestart = 0;

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,16,110};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,16,254};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"MP01"};		// max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"1872822821"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = { 0x83, 0x7A, 0xAF, 0x74, 0xEC,	// Key 0
                                  0x5F, 0xE5, 0x65, 0x9F, 0xF2,	// Key 1
                                  0x69, 0x61, 0x53, 0x1C, 0x51, // Key 2
                                  0xB5, 0x6F, 0xBE, 0xCB, 0x6B,	// Key 3
                                };

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------

// Webpage in flash memory
const prog_char htmlHead[] PROGMEM = {"<html><head><meta http-equiv=""content-type"" content=""text/html; charset=utf-8"" /><title>OneButtonAlert Device</title></head>"};
const prog_char htmlMotor[] PROGMEM = {"<select name=""OPERATION"" id=""OPERATION"" style=""font-size:50px""><option value=""OFF"">Turn ON</option><option value=""ON"">Turn OFF</option><option value=""90"">90</option></select><h1><input type=submit value=SUBMIT style=""font-size:50px""></h1></form></div><br>"};

// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) 
{
    // Check if the requested URL matches "/"
    if (strcmp(URL, "/") == 0) {
        _webpageHelper();
        // URL was recognized
        return true;
    }
     
    if (strcmp (URL, "/?OPERATION=OFF") == 0) {
        thestart = 1;
        _webpageHelper();
        move_to_start();       
        return (true);
    }    


    if (strcmp (URL, "/?OPERATION=ON") == 0) {
        thestart = 0;
        _webpageHelper();
        move_to_end();
        return (true);
    }    
    
 
    if (strcmp (URL, "/?OPERATION=90") == 0) {
        thestart = 0;
        _webpageHelper(); 
        move_to_ninty();
        return (true);
    }    

    // URL not found
    return false;
}

int i = 0;     // Loop counter 
    
// This is our motor.
Servo myMotor;

void arm(){
    // arm the speed controller, modify as necessary for your ESC  
    setSpeed(0); 
    delay(3000); //delay 1 second,  some speed controllers may need longer
}

void setSpeed(int speed){
      // speed is from 0 to 100 where 0 is off and 100 is maximum speed
      //the following maps speed values of 0-100 to angles from 0-180,
     // some speed controllers may need different values, see the ESC instructions
     int angle = map(speed, 0, 100, 0, 180);
     myMotor.write(angle);    
}

void setup() {
    WiServer.enableVerboseMode(true);

    // Put the motor to Arduino pin #9
    myMotor.attach(8);
    arm(); 
    myMotor.write(70);
    // Required for I/O from Serial monitor
    Serial.begin(57600);
     Serial.println("Starting WiFi");

     // Initialize WiServer and have it use the sendMyPage function to serve pages
    WiServer.init(sendMyPage);
}

U8 state = 0;

void loop()
{
    U8 newState = WiServer.getConnectionState();

    if(newState != state) {
        state = newState;
    }

    if(!state) {
        WiServer.init(sendMyPage);
    }

    // Run WiServer
    WiServer.server_task();
    delay(10);
}

void _webpageHelper() 
{
    WiServer.print_P(htmlHead);
  
    WiServer.print ("<body><p align=""center"">");
    WiServer.print ("<b><font size=20><center>Device Local IP Address Status</center></b><br>");
    WiServer.print ("<center>");
    WiServer.print (local_ip[0], DEC);
    WiServer.print (".");
    WiServer.print (local_ip[1]), DEC;
    WiServer.print (".");
    WiServer.print (local_ip[2], DEC);
    WiServer.print (".");
    WiServer.print (local_ip[3], DEC);
    WiServer.print ("</center></font><br>");
 
    if (thestart == 0) {
        WiServer.print("<div align=""center""><form><font size=20 color=red> Engine Motor - OFF</font><br>");
        WiServer.print("<method=GET>");
        WiServer.print_P(htmlMotor);
    }    
    else {
        WiServer.print("<div align=""center""><form><font size=20 color=green> Engine Motor - ON</font><br>");
        WiServer.print("<method=GET>");
        WiServer.print_P(htmlMotor);
    }   
     
    WiServer.print ("</div></body></html>");
}


void move_to_start() 
{
    myMotor.write(80);
    //delay(500); // delay 1000ms, this allows the previous instruction to be finished. 
}
     

void move_to_end() 
{
    myMotor.write(70);
}

void move_to_ninty()
{
    myMotor.write(90);
}

