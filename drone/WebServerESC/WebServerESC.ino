/*
 * Allows control of an Electronic Speed Controller (ESC) via a web server.
 * The request happens through a GET http method.
 */

#include <avr/pgmspace.h>
#include <Servo.h>
#include <WiServer.h>

byte armed = 0;

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
const prog_char htmlMotor[] PROGMEM = {
"<select name=""OPERATION"" id=""OPERATION"" style=""font-size:50px""><option value=""reset"">Reset Motors</option><option value=""arm"">Arm Motors</option><option value=""throttle25"">Throttle 25</option><option value=""throttle50"">Throttle 50</option><option value=""throttle75"">Throttle 75</option></select><h1><input type=submit value=SUBMIT style=""font-size:50px""></h1></form></div><br>"};

// time of last arming
unsigned long lastArmTime = 0;

// wifi connection state
U8 state = 0;

// These are the motors
Servo throttleMotor;
Servo pitchMotor;
Servo yawMotor;
Servo rollMotor;

/*
Servo angle : PWM range
40 : 0    - 1230
60 : 1231 - 1360
70 : 1361 - 1490
90 : 1491 - 1620
100: 1621 - 1749
120: 1750 +
*/
// speed is 0-100
int servoAngleForSpeed(int speed)
{
    return map(speed, 0, 100, 40, 120);
}

// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) 
{
    // Check if the requested URL matches "/"
    if (strcmp(URL, "/") == 0) {
        _webpageHelper();
        // URL was recognized
        return true;
    }
     
    if (strcmp (URL, "/?OPERATION=reset") == 0) {
        resetMotors();
        _webpageHelper();
        return (true);
    }
 
    if (strcmp (URL, "/?OPERATION=arm") == 0) {
        arm();
        _webpageHelper();
        return (true);
    }
    
    if (strcmp (URL, "/?OPERATION=throttle25") == 0) {
        throttleMotor.write(servoAngleForSpeed(25));
        _webpageHelper();
        return (true);
    }
    
    if (strcmp (URL, "/?OPERATION=throttle50") == 0) {
        throttleMotor.write(servoAngleForSpeed(50));
        _webpageHelper(); 
        return (true);
    }
    
    if (strcmp (URL, "/?OPERATION=throttle75") == 0) {
        throttleMotor.write(servoAngleForSpeed(75));
        _webpageHelper(); 
        return (true);
    }
    
 
  
    Serial.println("url not found");
    // URL not found
    return false;
}

void resetMotors()
{
    throttleMotor.write(servoAngleForSpeed(0));
    yawMotor.write(servoAngleForSpeed(50));
    rollMotor.write(servoAngleForSpeed(50));
    pitchMotor.write(servoAngleForSpeed(50));    
}

void setup() 
{    
     // Required for I/O from Serial monitor
    Serial.begin(57600);
 
    Serial.println("Starting WiFi");
     // Initialize WiServer and have it use the sendMyPage function to serve pages
    WiServer.init(sendMyPage);


    rollMotor.attach(6);
    yawMotor.attach(7);
    throttleMotor.attach(8);
    pitchMotor.attach(9);
    
    WiServer.enableVerboseMode(true);

    Serial.println("setting all channels at resting state");
    resetMotors();
    delay(5000);
    
    arm();    
    
    /*
    Serial.println("setting throttle to 25");
    throttleMotor.write(servoAngleForSpeed(25));
    */
    
 }

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
 
    if (armed) {
        WiServer.print("<div align=""center""><form><font size=20 color=green> Engine Motor - ON</font><br>");
        WiServer.print("<method=GET>");
        WiServer.print_P(htmlMotor);
    }    
    else {
        WiServer.print("<div align=""center""><form><font size=20 color=red> Engine Motor - OFF</font><br>");
        WiServer.print("<method=GET>");
        WiServer.print_P(htmlMotor);
    }   
     
    WiServer.print ("</div></body></html>");
}

void arm() 
{
    unsigned long now = millis();
    
    // don't try rearming if we already tried earlier, this will confusing the APM
    if(now - lastArmTime < 6000) {
         return;
    }
    
    Serial.println("arming");
    throttleMotor.write(servoAngleForSpeed(0));
    yawMotor.write(servoAngleForSpeed(130));
    delay(5000);
    resetMotors();
    armed = 1;
    lastArmTime = now;
}
