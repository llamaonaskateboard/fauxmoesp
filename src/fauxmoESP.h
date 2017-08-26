/*

FAUXMO ESP 2.0.0

Copyright (C) 2016 by Xose Pérez <xose dot perez at gmail dot com>

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef FAUXMOESP_h
#define FAUXMOESP_h

#define DEBUG_FAUXMO Serial

#define DEFAULT_TCP_BASE_PORT   49153
#define UDP_MULTICAST_IP        IPAddress(239,255,255,250)
#define UDP_MULTICAST_PORT      1900
#define TCP_MAX_CLIENTS         10

#define UDP_SEARCH_PATTERN      "M-SEARCH"
#define UDP_DEVICE_PATTERN      "urn:Belkin:device:**"
#define UDP_EVENT_PATTERN       "urn:Belkin:service:basicevent:1"
#define UDP_ROOT_PATTERN        "upnp:rootdevice"

#define UDP_RESPONSES_INTERVAL  250
#define UDP_RESPONSES_TRIES     5

const char UDP_TEMPLATE[] PROGMEM =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Sun, 20 Nov 2016 00:00:00 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://%s:%d/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: %s\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "X-User-Agent: redsonic\r\n"
    "ST: urn:Belkin:service:basicevent:1\r\n"
    "USN: uuid:Socket-1_0-%s::urn:Belkin:service:basicevent:1\r\n\r\n";

const char SETUP_TEMPLATE[] PROGMEM =
    "<?xml version=\"1.0\"?>"
    "<root><device>"
        "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
        "<friendlyName>%s</friendlyName>"
        "<manufacturer>Belkin International Inc.</manufacturer>"
        "<manufacturerURL>http://www.belkin.com</manufacturerURL>"
        "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>"
        "<modelName>Socket</modelName>"
        "<modelNumber>1.0</modelNumber>"
        "<modelURL>http://www.belkin.com/plugin/</modelURL>"
        "<serialNumber>%s</serialNumber>"
        "<UDN>uuid:Socket-1_0-%s</UDN>"
        "<UPC>123456789</UPC>"
        "<macAddress>%s</macAddress>"
        "<firmwareVersion>WeMo_WW_2.00.10966.PVT-OWRT-SNS</firmwareVersion>"
        "<iconVersion>0|49153</iconVersion>"
        "<binaryState>1</binaryState>"
        "<iconList>"
        "   <icon>"
        "       <mimetype>jpg</mimetype>"
        "       <width>100</width>"
        "       <height>100</height>"
        "       <depth>100</depth>"
        "       <url>icon.jpg</url>"
        "   </icon>"
        "</iconList>"
        "<serviceList>"
            "<service>"
                "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                "<controlURL>/upnp/control/basicevent1</controlURL>"
                "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                "<SCPDURL>/eventservice.xml</SCPDURL>"
            "</service>"
            "<service>"
                "<serviceType>urn:Belkin:service:deviceinfo:1</serviceType>"
                "<serviceId>urn:Belkin:serviceId:deviceinfo1</serviceId>"
                "<controlURL>/upnp/control/deviceinfo1</controlURL>"
                "<eventSubURL>/upnp/event/deviceinfo1</eventSubURL>"
                "<SCPDURL>/deviceinfoservice.xml</SCPDURL>"
            "</service>"
        "</serviceList>"
        "<presentationURL>/pluginpres.html</presentationURL>"
    "</device></root>";


const char EVENTSERVICE_TEMPLATE[] PROGMEM =
    "<?xml version=\"1.0\"?>"
    "<scpd xmlns=\"urn:Belkin:service-1-0\">"
    "  <specVersion>"
    "    <major>1</major>"
    "    <minor>0</minor>"
    "  </specVersion>"
    "  <actionList>"
    "    <action>"
    "      <name>SetBinaryState</name>"
    "      <argumentList>"
    "         <argument>"
    "           <retval />"
    "           <name>BinaryState</name>"
    "           <relatedStateVariable>BinaryState</relatedStateVariable>"
    "           <direction>in</direction>"
    "          </argument>"
    "      </argumentList>"
    "    </action>"
    "    <action>"
    "      <name>GetBinaryState</name>"
    "      <argumentList>"
    "        <argument>"
    "          <retval />"
    "          <name>BinaryState</name>"
    "          <relatedStateVariable>BinaryState</relatedStateVariable>"
    "          <direction>out</direction>"
    "        </argument>"
    "      </argumentList>"
    "    </action>"
    "  </actionList>"
    "  <serviceStateTable>"
    "    <stateVariable sendEvents=\"yes\">"
    "      <name>BinaryState</name>"
    "      <dataType>string</dataType>"
    "      <defaultValue>0</defaultValue>"
    "    </stateVariable>"
    "  </serviceStateTable>"
    "</scpd>";

const char DEVICEINFOSERVICE_TEMPLATE[] PROGMEM = "<scpd xmlns=\"urn:Belkin:service-1-0\"><specVersion><major>1</major><minor>0</minor></specVersion><actionList><action><name>GetDeviceInformation</name><argumentList><argument><retval/><name>UTC</name><relatedStateVariable>UTC</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>TimeZone</name><relatedStateVariable>TimeZone</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>dst</name><relatedStateVariable>dst</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>DstSupported</name><relatedStateVariable>DstSupported</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>DeviceInformation</name><relatedStateVariable>DeviceInformation</relatedStateVariable><direction>out</direction></argument><!--   Adding Countdown Time  --><argument><retval/><name>CountdownTime</name><relatedStateVariable>CountdownTime</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>TimeSync</name><relatedStateVariable>TimeSync</relatedStateVariable><direction>out</direction></argument></argumentList></action><action><name>GetInformation</name><argumentList><argument><retval/><name>UTC</name><relatedStateVariable>UTC</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>TimeZone</name><relatedStateVariable>TimeZone</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>dst</name><relatedStateVariable>dst</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>DstSupported</name><relatedStateVariable>DstSupported</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>Information</name><relatedStateVariable>Information</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>TimeSync</name><relatedStateVariable>TimeSync</relatedStateVariable><direction>out</direction></argument></argumentList></action><action><name>OpenInstaAP</name><argumentList><argument></argument></argumentList></action><action><name>CloseInstaAP</name><argumentList><argument></argument></argumentList></action><action><name>GetConfigureState</name><argumentList><argument><retval/><name>ConfigureState</name><relatedStateVariable>ConfigureState</relatedStateVariable><direction>out</direction></argument></argumentList></action><action><name>InstaConnectHomeNetwork</name><argumentList><argument><retval/><name>ssid</name><relatedStateVariable>ssid</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>auth</name><relatedStateVariable>auth</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>password</name><relatedStateVariable>password</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>encrypt</name><relatedStateVariable>encrypt</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>channel</name><relatedStateVariable>channel</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>brlist</name><relatedStateVariable>brlist</relatedStateVariable><direction>in</direction></argument></argumentList></action><action><name>UpdateBridgeList</name><argumentList><argument><retval/><name>BridgeList</name><relatedStateVariable>BridgeList</relatedStateVariable><direction>in</direction></argument></argumentList></action><action><name>GetRouterInformation</name><argumentList><argument><retval/><name>mac</name><relatedStateVariable>mac</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>ssid</name><relatedStateVariable>ssid</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>auth</name><relatedStateVariable>auth</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>password</name><relatedStateVariable>password</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>encrypt</name><relatedStateVariable>encrypt</relatedStateVariable><direction>out</direction></argument><argument><retval/><name>channel</name><relatedStateVariable>channel</relatedStateVariable><direction>out</direction></argument></argumentList></action><action><name>InstaRemoteAccess</name><argumentList><argument><retval/><name>DeviceId</name><relatedStateVariable>DeviceId</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>dst</name><relatedStateVariable>dst</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>HomeId</name><relatedStateVariable>HomeId</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>DeviceName</name><relatedStateVariable>DeviceName</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>MacAddr</name><relatedStateVariable>MacAddr</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>pluginprivateKey</name><relatedStateVariable>pluginprivateKey</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>smartprivateKey</name><relatedStateVariable>smartprivateKey</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>smartUniqueId</name><relatedStateVariable>smartUniqueId</relatedStateVariable><direction>in</direction></argument><argument><retval/><name>numSmartDev</name><relatedStateVariable>numSmartDev</relatedStateVariable><direction>in</direction></argument></argumentList></action></actionList><serviceStateTable><stateVariable sendEvents=\"yes\"><name>DeviceInformation</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><!--   Adding Countdown Time  --><stateVariable sendEvents=\"yes\"><name>CountdownTime</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>Information</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>ConfigureState</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>BridgeList</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>mac</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>ssid</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>auth</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>password</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>encrypt</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>channel</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>PairingStatus</name><dataType>string</dataType><defaultValue>Connecting</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>statusCode</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents=\"yes\"><name>TimeSync</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable></serviceStateTable></scpd>";

const char BASICEVENT_TEMPLATE[] PROGMEM =
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>"
    "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">"
    "<BinaryState>0</BinaryState>"
    "</u:GetBinaryStateResponse>"
    "</s:Body> </s:Envelope>";

const char HEADERS[] PROGMEM =
    "HTTP/1.1 200 OK\r\n"
    "CONTENT-LENGTH: %d\r\n"
    "CONTENT-TYPE: text/xml\r\n"
    "DATE: Sun, 01 Jan 2017 00:00:00 GMT\r\n"
    "LAST-MODIFIED: Sat, 01 Jan 2017 00:00:00 GMT\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "X-USER-AGENT: redsonic\r\n"
    "CONNECTION: close\r\n\r\n"
    "%s\r\n";

#ifdef DEBUG_FAUXMO
    #define DEBUG_MSG_FAUXMO(...) DEBUG_FAUXMO.printf( __VA_ARGS__ )
#else
    #define DEBUG_MSG_FAUXMO(...)
#endif

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <WiFiUdp.h>
#include <functional>
#include <vector>

typedef std::function<void(unsigned char, const char *, bool)> TStateFunction;

typedef struct {
    char * name;
    char * uuid;
    bool hit;
    AsyncWebServer * server;
} fauxmoesp_device_t;

class fauxmoESP {

    public:

        fauxmoESP(unsigned int port = DEFAULT_TCP_BASE_PORT);
        void addDevice(const char * device_name);
        void onMessage(TStateFunction fn) { _callback = fn; }
        void enable(bool enable);
        void handle();

    private:

        bool _enabled = true;
        unsigned int _base_port = DEFAULT_TCP_BASE_PORT;
        std::vector<fauxmoesp_device_t> _devices;
        WiFiUDP _udp;
        TStateFunction _callback = NULL;

        unsigned int _roundsLeft = 0;
        unsigned int _current = 0;
        unsigned long _lastTick;
        IPAddress _remoteIP;
        unsigned int _remotePort;

        void _sendUDPResponse(unsigned int device_id);
        void _nextUDPResponse();
        void _handleUDPPacket(const IPAddress remoteIP, unsigned int remotePort, uint8_t *data, size_t len);
        void _handleSetup(AsyncWebServerRequest *request, unsigned int device_id);
        void _handleEventService(AsyncWebServerRequest *request, unsigned int device_id);
        void _handleDeviceInfoService(AsyncWebServerRequest *request, unsigned int device_id);
        void _handleContent(AsyncWebServerRequest *request, unsigned int device_id, char * content);

};

#endif
