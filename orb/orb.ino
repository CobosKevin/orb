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
#include <WiFlySerial.h>
#include <FreeMemory.h>

#include "Credentials.h"
#include "LEDDisplay.h"
#include "Jenkins.h"


#define RED_LED_PIN    9U
#define GREEN_LED_PIN 10U
#define BLUE_LED_PIN  11U

#define SETUP_MODE_PIN 13U

#define REQUEST_BUFFER_SIZE 180

#define JENKINS_CONNECT_TIMEOUT 5000UL
#define JENKINS_CONNECT_ATEMPTS 4

Jenkins jenkins;
WiFlySerial wifi(ARDUINO_RX_PIN, ARDUINO_TX_PIN);
// Check the state of the build periodically
TimedAction getBuildStateAction(30000, sendRequest);
// Update display periodically
TimedAction refreshDisplayAction(5, refreshDisplay);

static boolean fSetupMode = false;
static boolean fSetupFailed = false;

void setup()
{
    pinMode(SETUP_MODE_PIN, INPUT);
    fSetupMode = digitalRead(SETUP_MODE_PIN);

    Serial.begin(38400);

    Serial << F("setup(): enter") << endl;
    Serial << F("RAM: ") << freeMemory() << endl;

    Display.setRGBPins(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);
    Display.test();

    if (!fSetupMode) 
    {
        Display.status(STATUS_NO_NETWORK);
        fSetupFailed = !startWiFly();
        if (!fSetupFailed)
        {
            Display.status(STATUS_OK);
            sendRequest();
        }
        else
        {
            Serial << F("Failed to acquire network") << endl;        
        }
    }
    else
    {
        Display.status(STATUS_SETUP_MODE);
        Serial << F("In setup mode") << endl;
    }

    Serial << F("setup(): exit") << endl;
}

void loop()
{
    refreshDisplayAction.check();

    if (!fSetupMode && !fSetupFailed) 
    {
        if (wifi.isConnectionOpen() && wifi.available() > 0)
        {
            while (wifi.available() > 0)
            {
                refreshDisplayAction.check();
                jenkins.parse((char) wifi.read());
                delay(1);
            }
            wifi.closeConnection();
            displayBuildState();
        }
        getBuildStateAction.check();
    }
}

void refreshDisplay()
{
    Display.refresh();
}

void refreshDisplayActionCheck()
{
    refreshDisplayAction.check();
}

boolean startWiFly()
{
    boolean fConnected = false;
    char bufRequest[REQUEST_BUFFER_SIZE];

    Serial << F("startWiFly(): enter") << endl;
    Serial << F("RAM: ") << freeMemory() << endl;

    wifi.setWaitCallback(refreshDisplayActionCheck);

    if (wifi.begin())
    {
        Serial << F("MAC: ") << wifi.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;

        wifi.getDeviceStatus();

        if (wifi.isifUp())
        {
            fConnected = true;
        }
        else
        {
            // Try to join any open network
            wifi.setAuthMode(WIFLY_AUTH_OPEN);
            wifi.setDHCPMode(WIFLY_DHCP_ON);
            wifi.setJoinMode(WIFLY_JOIN_ANY);

            if (wifi.join())
            {
                fConnected = true;
            }
            else
            {
                // Can't seem to connect. Go into ADHOC mode so that we can telnet to device
                Serial << F("setDHCP:") << wifi.setDHCPMode(WIFLY_DHCP_OFF) << endl;
                Serial << F("setAuthMode:") << wifi.setAuthMode(WIFLY_AUTH_OPEN) << endl;
                Serial << F("setSSID:") << wifi.setSSID("orb") << endl;
                Serial << F("setIP:") << wifi.setIP("169.254.1.1") << endl;
                Serial << F("setNetMask:") << wifi.setNetMask("255.255.0.0") << endl;
                Serial << F("setChannel:") << wifi.setChannel("1") << endl;
                Serial << F("setJoinMode:") << wifi.setJoinMode(WIFLY_JOIN_MAKE_ADHOC) << endl;

                Serial << F("Going into ADHOC mode") << endl;

                // This will reset the wifly module and go into ADHOC mode
                wifi.setProtocol(WIFLY_IPMODE_TCP);
            }
        }

        if (fConnected)
        {
            wifi.setNTP(ntpHost);

            Serial << F("IP: ") << wifi.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl
                   << F("Netmask: ") << wifi.getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl
                   << F("Gateway: ") << wifi.getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl
                   << F("DNS: ") << wifi.getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl;

            wifi.closeConnection();
            wifi.flush();
            while (wifi.available())
            {
                wifi.read();
            }
        }
    }

    Serial << F("RAM: ") << freeMemory() << endl;
    Serial << F("startWiFly(): exit") << endl;
    return fConnected;
}

void sendRequest()
{
    Serial << F("sendRequest(): enter") << endl;

    Serial << F("RAM: ") << freeMemory() << endl;

    static int failedCount = 0;

    char bufRequest[REQUEST_BUFFER_SIZE];
    PString strRequest(bufRequest, REQUEST_BUFFER_SIZE);

    // Build GET request
    strRequest << F("GET ") << jenkins.getPath() << F(" HTTP/1.1\n")
               << F("Host: ") << jenkinsHost << F("\n")
               << F("Connection: close\n")
               << F("\n\n");

    if (wifi.openConnection(jenkinsHost, JENKINS_CONNECT_TIMEOUT))
    {
        failedCount = 0;
        wifi << (const char*) strRequest << endl;
    }
    else
    {
        // Failed to open connection
        Serial << F("Failed connecting to: ") << jenkinsHost << endl;
        failedCount++;
    }

    if (failedCount >= JENKINS_CONNECT_ATEMPTS)
    {
        Display.status(STATUS_CONNECTION_ERROR);
    }
    else
    {
        Display.status(STATUS_OK);
    }

    Serial << F("sendRequest(): exit") << endl;
}


void displayBuildState()
{
    static long lastBuildTime = 0;

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
        lastBuildTime = millis();
        Display.buildInProgress(project.success);
    }
    else
    {
        // After 2 min turn off lights or notify of failure state
        if (millis() - lastBuildTime > 120000)
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
