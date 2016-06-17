# README #

This project adheres to an [MIT License](LICENSE).

## About

The project connects a DHT22 sensor, a SparkFun soil moisture sensor and a DC motor to a Particle Photon. The readings from the sensor are reported to the users particle cloud and can be viewed on the dashboard. The motor can be controlled through the cloud interface as well, and there is an app which can be used for this purpose.

## Hardware

The schematic for this project is attached to the [schematic repo][Circuit Schematic].

## Related Repositories

1. [Circuit Schematic](https://github.com/Aditya90/GardenWatering-Schematic)
2. [Ionic app for control](https://github.com/Aditya90/GardenWatering-WebUi)

## References
1. [Software timer documentation of the particle photon](https://docs.particle.io/reference/firmware/photon/#software-timers)

## Todos
1. Make the soil moisture sensor manager a singleton
2. Instantiate the moisture sensors from the manager and not from the main app

## Credits
1. [DHT 22 Drivers - PietteTech](https://github.com/piettetech/PietteTech_DHT)
