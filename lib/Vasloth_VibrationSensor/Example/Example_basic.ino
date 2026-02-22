#include <Vasloth_VibrationSensor.h>

Vasloth_VibrationSensor sensor(4);

void setup() {
    Serial.begin(115200);
    sensor.begin();
}

void loop() {
    sensor.update();

    if (sensor.vibrationDetected()) {
        Serial.println(">>> Vibracion real detectada");
    }
}