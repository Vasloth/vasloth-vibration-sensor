#include <Arduino.h>
#include <Vasloth_VibrationSensor.h>

Vasloth_VibrationSensor sensor(4);

void setup() {
  Serial.begin(9600);
  sensor.begin();

  // --- Configuración recomendada (perfil normal) ---
  sensor.setWarmupMs(2000);
  sensor.setSampleMs(5);      // 200 Hz
  sensor.setWindowMs(1000);   // 1s

  // deltaMin, energiaUmbral, persistenciaReq
  sensor.setThresholds(40, 6000, 3);

  Serial.println(">>> VASLOTH V-S: Monitoreo configurable");
}

void loop() {
  sensor.update();

  if (sensor.isWarmingUp()) return;

  if (!sensor.isBaselineReady()) {
    Serial.println("Calibrando baseline...");
    delay(250);
    return;
  }

  Serial.print("Baseline: ");
  Serial.print(sensor.getBaseline());

  Serial.print(" | Energia: ");
  Serial.print(sensor.getEnergyLastWindow());

  Serial.print(" | Ventanas: ");
  Serial.print(sensor.getPersistenceWindows());

  if (sensor.vibrationDetected()) {
    Serial.println(" | >>> VIBRACION REAL DETECTADA");
  } else {
    Serial.println(" | Estado: Filtrando/Quieto");
  }

  delay(100);
}