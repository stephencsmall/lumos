# lumos
Particle-based household sensor cluster with influxdb logging

## What is Lumos?
Lumos is device for gathering light, temperature, and humidity data based on the Particle Photon, HTU21D, TSL2561, and InfluxDB.  It is a simple sketch that gathers sensor data and forwards it to InfluxDB via UDP.  It's originally built for Adafruit's HTU21D and TSL2561 sensor breakouts, but could probably be adapted to anything that uses I2C.

![img.jpg](img.jpg)
