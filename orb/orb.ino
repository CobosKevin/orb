/*

	(c) Copyright 2012 Ternary Labs, All Rights Reserved.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to
	deal in the Software without restriction, including without limitation the
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
	IN THE SOFTWARE.

 */

#include <Arduino.h>
#include <Streaming.h>
#include <PString.h>
#include <SoftwareSerial.h>
#include <TimedAction.h>
    
#include "WiFlySerial.h"
#include "FreeMemory.h"
#include "Credentials.h"
#include "LEDDisplay.h"
#include "Jenkins.h"

WiFlySerial wifi(ARDUINO_RX_PIN, ARDUINO_TX_PIN); 

// LED Pins
#define RED_LED_PIN    9
#define GREEN_LED_PIN 10
#define BLUE_LED_PIN  11

#define REQUEST_BUFFER_SIZE 180
#define BODY_BUFFER_SIZE 100

#define JENKINS_HOST "ci.test.com"

Jenkins jenkins = Jenkins(JENKINS_HOST);

// Check the state of the build periodically
TimedAction getBuildStateAction = TimedAction(30000, sendRequest);

// Update display periodically
TimedAction refreshDisplayAction = TimedAction(5, refreshDisplay);

long lastBuild = 0;

void setup()  { 
	Serial.begin(9600);

    Serial << F("setup(): enter") << endl;  
    Serial << F("RAM: ") << freeMemory() << endl;

    Display.setRGBPins(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);
    Display.test();

	testWiFly();

    sendRequest();

    Serial << F("setup(): exit") << endl;  
} 

void loop()  { 	
    refreshDisplayAction.check();
    if (wifi.isConnectionOpen() && wifi.available() > 0)
    {
    	while (wifi.available() > 0)
    	{
            refreshDisplayAction.check();
    		jenkins.parse((char) wifi.read());
            delay(1);
    	}
        wifi.closeConnection();
        refreshDisplayAction.check();
        displayBuildSate();
    }
    refreshDisplayAction.check();
	getBuildStateAction.check();
}

void refreshDisplay()
{
    Display.refresh();
}


bool testWiFly() {
	char bufRequest[REQUEST_BUFFER_SIZE];
	char bufBody[BODY_BUFFER_SIZE];

    Serial << F("testWiFly(): enter") << endl;  
	Serial << F("RAM: ") << freeMemory() << endl;  

	//wifi.setDebugChannel((Print*) &Serial);

	if (wifi.begin()) {
		Serial << F("WiFly version: ") <<  wifi.getLibraryVersion(bufRequest, REQUEST_BUFFER_SIZE) << endl;
		Serial << F("MAC: ") << wifi.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;    
		
		wifi.setAuthMode(WIFLY_AUTH_WPA2_PSK);
		wifi.setJoinMode(WIFLY_JOIN_AUTO);
		wifi.setDHCPMode(WIFLY_DHCP_ON);
		 
		wifi.getDeviceStatus();

		if (!wifi.isifUp()) 
        {
			Serial << F("Leaving: ") <<  ssid << wifi.leave() << endl;

			if (wifi.setSSID(ssid)) 
            {    
				Serial << F("SSID Set: ") << ssid << endl;
			}

			if (wifi.setPassphrase(passphrase)) 
            {
				Serial << F("Passphrase Set") << endl;
			}

			Serial << F("Joining: ") << ssid << endl;

			if (wifi.join()) 
            {
				Serial << F("Joined: ") << ssid << endl;
				wifi.setNTP(ntp_server);
			} 
            else 
            {
				Serial << F("Joining: ") << ssid << F(" failed") << endl;
			}
		}

		Serial << F("IP: ") << wifi.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl 
                << F("Netmask: ") << wifi.getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl 
                << F("Gateway: ") << wifi.getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl 
                << F("DNS: ") << wifi.getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl;

		wifi.SendCommand("set comm remote 0", ">", bufBody, BODY_BUFFER_SIZE);
		wifi.closeConnection();


		wifi.flush();
		while (wifi.available()) 
        {
			wifi.read();
		}
    }
    Serial << F("RAM: ") << freeMemory() << endl;
    Serial << F("testWiFly(): exit") << endl;  
}

void sendRequest() {
    Serial << F("sendRequest(): enter") << endl;  

    Serial << F("RAM: ") << freeMemory() << endl;

    static int failedCount = 0;

	char bufRequest[REQUEST_BUFFER_SIZE];  
	PString strRequest(bufRequest, REQUEST_BUFFER_SIZE);
	
	// Build GET request
    strRequest << F("GET ") << jenkins.getPath() << F(" HTTP/1.1\n")
                << F("Host: ") << jenkins.getHost() << F("\n")
                << F("Connection: close\n") 
                << F("\n\n");
	
	// Open connection and send request. Timeout in 5s
	if (wifi.openConnection(jenkins.getHost(), 5000UL)) 
	{
        failedCount = 0;
        refreshDisplayAction.check();
		wifi << (const char*) strRequest << endl;
	} 
    else 
    {
		// Failed to open connection
		Serial << F("Failed connecting to: ") << jenkins.getHost() << endl;
        failedCount++;
	}	

    if (failedCount > 2)
    {
        Display.status(STATUS_CONNECTION_ERROR);
    }
    else
    {
        Display.status(STATUS_OK);
    }

    Serial << F("sendRequest(): exit") << endl;  
}


void displayBuildSate() {
    Project project;
    int buildingIndex = jenkins.projects.building();

    if (buildingIndex >= 0)
    {
        project = jenkins.projects.all[buildingIndex];
    }
    else
    {
        project = jenkins.projects.mostRecentBuild();
    }

    Serial << F("Projects: ") << jenkins.projects.count() << endl;

    for (int i=0; i<jenkins.projects.count(); i++)
    {
        Serial << F("Hash: ") << jenkins.projects.all[i].hash << endl;
        Serial << F("Last Build Time: ") << jenkins.projects.all[i].lastBuildTimeUTC << endl;
        Serial << F("Status: ") << (jenkins.projects.all[i].success ? F("Success") : F("Failure")) << endl;
        Serial << F("Activity: ") << (jenkins.projects.all[i].building ? F("Building") : F("Sleeping")) << endl;

    }
    Serial << F("Project: ") << project.hash << endl;

	if (project.building) 
    {
        lastBuild = millis();
        Display.buildInProgress(project.success);
	} 
    else 
    {
        // After 2 min turn off lights or notified of failure state
        if (millis() - lastBuild > 120000)
        {
            if (jenkins.projects.hasFailure())
            {
                Display.someBuildFailed();
            }
            else
            {
                Display.sleep();
            }
        }
        else
        {
            Display.buildDone(project.success);
        }
	}
}
