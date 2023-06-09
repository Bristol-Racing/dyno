#include "sensors/loadCell.hpp"
#include "sensors/hallEffect.hpp"
#include "sensors/ADC_Current.hpp"
#include "sensors/voltage.hpp"
#include "sensors/temperature.hpp"
#include "sensors/sensorManager.hpp"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 4;

#define CURRENT_VIN A0
#define VOLTAGE_VIN A6
#define TEMPERATURE_VIN A3

const int time_per_reading = 100;
const int readings = 10;

const double arm_length = 142.5 / 1000.0;  //  meters

int currentReadings = 0;
long reading = 0;

int sensorCount = 5;
Sensor::LoadCell scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, arm_length);
Sensor::HallEffect hallEffect;
Sensor::CurrentSensor currentSensor;
Sensor::VoltageSensor voltageSensor(VOLTAGE_VIN, 993000.0, 219200.0);
Sensor::TemperatureSensor temperature(TEMPERATURE_VIN);

Sensor::SensorManager manager(sensorCount, time_per_reading * readings);

void setup() {
    Serial.begin(57600);

    scale.setTickRate(time_per_reading);
    scale.setReadRate(1000);

    currentSensor.setup();
    currentSensor.setTickRate(time_per_reading);
    currentSensor.setReadRate(1000);

    hallEffect.setTickRate(1);
    hallEffect.setReadRate(1000);

    voltageSensor.setReadRate(1000);

    temperature.setReadRate(1000);

    manager.addSensor(&scale);
    manager.addSensor(&hallEffect);
    manager.addSensor(&currentSensor);
    manager.addSensor(&voltageSensor);
    manager.addSensor(&temperature);
    manager.setReadCallback(&readCallback);
}

void loop() {
    manager.spin();
}

void readCallback(double * results) {
    for (int i = 0; i < sensorCount; i++) {
        if (i > 0) {
            Serial.print(",");
        }
        Serial.print(results[i]);
    }
    Serial.println();
}
