/*
 * A simple sketch that uses WiServer to serve a web page
 */

#include <WiShield.h>
#include <WiServer.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2
// #define DEBUG 1

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,16,110};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,16,254};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"MP01"};		// max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"abc"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
				};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------


// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) {
  
    // Check if the requested URL matches "/"
    if (strcmp(URL, "/") == 0) {
        // Use WiServer's print and println functions to write out the page content
        WiServer.print("<html>");
        WiServer.print("Hello World!");
        WiServer.print("</html>");
        
        // URL was recognized
        return true;
    }
    // URL not found
    return false;
}


void setup() {
  WiServer.enableVerboseMode(true);

  // Enable Serial output and ask WiServer to generate log messages (optional)
  Serial.begin(57600);

  // Initialize WiServer and have it use the sendMyPage function to serve pages
  WiServer.init(sendMyPage);  
}

U8 state = 0;

void loop(){
  // Run WiServer

  U8 newState = WiServer.getConnectionState();
  
  if(newState != state) {
    state = newState;
  }

  if(!state) {
    WiServer.init(sendMyPage);
  }
  
  WiServer.server_task();
 
  delay(10);
}

