#include <stdio.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
"<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
"<meta charset='UTF-8'>"
"<meta http-equiv='X-UA-Compatible' content='IE=edge'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Document</title>"
"<style>"
"html {"
"box-sizing: border-box;"
"font-size: 62.5%; /* 1 rem = 10px */"
"}"
"*, *:before, *:after {"
"box-sizing: inherit;"
"}"
"body {"
"font-size: 1.6rem;"
"line-height: 2;"
"}"
"div{"
"width:100%;"
"text-align: center;"
"}"
"h1{"
"font-size: 6rem;"
"margin: 0;"
"}"
"button{"
"font-size: 2rem;"
"}"
"button {"
"background-color:#4eb5f1;"
"color: #fff;"
"border:none;" 
"border-radius:10px;"
"min-width: 120px;"
"height: 50px;"
"width: 100px;"
"}"
"/*medida de celular*/"
"@media(max-width: 780px){"
"h1{"
"font-size: 10rem;"
"}"
"button{"
"height: 80px;"
"font-size: 4rem;"
"}"
"}"
"</style>"
"</head>"
"<body>"
"<div>"
"<h1>Led</h1>"
"<nav>"
"<a href=\"/H\"><button >ON</button></a>"
"<a href=\"/L\"><button >OFF</button></a>"
"</nav>"
"</div>"      
"</body>"
"</html>";

//-------------------------------------------------------


const char *ssid = "brianArduino";
const char *password = "123456789";
String currentLine = ""; 
bool parpadeo = false;
int cont;
bool estado = false;
WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  //IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  cont = 0;
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    //String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print(pagina);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          parpadeo = false;
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          parpadeo = false;
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
  }
}