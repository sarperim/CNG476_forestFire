#include <omnetpp.h>
// If Flora is in a sibling directory relative to your project:
#include "/home/username/omnetpp-6.0/samples/flora/src/flora/node/LoRaNode.h"


using namespace omnetpp;

class SensorNode : public ::org::flora::node::LoRaNode {
  protected:
    // Fire detection logic
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Helper function to update environmental parameters
    void updateEnvironment();
};


Define_Module(SensorNode);

void SensorNode::initialize() {
    // Schedule periodic fire checks (every 5 minutes)
    scheduleAt(simTime() + 300, new cMessage("checkFire"));
}

void SensorNode::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Read parameters (as float)
        float temp = par("temperature").floatValue();
        float smoke = par("smokeDensity").floatValue();
        float co2 = par("CO2Level").floatValue();

        // Fire detection logic
        if (temp > par("tempThreshold").floatValue() ||
            smoke > par("smokeThreshold").floatValue() ||
            co2 > par("CO2Threshold").floatValue()) {

            fireDetected = true;
            EV << "Fire detected at node " << getIndex() << endl;
            send(new cMessage("FIRE_ALERT"), "radioOut");
        }

        // Simulate environmental changes
        updateEnvironment();

        // Re-schedule
        scheduleAt(simTime() + 300, msg);
    }
}

void SensorNode::updateEnvironment() {
    // Simulate wind spreading CO2/smoke
    float wind = par("windSpeed").floatValue();
    if (wind > 5.0f) {
        par("CO2Level").setFloatValue(par("CO2Level").floatValue() + wind * 0.1f);
        par("smokeDensity").setFloatValue(par("smokeDensity").floatValue() + wind * 1.0f);
    }
}
