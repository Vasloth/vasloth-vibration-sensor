# Vasloth Vibration Sensor Library (V1)

Official Arduino-compatible library for the **Vasloth Vibration Sensor (V1)** developed by Vasloth Hardware.

---

## Overview

This library implements the official signal processing logic for the Vasloth Vibration Sensor, including:

- Adaptive baseline stabilization  
- Energy accumulation per time window  
- Persistence filtering  
- False positive reduction  
- Reliable structural vibration detection  

The sensor provides an analog output optimized for direct ADC reading.  
This library handles the detection logic in firmware.

---

## Features

- Automatic warm-up stabilization  
- 200 Hz sampling  
- Energy-based vibration analysis  
- Configurable thresholds  
- Persistence-based detection  
- Compatible with ESP32, Arduino and similar platforms  

---

## Basic Example

```cpp
#include <Vasloth_VibrationSensor.h>

Vasloth_VibrationSensor sensor(4);

void setup() {
    Serial.begin(115200);
    sensor.begin();
}

void loop() {
    sensor.update();

    if (sensor.vibrationDetected()) {
        Serial.println("Vibration detected");
    }
}