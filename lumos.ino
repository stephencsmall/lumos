#include "HTU21D.h"
#include "Adafruit_TSL2561_U.h"

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
HTU21D htu = HTU21D();
UDP Udp;

void configureTSLSensor(void) {
    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}

int sendUdp(double val, String type) {

    char tempString[128];
    int sent;
    Udp.begin(4444);
    Udp.beginPacket(IPAddress(10, 45, 0, 109), 4444);
    sprintf(tempString, type +" value=%f", val);
    sent = Udp.write(tempString);
        if (sent < 1) {
            Udp.endPacket();
            Udp.stop();
            return -1;
        }
    Udp.endPacket();
    Udp.stop();
    return 0;
}

void setup(void) {

    Particle.publish("Light Sensor Startup...");

    /* Initialise the TSL2561 sensor */
    if(!tsl.begin()) {
        Particle.publish("Ooops, no TSL2561 detected. Check  wiring or I2C ADDR!");
        while(1);
    }
    /* Initialise the HTU21D sensor */
    if(!htu.begin()) {
        Particle.publish("Ooops, no HTU21D detected. Check  wiring or I2C ADDR!");
        while(1);
    }

    /* Setup the sensor gain and integration time */
    configureTSLSensor();

}

void loop(void) {

    /* Get a new sensor event */
    sensors_event_t event;
    tsl.getEvent(&event);
	if (sendUdp(htu.readHumidity(), "hum")<0){
		Particle.publish("Failed to send a UDP packet");
	}
	if (sendUdp(htu.readTemperature(), "temp")<0){
		Particle.publish("Failed to send a UDP packet");
	}
    /* Display the results (light is measured in lux) */
    if (event.light) {
        if (sendUdp(event.light, "lux")<0){
           Particle.publish("Failed to send a UDP packet");
        }
    }
    else {
        /* If event.light = 0 lux the sensor is probably saturated
        and no reliable data could be generated! */
        sendUdp(0,"lux");
        Particle.publish("Sensor overload, sending 0");
    }
    delay(1000);
}
