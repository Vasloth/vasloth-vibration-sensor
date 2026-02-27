#ifndef VASLOTH_VIBRATION_SENSOR_H
#define VASLOTH_VIBRATION_SENSOR_H

#include <Arduino.h>

class Vasloth_VibrationSensor {
public:
  explicit Vasloth_VibrationSensor(uint8_t pin);

  void begin();
  void update();

  // === Telemetría / estado ===
  bool isWarmingUp() const;
  bool isBaselineReady() const;
  int  getBaseline() const;

  long getEnergyCurrentWindow() const;   // energía acumulada en la ventana actual
  long getEnergyLastWindow() const;      // energía de la última ventana cerrada

  int  getPersistenceWindows() const;    // ventanas con vibración consecutivas
  bool vibrationDetected() const;        // true si persistencia >= requerida

  // === Configuración (usuario) ===
  void setThresholds(int deltaMin, long energiaUmbral, int persistenciaReq);

  void setWarmupMs(int ms);
  void setSampleMs(int ms);
  void setWindowMs(int ms);

private:
  uint8_t _pin;

  // --- Parámetros configurables ---
  int _tiempoWarmup = 2000;
  int _sampleMs     = 5;
  int _ventanaMs    = 1000;

  int  _umbralDeltaMin = 40;
  long _umbralEnergia  = 6000;

  int _persistenciaRequerida = 3;

  // --- Estado interno ---
  unsigned long _t0 = 0;
  unsigned long _tVentana = 0;
  unsigned long _tSample = 0;

  int  _baseline = 0;
  bool _baselineListo = false;

  long _energia = 0;
  long _energiaUltimaVentana = 0;

  int  _ventanasConVibracion = 0;
  bool _vibracionReal = false;

  // Helper interno para reset limpio
  void resetRuntimeState(unsigned long ahora);
};

#endif