# ESP32-S3 FreeRTOS Demo: LED, Serial, and Compute Tasks

## Overview
This project demonstrates a basic FreeRTOS application on the **ESP32-S3** microcontroller. It runs multiple concurrent tasks, each with different priorities:

![image](/images/board3.png)

![image](/images/Exercise3.png)

``` 
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== Onboard RGB LED (ESP32-S3-DevKitC-1 v1.1) =====
#define LED_PIN    38
#define NUM_LEDS   1
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Optional: task handles for future diagnostics
TaskHandle_t hLED = nullptr, hSER = nullptr, hCPU = nullptr;

// ---------- Task: LED (highest priority) ----------
void TaskLED(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(400);   // 400 ms period

  uint32_t colors[] = {
    Adafruit_NeoPixel::Color(255, 0, 0),  // Red
    Adafruit_NeoPixel::Color(0, 255, 0),  // Green
    Adafruit_NeoPixel::Color(0, 0, 255),  // Blue
    0                                      // Off
  };
  int i = 0;

  for (;;) {
    // LED ON for 150 ms
    strip.setPixelColor(0, colors[i]);
    strip.show();
    vTaskDelay(pdMS_TO_TICKS(150));

    // LED OFF until the next period boundary (fixed-rate schedule)
    strip.setPixelColor(0, 0);
    strip.show();
    vTaskDelayUntil(&last, period);

    i = (i + 1) % 4; // R -> G -> B -> Off
  }
}

// ---------- Task: Serial (medium priority) ----------
void TaskSerial(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000);  // 1 s

  for (;;) {
    size_t freeHeap = xPortGetFreeHeapSize();
    Serial.printf("[t=%lu ms] freeHeap=%u bytes\n",
                  (unsigned long)millis(),
                  (unsigned int)freeHeap);
    vTaskDelayUntil(&last, period);
  }
}

// ---------- Task: Compute (lowest priority) ----------
void TaskCompute(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(200);   // 200 ms
  volatile uint32_t sink = 0;

  for (;;) {
    // Light CPU load: simple arithmetic loop
    for (int k = 0; k < 50000; ++k) {
      sink += (k ^ ((k << 1) + (k >> 1)));
    }
    vTaskDelayUntil(&last, period);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\nESP32-S3 FreeRTOS demo (LED + Serial + Compute)");

  // Init onboard RGB LED
  strip.begin();
  strip.setBrightness(120);  // 0..255
  strip.clear();
  strip.show();

  // Create tasks (bigger number = higher priority)
  xTaskCreate(TaskLED,    "TaskLED",    2048, NULL, 3, &hLED);
  xTaskCreate(TaskSerial, "TaskSerial", 3072, NULL, 2, &hSER);
  xTaskCreate(TaskCompute,"TaskCompute",2048, NULL, 1, &hCPU);
}

void loop() {
  // Not used: FreeRTOS scheduler runs our tasks
}

``` 

- **LED Task (High Priority)** – Controls the onboard RGB LED.
- **Serial Task (Medium Priority)** – Sends periodic messages over the serial port.
- **Compute Task (Low Priority)** – Performs light CPU-intensive computations in the background.

The project illustrates **task scheduling**, **timing**, and **priority handling** using FreeRTOS on the ESP32-S3.

---

## Features
- Onboard RGB LED control with adjustable brightness.
- Periodic serial logging to monitor system behavior.
- Background computation task simulating CPU load.
- Task priorities demonstrate FreeRTOS scheduling in a simple way.

---

## Hardware Requirements
- ESP32-S3 development board.
- Optional: Onboard RGB LED or compatible LED strip.

---

## Software Requirements
- Arduino IDE or PlatformIO.
- ESP32 board support installed in the Arduino IDE.
- FreeRTOS library (included with ESP32 Arduino core).

---

## Task Details
| Task         | Priority | Description                                      |
|--------------|----------|--------------------------------------------------|
| `TaskLED`    | 3        | Controls the onboard LED with a specific pattern.|
| `TaskSerial` | 2        | Prints status messages over serial every 500 ms. |
| `TaskCompute`| 1        | Performs a lightweight arithmetic loop every 200 ms.|

---

## Installation
1. Install **Arduino IDE** and ESP32 board support.
2. Clone or download this repository.
3. Open the project in Arduino IDE.
4. Select your ESP32-S3 board and port.
5. Upload the sketch to the board.

---

## Usage
- Open the Serial Monitor at **115200 baud** to see periodic messages from the Serial task.
- Observe the onboard RGB LED behavior.
- The Compute task runs in the background without blocking other tasks.

---

## Who is this for?
This project is ideal for:
- **Embedded developers** learning FreeRTOS task management.
- **Students** exploring multitasking and scheduling on ESP32.
- **Hobbyists** experimenting with ESP32 LED and serial applications.

---

## Notes
- The loop function is unused because the FreeRTOS scheduler handles all tasks.
- Task priorities are set to demonstrate how FreeRTOS manages CPU time allocation.

