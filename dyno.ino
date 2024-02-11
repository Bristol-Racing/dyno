#include "sensors/loadCell.hpp"
#include "sensors/hallEffect.hpp"
#include "sensors/ADC_Current.hpp"
#include "sensors/voltage.hpp"
#include "sensors/temperature.hpp"
#include "sensors/sensorManager.hpp"

// Sensor pins
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 4;
#define CURRENT_VIN A0
#define VOLTAGE_VIN A6
#define TEMPERATURE_VIN A3
//  Plus pin 2 for hall effect

//  tick every 100ms
const int time_per_tick = 100;
//  callback every 100ms
const int time_per_callback = 1000;

//  Load cell arm length
const double arm_length = 142.5 / 1000.0;  //  meters

//  Initialise the 5 sensors
int sensorCount = 5;
Sensor::LoadCell scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, arm_length);
Sensor::HallEffect hallEffect;
Sensor::CurrentSensor currentSensor(1.0, 1.0);  //  Not calibrated
Sensor::VoltageSensor voltageSensor(VOLTAGE_VIN, 1.0);  //  Not calibrated
Sensor::TemperatureSensor temperature(TEMPERATURE_VIN);

//  Initialise the manager
Sensor::SensorManager manager(sensorCount, time_per_callback);

void setup() {
    //  Start serial communication
    Serial.begin(57600);

    //  set tick and report rate for the load cell
    scale.setTickRate(time_per_tick);
    scale.setReportRate(1000);

    //  Setup the current sensor and set tick and report rate
    currentSensor.setup();
    currentSensor.setTickRate(time_per_tick);
    currentSensor.setReportRate(1000);

    //  Other sensors just need report rates. All 1 second
    hallEffect.setReportRate(1000);
    voltageSensor.setReportRate(1000);
    temperature.setReportRate(1000);

    //  Add all the sensors to the manager
    manager.addSensor(&scale);
    manager.addSensor(&hallEffect);
    manager.addSensor(&currentSensor);
    manager.addSensor(&voltageSensor);
    manager.addSensor(&temperature);

    //  Set the callback function
    manager.setReportCallback(&reportCallback);
}

void loop() {
    //  Spin the manager, so that it schedules and handles sensor readings
    manager.spin();
}

//  Called by the sensor manager to pass readings back to the main program
void reportCallback(double * results) {
    //  Goes through and prints out all the sensor readings, separated by commas
    for (int i = 0; i < sensorCount; i++) {
        if (i > 0) {
            Serial.print(",");
        }
        Serial.print(results[i]);
    }
    Serial.println();
}
