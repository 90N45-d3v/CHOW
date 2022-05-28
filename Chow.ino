#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SimpleCLI.h>
#include <WiFiUdp.h>
#define LED 2
#define UDP_PORT 55320

SimpleCLI cli;
WiFiUDP UDP;

char packet[255];

void scanResult(int networksFound)
{
  Serial.printf("%d network(s) found:\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
}

void help_callback(cmd* commandPointer) {
  Command cmd(commandPointer);

  Serial.print("\nChat with other ESP8266's over UDP in nearby WiFi networks\n");
  Serial.print("\nCOMMANDS:\n");
  Serial.print("help        - Show this helpful information\n");
  Serial.print("scan        - Scan for WLAN to find other networks\n");
  Serial.print("connect     - Connect your ESP8266 to a network. Required for communication (ex.: connect -ssid <SSID> -pass <PASSWORD>)\n");
  Serial.print("host        - Host your own CHOW-Network. Password length has to be at least 8 characters (ex.: host -ssid <SSID> -pass <PASSWORD>)\n");
  Serial.print("send        - Send text to someone, who is in the same network as you (ex.: send -text <Your text> -to <IP Address of receiver>)\n");
  Serial.print("signal      - Get the current recieved signal strength (RSSI) (ex.: signal)\n");
  Serial.print("banner      - Shows you the CHOW banner (ex.: banner)\n");
  Serial.print("\n");
  
}

void scan_callback(cmd* commandPointer) {
  Command cmdScan(commandPointer);

  Serial.print("\n[*] Scanning for WiFi networks\n");
  WiFi.scanNetworksAsync(scanResult);

}

void connect_callback(cmd* commandPointer) {
  Command cmdConnect(commandPointer);

  Argument connect_ssid = cmdConnect.getArg("ssid");
  Argument connect_passwd = cmdConnect.getArg("pass");
  String ssid = connect_ssid.getValue();
  String password = connect_passwd.getValue();

  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.print("\n\n[*] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, LOW);
    delay(300);
    Serial.print(".");
    digitalWrite(LED, HIGH);
    delay(200);
  }

  Serial.print("\n[*] Successfully connected to ");
  Serial.println(ssid);
  Serial.print("\n[*] Your local IP: ");
  Serial.println(WiFi.localIP());

}

void host_callback(cmd* commandPointer) {
  Command cmdHost(commandPointer);

  Argument host_ssid = cmdHost.getArg("ssid");
  Argument host_passwd = cmdHost.getArg("pass");
  String ssid = host_ssid.getValue();
  String password = host_passwd.getValue();
  int ap_channel = 3;

  if (password.length() > 7) {
    
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    delay(100);

    Serial.print("\n[*] Starting AP as ");
    Serial.print(ssid);
    Serial.print(", with ");
    Serial.print(password);
    Serial.print(" as password");

    WiFi.softAP(ssid, password, ap_channel);

    Serial.print("\n[*] AP successfully started");
    Serial.print("\n[*] Your local IP: 192.168.4.1\n");

  } else {
    Serial.print("\n[!] The password must be at least 8 characters long");
  }

}

void send_callback(cmd* commandPointer) {
  Command cmdSend(commandPointer);

  Argument send_text = cmdSend.getArg("text");
  Argument send_receiver = cmdSend.getArg("to");
  String text_str = send_text.getValue();
  String receiver_str = send_receiver.getValue();
  const char* text = text_str.c_str();
  const char* receiver = receiver_str.c_str();
  const uint16_t re_port = 55320;
  

  Serial.print("\n[ESP --> ");
  Serial.print(receiver);
  Serial.print("] ");
  Serial.println(text);

  UDP.beginPacket(receiver, re_port);
  UDP.write(text);
  UDP.endPacket();

}

void signal_callback(cmd* commandPointer) {
  Command cmdSignal(commandPointer);

  long rssi = WiFi.RSSI();
  Serial.print("\n[*] Current RSSI: ");
  Serial.print(rssi);
  Serial.print("dBm");
  if (rssi == 31) {
    Serial.print(" (No WiFi connection)");
  }

}

void banner_callback(cmd* commandPointer) {
  Command cmd(commandPointer);

  Serial.print("\n  __  _ _   _   _ _ _ \n / _|| U | / \\ | | | |\n( (_ |   |( o )| V V |\n \\__||_n_| \\_/  \\_n_/ ");
  Serial.print("\n by github.com/90N45-d3v\n");

}

void CLI_callback(cmd* commandPointer) {
  Command cmd(commandPointer);

  Serial.print("\n  *     *\n   \\   /\n  _|___|_\n/     _  \\\n|  _/ }  |\n| `>' \\  |\n \\__\\__`_/\n    |  '-.    .-.\n    '   ';`--' .'\n     '.   `'-./\n      '.`\"-..-;`\n        `;-..'\n        _| _|\n         /` /`\nThanks to Spacehuhn for the SimpleCLI!\n--> github.com/SpacehuhnTech/SimpleCLI\n");
}

void errorCallback(cmd_error* e) {
  
  CommandError cmdError(e);

  Serial.print("\n[!] ERROR: ");
  Serial.println(cmdError.toString());

  if (cmdError.hasCommand()) {
    Serial.print("[?] Did you mean \"");
    Serial.print(cmdError.getCommand().toString());
    Serial.println("\"?");
  }

}

void setup() {
  Serial.begin(115200);

  UDP.begin(UDP_PORT);

  pinMode(LED, OUTPUT);

  delay(3000);

  Serial.print("\n  __  _ _   _   _ _ _ \n / _|| U | / \\ | | | |\n( (_ |   |( o )| V V |\n \\__||_n_| \\_/  \\_n_/ ");
  Serial.print("\n by github.com/90N45-d3v\n");

  digitalWrite(LED, HIGH);

  Command cmdHelp = cli.addCmd("help", help_callback);
  Command cmdScan = cli.addCmd("scan", scan_callback);
  Command cmdConnect = cli.addCmd("connect", connect_callback);
  Argument connect_ssid = cmdConnect.addArg("ssid");
  Argument connect_passwd = cmdConnect.addArg("pass");
  Command cmdHost = cli.addCmd("host", host_callback);
  Argument host_ssid = cmdHost.addArg("ssid", "CHOW-Network");
  Argument host_passwd = cmdHost.addArg("pass", "90N45-d3v");
  Command cmdSend = cli.addCmd("send", send_callback);
  Argument send_text = cmdSend.addArg("text");
  Argument send_receiver = cmdSend.addArg("to");
  Command cmdSignal = cli.addCmd("signal", signal_callback);
  Command cmdCLI = cli.addCmd("egg", CLI_callback);
  Command cmdBanner = cli.addCmd("banner", banner_callback);

}
 
void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    cli.parse(input);
  }

  if (cli.errored()) {
    CommandError cmdError = cli.getError();
    Serial.print("\n[!] ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
      Serial.print("[?] Did you mean \"");
      Serial.print(cmdError.getCommand().toString());
      Serial.println("\"?");
    }
  }

  int packetSize = UDP.parsePacket();
  if (packetSize) { 
    int len = UDP.read(packet, 255);

    if (len > 0) {

      packet[len] = '\0';

    }

    digitalWrite(LED, LOW);

    Serial.print("\n[");
    Serial.print(UDP.remoteIP());
    Serial.print(" --> ESP] ");
    Serial.println(packet);

    delay(500);

    digitalWrite(LED, HIGH);
 
  }
}
