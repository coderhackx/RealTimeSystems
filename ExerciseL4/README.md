# Group Exercise L-4: Memory Management (Group Submission)

**Member:**  
- Mariya 
- Khan
- Abdul  

---
## Project Title

**FreeRTOS Memory Management Demo (ESP32)**
![image](/images/memory.png)

This project demonstrates safe **stack** and **heap** usage in FreeRTOS on the ESP32.  
It shows how to monitor memory usage and avoid heap leaks when dynamically allocating memory.

Based on [Shawn Hymel’s FreeRTOS tutorials (2020)](https://github.com/ShawnHymel/freertos-skeleton), updated in **2025**.

---

## Features

- Runs on **ESP32** (single- or dual-core).
- Demonstrates:
  - Stack allocation (`int b[100];` on task stack).
  - Heap allocation with `pvPortMalloc()` and `vPortFree()`.
  - Monitoring stack usage with `uxTaskGetStackHighWaterMark()`.
  - Monitoring heap usage with `xPortGetFreeHeapSize()`.
- Prevents heap leaks by properly freeing allocated memory.

---

## Requirements

- **ESP32 board** (e.g., ESP32 DevKit, ESP32-WROOM).
- **Arduino IDE** (with ESP32 core) or PlatformIO.
- FreeRTOS (built-in with ESP32 Arduino core).

---

## How It Works

1. A FreeRTOS task (`testTask`) runs continuously:
   - Allocates a small array on the **stack**.
   - Dynamically allocates and frees memory on the **heap**.
   - Prints:
     - Remaining stack space (words).
     - Free heap size before and after allocation.

2. Example Serial Monitor output (**115200 baud**):

