#include "FireDetectionApp.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/INETDefs.h"

Define_Module(FireDetectionApp);

void FireDetectionApp::initialize()
{
    timer = new cMessage("sensingTimer");
    readParametersFromXML();

    // Schedule first sensing event
    scheduleAt(simTime() + sensingInterval, timer);
}

void FireDetectionApp::readParametersFromXML()
{
    cXMLElement *xmlConfig = par("configFile").xmlValue();
    if (!xmlConfig)
        throw cRuntimeError("FireDetectionApp: No XML config provided");

    // Read generalParameters
    cXMLElement *generalParams = xmlConfig->getFirstChildWithTag("generalParameters");
    if (!generalParams)
        throw cRuntimeError("FireDetectionApp: Missing <generalParameters> tag");

    const char *fireThStr = generalParams->getFirstChildWithTag("fireThreshold")->getNodeValue();
    fireThreshold = atof(fireThStr);

    const char *intervalStr = generalParams->getFirstChildWithTag("sensingInterval")->getNodeValue();
    sensingInterval = atof(intervalStr);

    // Read sensors
    cXMLElement *envSensors = xmlConfig->getFirstChildWithTag("environmentalSensors");
    if (!envSensors)
        throw cRuntimeError("FireDetectionApp: Missing <environmentalSensors> tag");

    auto sensorTags = envSensors->getChildren();
    for (auto it = sensorTags.begin(); it != sensorTags.end(); ++it) {
        cXMLElement *sensorElem = *it;
        if (strcmp(sensorElem->getTagName(), "sensor") == 0) {
            SensorInfo s;
            s.name = sensorElem->getAttribute("name");
            s.minVal = atof(sensorElem->getAttribute("min"));
            s.maxVal = atof(sensorElem->getAttribute("max"));
            s.threshold = atof(sensorElem->getAttribute("threshold"));
            sensors.push_back(s);
        }
    }
}

double FireDetectionApp::simulateSensorValue(const SensorInfo& sensor)
{
    // Simple random value between min and max
    return sensor.minVal + uniform(0, 1) * (sensor.maxVal - sensor.minVal);
}

bool FireDetectionApp::detectFire()
{
    // Check if any sensor exceeds its threshold
    for (const auto& s : sensors) {
        double val = simulateSensorValue(s);
        EV_INFO << "Sensor " << s.name.c_str() << " value: " << val << ", threshold: " << s.threshold << "\n";
        if (val >= s.threshold) {
            EV_INFO << "Fire detected by sensor: " << s.name.c_str() << "\n";
            return true;
        }
    }
    return false;
}

void FireDetectionApp::handleMessage(cMessage *msg)
{
    if (msg == timer) {
        if (detectFire()) {
            sendFireAlert();
        }
        // Reschedule sensing timer
        scheduleAt(simTime() + sensingInterval, timer);
    }
    else {
        delete msg;
    }
}

void FireDetectionApp::sendFireAlert()
{
    // Create a simple packet to send to gateway
    auto pkt = new inet::Packet("FireAlert");
    // For simplicity, just send empty packet or add fire info as a tag or payload here

    // Send packet to network layer for transmission
    send(pkt, "radioOut");

    EV_INFO << "Fire alert packet sent at " << simTime() << "\n";
}

void FireDetectionApp::finish()
{
    cancelAndDelete(timer);
}
