#include "Vasloth_VibrationSensor.h"

Vasloth_VibrationSensor::Vasloth_VibrationSensor(uint8_t pin) : _pin(pin) {}

void Vasloth_VibrationSensor::resetRuntimeState(unsigned long ahora) {
  _energia = 0;
  _energiaUltimaVentana = 0;
  _ventanasConVibracion = 0;
  _vibracionReal = false;

  // Mantener timing consistente
  _tVentana = ahora;
  _tSample  = ahora;
}

void Vasloth_VibrationSensor::begin() {
  analogReadResolution(12);
  delay(200);

  unsigned long ahora = millis();

  _t0 = ahora;
  _tVentana = ahora;
  _tSample = ahora;

  _baseline = 0;
  _baselineListo = false;

  resetRuntimeState(ahora);
}

void Vasloth_VibrationSensor::setThresholds(int deltaMin, long energiaUmbral, int persistenciaReq) {
  _umbralDeltaMin = deltaMin;
  _umbralEnergia = energiaUmbral;
  _persistenciaRequerida = persistenciaReq;
}

void Vasloth_VibrationSensor::setWarmupMs(int ms) {
  _tiempoWarmup = ms;
}

void Vasloth_VibrationSensor::setSampleMs(int ms) {
  _sampleMs = ms;
}

void Vasloth_VibrationSensor::setWindowMs(int ms) {
  _ventanaMs = ms;
}

void Vasloth_VibrationSensor::update() {
  unsigned long ahora = millis();

  // ===== Warmup =====
  if (ahora - _t0 < (unsigned long)_tiempoWarmup) {
    // Durante warmup: no acumular estados raros ni ventanas
    resetRuntimeState(ahora);
    return;
  }

  // ===== Baseline inicial =====
  if (!_baselineListo) {
    long acc = 0;
    for (int i = 0; i < 300; i++) {
      acc += analogRead(_pin);
      delay(2);
    }
    _baseline = acc / 300;
    _baselineListo = true;

    // Arranque limpio post-baseline
    resetRuntimeState(ahora);
    return;
  }

  // ===== Muestreo =====
  if (ahora - _tSample >= (unsigned long)_sampleMs) {
    _tSample = ahora;

    int lectura = analogRead(_pin);
    int delta = abs(lectura - _baseline);

    // Baseline adaptativo (ajuste fino)
    _baseline = (_baseline * 995 + lectura * 5) / 1000;

    if (delta > _umbralDeltaMin) {
      _energia += delta;
    }
  }

  // ===== Evaluación por ventana =====
  if (ahora - _tVentana >= (unsigned long)_ventanaMs) {
    _tVentana = ahora;

    // Telemetría: energía de la ventana cerrada
    _energiaUltimaVentana = _energia;

    // Persistencia
    if (_energia > _umbralEnergia) {
      _ventanasConVibracion++;
    } else {
      _ventanasConVibracion = 0;
    }

    _vibracionReal = (_ventanasConVibracion >= _persistenciaRequerida);

    // Reset para próxima ventana
    _energia = 0;
  }
}

// ===== Getters / telemetría =====
bool Vasloth_VibrationSensor::isWarmingUp() const {
  return (millis() - _t0) < (unsigned long)_tiempoWarmup;
}

bool Vasloth_VibrationSensor::isBaselineReady() const {
  return _baselineListo;
}

int Vasloth_VibrationSensor::getBaseline() const {
  return _baseline;
}

long Vasloth_VibrationSensor::getEnergyCurrentWindow() const {
  return _energia;
}

long Vasloth_VibrationSensor::getEnergyLastWindow() const {
  return _energiaUltimaVentana;
}

int Vasloth_VibrationSensor::getPersistenceWindows() const {
  return _ventanasConVibracion;
}

bool Vasloth_VibrationSensor::vibrationDetected() const {
  return _vibracionReal;
}