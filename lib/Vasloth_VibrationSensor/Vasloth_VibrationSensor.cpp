#include "Vasloth_VibrationSensor.h"

Vasloth_VibrationSensor::Vasloth_VibrationSensor(uint8_t pin) {
    _pin = pin;
}

void Vasloth_VibrationSensor::begin() {
    analogReadResolution(12);
    _tStart = millis();
    _tSample = millis();
    _tWindow = millis();
}

void Vasloth_VibrationSensor::setThresholds(int deltaMin, long energiaMin, int persistencia) {
    _deltaMin = deltaMin;
    _energyThreshold = energiaMin;
    _persistenciaRequerida = persistencia;
}

void Vasloth_VibrationSensor::update() {

    unsigned long now = millis();

    // Warmup
    if (now - _tStart < _warmupTime) return;

    // Calibración inicial baseline
    if (!_baselineReady) {
        long acc = 0;
        for (int i = 0; i < 300; i++) {
            acc += analogRead(_pin);
            delay(2);
        }
        _baseline = acc / 300;
        _baselineReady = true;
        return;
    }

    // Muestreo
    if (now - _tSample >= _sampleMs) {
        _tSample = now;

        int lectura = analogRead(_pin);
        int delta = abs(lectura - _baseline);

        // Baseline adaptativo
        _baseline = (_baseline * 995 + lectura * 5) / 1000;

        if (delta > _deltaMin)
            _energy += delta;
    }

    // Evaluación por ventana
    if (now - _tWindow >= _windowMs) {
        _tWindow = now;

        if (_energy > _energyThreshold)
            _windowsWithVibration++;
        else
            _windowsWithVibration = 0;

        _vibrationFlag = (_windowsWithVibration >= _persistenciaRequerida);

        _energy = 0;
    }
}

bool Vasloth_VibrationSensor::vibrationDetected() {
    return _vibrationFlag;
}

long Vasloth_VibrationSensor::getEnergy() {
    return _energy;
}

int Vasloth_VibrationSensor::getBaseline() {
    return _baseline;
}