
simple SensorNode extends LoRaNode { //leaf nodes
    parameters:
        
        float temperature @unit("C");
        float humidity @unit("%");
        float windSpeed @unit("m/s"); 
        float CO2Level @unit("ppm");   
        float COLevel @unit("ppm");   
        float smokeDensity @unit("µg/m³");  
        
        bool fireDetected = default(false);
        
        float tempThreshold @unit("C") = default(70.0f);
        float smokeThreshold @unit("µg/m³") = default(500.0f);
        float CO2Threshold @unit("ppm") = default(1000.0f);
    
    gates:
        input radioIn;
        output radioOut;
}