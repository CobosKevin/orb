# The Orb

The Orb is an Arduino based ambiant device representing the state of Jenkins build jobs. It connects wirelessly to the Internet, parses the build file at /cc.xml and interpret the results.

## Color Codes
===

###GREEN
* SOLID - Most recent build successfull
* PULSING SLOW - Most recent successfull build in progress
* ALTERNATING WITH RED - Schroedinger state. Error parsing build xml file.

###RED 
* SOLID - Most recent build failed
* PULSING SLOW - Most recent failed build in progress
* PULSING FAST - One or more build in failed state
* ALTERNATING WITH GREEN - Schroedinger state. Error parsing build xml file.

###BLUE
* SOLID - In WiFi setup mode (see below)
* PULSING SLOW - Failed connecting to host
* PULSING FAST - No WiFi network


When there is no build activity and all build are in a success state, the Orb will turn off after 2 minutes.


##Change WiFi Settings
===

Open the Orb, toggle switch #1 to SETUP, cycle power.

On a computer, join adhoc network Wifly-GSX-dd. Make sure you have a self assigned IP address in the 169.254.1.x subnet.

Telnet to the Orb and enter command mode with ```$$$```. You can then enter your WiFi settings.

```
> telnet 169.254.1.1 2000
$$$
set wlan ssid YOUR-SSID
set wlan phrase YOUR-PASSWORD
set wlan auth 4 (0=none, 4=WPA2)
set ip dhcp 1
set wlan join 2
save
reboot
```

Toggle switch #1 to WIFI, cycle power.

##Jenkins Configuration
===
Make sure that READ permissions is granted for /cc.xml. This opens a hole in security to allow unauthenticated access to /cc.xml at the root of the server. This URI provides monitoring capability to a range of desktop clients. Enabling this option reveals limited information about your build to the whole world. Use with care.

##Unit Tests
===

```
rake all
```