#ifndef VASLOTH_VIBRATION_SENSOR_H
#define VASLOTH_VIBRATION_SENSOR_H

#include <Arduino.h>

class Vasloth_VibrationSensor {
public:
    Vasloth_VibrationSensor(uint8_t pin);

    void begin();
    void update();

    bool vibrationDetected();
    long getEnergy();
    int getBaseline();

    void setThresholds(int deltaMin, long energiaMin, int persistencia);

private:
    uint8_t _pin;

    // Parámetros
    const int _warmupTime = 2000;
    const int _sampleMs = 5;
    const int _windowMs = 1000;

    int _deltaMin = 40;
    long _energyThreshold = 6000;
    int _persistenciaRequerida = 3;

    // Estado interno
    unsigned long _tStart;
    unsigned long _tSample;
    unsigned long _tWindow;

    bool _baselineReady = false;
    int _baseline = 0;
    long _energy = 0;
    int _windowsWithVibration = 0;

    bool _vibrationFlag = false;
};

#endif