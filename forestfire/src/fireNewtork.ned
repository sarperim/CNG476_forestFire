package forestfire.simulation;

import forestfire.simulation.nodes.SensorNode;
import forestfire.simulation.nodes.LoRaGateway;

network ForestNetwork {
    parameters:
        int numSensors = default(10); 
        
    submodules:
   
        sensor[numSensors]: SensorNode {
            @display("p=,,red");  
        }
        
 
        gateway: LoRaGateway {
            @display("p=300,300,gold"); 
        }
        
    connections:
        for i=0..numSensors-1 {
            sensor[i].radioOut --> gateway.radioIn++;
            gateway.radioOut --> sensor[i].radioIn;
        }
}