/*
 * Allows control of an Electronic Speed Controller (ESC) via a web server.
 * The request happens through a GET http method.
 */

#include <Servo.h>
#include <WiServer.h>


// Definizione Variabili Globali Stato Led
byte thestart = 0;  // Stato led rosso



#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,60};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,254};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"2WIRE689"};		// max 32 bytes

unsigned char security_type = 2;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

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


// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) {
  
    // Check if the requested URL matches "/"
    if (strcmp(URL, "/") == 0) {
        Invia_Pagina_Web();
        // URL was recognized
        return true;
    }
    
    
    // Se URL richieso corrisponde a "?OPERATION=ACCENDI_ROSSO
	  if (strcmp (URL, "/?OPERATION=OFF") == 0) {
           thestart = 1;
           Invia_Pagina_Web();
	   move_to_start();       
	   

	    return (true);
	  }    
	 
	  // Se URL richieso corrisponde a "?OPERATION=SPEGNI_ROSSO"
	  if (strcmp (URL, "/?OPERATION=ON") == 0) {
	    thestart = 0;
            Invia_Pagina_Web();
	    move_to_end();
	    return (true);
	  }    
    
 
	  if (strcmp (URL, "/?OPERATION=90") == 0) {
	    thestart = 0;
	    Invia_Pagina_Web(); 
            move_to_ninty();
	    return (true);
	  }    
    
    
    
    
    // URL not found
    return false;
}



int PIN_BLUEII = 8;
int PIN_RED = 7;

int greenVal = 1;   // Initial values are Red full, Green and Blue off
int i = 0;     // Loop counter 
    
int ledPin = 13; // pin for the LED
int buttonPin = 6; // input pin of pushbutton
int val = 0; // variable for reading the pin status





// IP Address for www.weather.gov  
uint8 ip[] = {184,168,17,1};

// A request that gets the latest METAR weather data for LAX
//GETrequest getWeather(ip, 80, "www.onebuttonalert.com", "/makevoicecall/?accesscode=iHHusuLm2uNe&customersid=14");

//GETrequest getWeathersms(ip, 80, "www.onebuttonalert.com", "/makesmsnow/?accesscode=iHHusuLm2uNe&customersid=14");
    



    
// This is our motor.
Servo myMotor;

// This is the final output
// written to the motor.
String incomingString;

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
 
  // Put the motor to Arduino pin #9
  myMotor.attach(8);
  arm(); 
  myMotor.write(70);
   
  // Required for I/O from Serial monitor
  Serial.begin(57600);
  Serial.println("Starting WiFi");
  // Print a startup message
 // Serial.println("initializing");
   
  // pinMode(PIN_BLUEII, OUTPUT);
//   pinMode(PIN_RED , OUTPUT);
 //  pinMode(ledPin, OUTPUT); // declare LED as output
  // pinMode(buttonPin, INPUT); // declare pushbutton as input


  // Initialize WiServer and have it use the sendMyPage function to serve pages
  WiServer.init(sendMyPage);
  
  // Enable Serial output and ask WiServer to generate log messages (optional)
 // Serial.begin(9600);  // ...set up the serial ouput on 0004 style
  //Serial.begin(57600);
  WiServer.enableVerboseMode(false);
}





void loop(){
  
  //val = digitalRead(buttonPin); // read input value
  
//if (val == HIGH) { // check if the input is HIGH (button released)
//digitalWrite(PIN_BLUEII, LOW); // turn LED OFF
   
//} else {
//digitalWrite(PIN_BLUEII, HIGH ); // turn LED ON

 // a request to server page to make a sms
  // getWeathersms.submit();
   
  // getWeather.submit(); 
 //  delay(50);
 
    // A request to server page make a call
 //  getWeather.submit(); 
   

//}

  // Run WiServer
  WiServer.server_task();
  delay(10);




}




	void Invia_Pagina_Web() {
	  // Usando le funzioni WiServer.print trasmette al pagina Web da visualizzare
	  WiServer.print ("<html>");
	  WiServer.print ("<head>");
WiServer.print ("<meta http-equiv=""content-type"" content=""text/html; charset=utf-8"" />");
//WiServer.print ("<meta http-equiv=""refresh"" content=""25;url=http://192.168.1.60"" />");
WiServer.print ("<title>OneButtonAlert Device</title>");
//WiServer.print ("<link rel=""stylesheet"" type=""text/css"" media=""One Button Alert"" href=""http://onebuttonalert.com/buttonpressed/demo.css"" />");

	 

	  WiServer.print ("</head>");
	  WiServer.print ("<body><p align=""center"">");



	 // WiServer.print ("<div align=""center""><img src=""http://www.onebuttonalert.com/logo.png"" border=""0""></div>");
	  WiServer.print ("<br>");
	  WiServer.print ("<br>");
	  WiServer.print ("<b><font size=20><center>Device Local IP Address Status</center></b><br>");
	  WiServer.print ("<center>");

          WiServer.print (local_ip[0], DEC);
	  WiServer.print (".");
	  WiServer.print (local_ip[1], DEC);
	  WiServer.print (".");
	  WiServer.print (local_ip[2], DEC);
	  WiServer.print (".");
	  WiServer.print (local_ip[3], DEC);

          WiServer.print ("</center></font><br>");
     
	  if (thestart == 0) {
	   WiServer.print ("<div align=""center""><form><font size=20 color=red>Engine Motor - ON</font><br>");
	   WiServer.print ("<method=GET>");
           WiServer.print ("<select name=""OPERATION"" id=""OPERATION"" style=""font-size:50px""><option value=""OFF"">Turn ON</option><option value=""ON"">Turn OFF</option><option value=""90"">90</option></select>");
	   WiServer.print ("<h1><input type=submit value=SUBMIT style=""font-size:50px""></h1></form></div><br>");
	 }    
	  else {
	   WiServer.print ("<div align=""center""><form><font size=20 color=green> Engine Motor - OFF</font><br>");
	   WiServer.print ("<method=GET>");
           WiServer.print ("<select name=""OPERATION"" id=""OPERATION"" style=""font-size:50px""><option value=""OFF"">Turn ON</option><option value=""ON"">Turn OFF</option><option value=""90"">90</option></select>");
          // WiServer.print ("<select name=""OPERATION"" id=""OPERATION"" style=""font-size:50px""><option value=""OFF"">Turn OFF</option><option value=""90"">POWER UP</option></select>");
	   WiServer.print ("<input type=submit value=SUBMIT ></form></div><br>");
	 }   
	 

	  WiServer.print ("</div></body></html>");
	}
	 


	void move_to_start() {
 
         myMotor.write(80);
         


//delay(500); // delay 1000ms, this allows the previous instruction to be finished. 

	}
	 
	// Spegne il led rosso
	void move_to_end() {

         myMotor.write(70);
         
       
	}



	void move_to_ninty() {

         myMotor.write(90);
         

	}
