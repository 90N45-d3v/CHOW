![chow_final_banner](https://user-images.githubusercontent.com/79598596/170996907-980d2391-4eac-488f-ac97-570831158c94.svg)

<p align="center">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white">
  <img src="https://img.shields.io/badge/espressif-E7352C?style=for-the-badge&logo=espressif&logoColor=white">
</p>
<h4 align="center">Transform your little ESP8266 into a light weight and simple to use Communcation-Over-UDP system...</h4>

## Features
- Chat with other ESP8266's over UDP
- Use serial as UI for the communication with other ESP8266's (Want to know how? Type ````help```` in your serial monitor)
- Host your own custom WiFi network for communication with other ESP8266's
- Connect to existing WiFi networks for communication with other ESP8266's
- Use [Port-Forwarding](https://portforward.com/how-to-port-forward/) for communication over WAN (Internet)
- Get the current signal strength ([RSSI](https://en.m.wikipedia.org/wiki/Received_signal_strength_indication)) from the connection to your network

## Installation
You can easily use the pre-compiled binary file of CHOW and just flash it onto your ESP8266, or compile it yourself with the [Arduino IDE](https://www.arduino.cc/en/software).
### Installation with binary file:
1. Download the [latest binary file](https://github.com/90N45-d3v/CHOW/releases) of CHOW
2. Flash it on your ESP8266 with the baud rate of ````115200````. I recommend using [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp8266/)
````
esptool.py --chip esp8266 -p <port ex.: /dev/cu.usbserial-14130> -b 115200 write_flash -z 0x00000 </path/to/CHOW.bin>
````
3. Unplug the microcontroller. Plug it back into your computer.
4. Open the serial monitor and type the command ````help````
### Installation with Arduino IDE:
1. Download the [latest arduino sketch](https://github.com/90N45-d3v/CHOW/releases) of CHOW
2. Download the [SimpleCLI Library](https://github.com/spacehuhn/SimpleCLI/archive/master.zip) from [@spacehuhn](https://github.com/spacehuhn)
3. Unzip the downloaded SimpleCLI Library and rename the resulting folder to ````SimpleCLI````
4. Paste the folder in your arduino library folder (usually located somewhere at ````documents/Arduino/libraries````).
5. Now you can open the downloaded arduino sketch in your Arduino IDE and upload it to your ESP8266

## Working on...
- Encryption for secure UDP-Packets
- Message Broadcast for sending one message to multiple microcontrollers

### Additional information
If you have some improvements, errors or other things to say, just make a fork, pull-request, open an issue or contact me on twitter [@90N45](https://twitter.com/90N45).
<br>
<br>
<i>WANT TO TAKE A LOOK AT AN EASTER CHICKEN?<br>FIND THE ````EGG```` TO SEE WHO CREATED IT...</i>
