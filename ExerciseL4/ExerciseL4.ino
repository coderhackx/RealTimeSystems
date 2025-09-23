/**
 * FreeRTOS Memory Management Demo (Fixed Version)
 * 
 * Demonstrates safe stack and heap usage on ESP32.
 * 
 * Based on: Shawn Hymel (2020)
 * Updated: 2025
 */

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Task: Perform some mundane task
void testTask(void *parameter) {
  while (1) {
    int a = 1;
    int b[100];  // small local array on stack (~400 bytes)

    // Do something with array so it's not optimized out
    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }

    // Print out remaining stack memory (words)
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Allocate memory on the heap
    int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));

    if (ptr == NULL) {
      Serial.println("Not enough heap.");
    } else {
      // Do something with the memory
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }

      // ✅ Free allocated memory to prevent heap leak
      vPortFree(ptr);
    }

    // Print out number of free heap memory bytes after malloc/free
    Serial.print("Heap after malloc/free (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Wait for a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memory Demo (Fixed)---");

  // Start the only other task
  xTaskCreatePinnedToCore(
    testTask,
    "Test Task",
    1500,   // ✅ matches GitHub, safe enough (~6 KB stack)
    NULL,
    1,
    NULL,
    app_cpu
  );

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
