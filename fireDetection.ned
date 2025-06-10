//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package flora.simulations;

import inet.applications.udpapp.UdpBasicApp;
import inet.networklayer.configurator.ipv4.Ipv4FlatNetworkConfigurator;
import inet.node.inet.Router;
import inet.node.internetcloud.InternetCloud;
import flora.LoRaPhy.LoRaMedium;
import flora.LoraNode.LoRaNode;
import flora.LoraNode.LoRaGW;
import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth1G;

network fireDetection
{
    parameters:
        int networkSizeX = default(1000);  // smaller area for demo
        int networkSizeY = default(1000);
        @display("bgi=background/map1;bgb=1000,1000");
    submodules:
        // Gateway node at center
        GW: LoRaGW {
            @display("p=500,500;is=s");
        }

        // Fire sensor nodes scattered around
        FireSensor1: LoRaNode {
            @display("p=300,400");
        }

        FireSensor2: LoRaNode {
            @display("p=700,400");
        }

        FireSensor3: LoRaNode {
            @display("p=400,700");
        }

        FireSensor4: LoRaNode {
            @display("p=600,300");
        }

        FireSensor5: LoRaNode {
            @display("p=350,550");
        }

        // Wireless medium module
        LoRaMedium: LoRaMedium {
            @display("p=900,100");
        }

        // Optional: Network server for receiving alarms
        networkServer: StandardHost {
            parameters:
                @display("p=900,900");
        }

        // Optional: Routers and internet cloud if needed
        gwRouter: Router {
            @display("p=100,900");
        }
        nsRouter: Router {
            @display("p=722.88,899.584");
        }
        internetCloud: InternetCloud {
            @display("p=477.904,899.584");
        }
        ipv4FlatNetworkConfigurator: Ipv4FlatNetworkConfigurator {
            @display("p=899.584,248.99199");
        }
    connections allowunconnected:
        
        // Wired connections for gateways and routers/internet
        networkServer.ethg++ <--> Eth1G <--> nsRouter.ethg++;
        nsRouter.pppg++ <--> Eth1G <--> internetCloud.pppg++;
        internetCloud.pppg++ <--> Eth1G <--> gwRouter.pppg++;
        gwRouter.ethg++ <--> Eth1G <--> GW.ethg++;
}
