orb
===

GREEN

SLOW BLINKING - Most recent successfull build in progress
SOLID - Most recent build success


RED 

SLOW BLINKING - Most recent failed build in progress
SOLID - Most recent build failed
FAST BLINKING - One or more build in failed state


BLUE

SOLID - No network
SLOW BLINKING - Failed connecting to host

OFF - No activity and all build in success state


HOW TO CHANGE WIFI SETTINGS
===========================

Open Orb, switch 1+2 to SETUP, cycle power
Join adhoc network Wifly-GSX
> telnet 169.254.1.1 2000
$$$
set wlan ssid YOUR-SSID
set wlan phrase YOUR-PASSWORD
set wlan auth 4 (0=none, 4=WPA2)
set ip dhcp 1
set wlan join 2
save
reboot
Switch 1+2 to WIFI, cycle power

Unit Tests
==========

```
rake all
```