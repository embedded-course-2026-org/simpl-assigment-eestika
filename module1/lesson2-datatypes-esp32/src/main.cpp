/**
 * Lesson 2: Data Types in C
 * ESP32-C3 Demonstration Project
 *
 * Homework idea:
 * - Print full statistics periodically (e.g., every 10 counts)
 * - Counter increments every 2 seconds
 */

#include <Arduino.h>
#include <stdint.h>

#define LED_PIN 8

// --- Tuning ---
static const uint32_t LOOP_PERIOD_MS = 2000;   // every 2 seconds
static const uint8_t  REPORT_EVERY_N = 10;     // full report every 10 counts

// Forward declarations (good habit in embedded C/C++)
static void printSizeofReport(void);
static void printOverflowReport(void);
static void printCastingReport(void);
static void printFullReport(uint8_t count);

// -------------------------------------------
// setup()
// -------------------------------------------
void setup() {
  Serial.begin(115200);

  // Give time to open Serial Monitor after reset / upload
  delay(10000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("\r\n=== ESP32-C3 Data Types Demo (Periodic) ===\r\n");
  Serial.printf("Loop period: %lu ms, Full report every %u counts\r\n\r\n",
                (unsigned long)LOOP_PERIOD_MS, (unsigned)REPORT_EVERY_N);

  // Optional: print one full report at startup as a baseline
  printFullReport(0);
}

// -------------------------------------------
// loop()
// -------------------------------------------
void loop() {
  // Blink LED shortly each cycle (non-blocking blink would be even nicer,
  // but for this homework we keep it simple)
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);

  // Counter increments every 2 seconds
  static uint8_t count = 0;
  count++;

  uint32_t uptime_ms = millis();
  Serial.printf("Count: %u, Uptime: %lu ms\r\n",
                (unsigned)count, (unsigned long)uptime_ms);

  // Print full report every N counts (10, 20, 30, ...)
  if ((count % REPORT_EVERY_N) == 0) {
    printFullReport(count);
  }

  delay(LOOP_PERIOD_MS);
}

// -------------------------------------------
// Report functions
// -------------------------------------------
static void printFullReport(uint8_t count) {
  Serial.printf("\r\n===== FULL REPORT @ Count %u =====\r\n", (unsigned)count);

  printSizeofReport();
  printOverflowReport();
  printCastingReport();

  Serial.println("===== END REPORT =====\r\n");
}

static void printSizeofReport(void) {
  Serial.println("--- sizeof() Demonstration ---");
  // sizeof returns size_t (often unsigned), print as unsigned
  Serial.printf("uint8_t:  %u byte\r\n",  (unsigned)sizeof(uint8_t));
  Serial.printf("uint16_t: %u bytes\r\n", (unsigned)sizeof(uint16_t));
  Serial.printf("uint32_t: %u bytes\r\n", (unsigned)sizeof(uint32_t));
  Serial.printf("int8_t:   %u byte\r\n",  (unsigned)sizeof(int8_t));
  Serial.printf("int16_t:  %u bytes\r\n", (unsigned)sizeof(int16_t));
  Serial.printf("int32_t:  %u bytes\r\n", (unsigned)sizeof(int32_t));
  Serial.printf("float:    %u bytes\r\n", (unsigned)sizeof(float));
  Serial.printf("double:   %u bytes\r\n", (unsigned)sizeof(double));
  Serial.println();
}

static void printOverflowReport(void) {
  Serial.println("--- Overflow Demonstration ---");
  Serial.println("uint8_t overflow (unsigned):");

  uint8_t counter = 253;
  for (int i = 0; i < 5; i++) {
    Serial.printf("counter = %u\r\n", (unsigned)counter);
    counter++;
  }

  Serial.println("Notice: 255 -> 0 (wraps around!)\r\n");
}

static void printCastingReport(void) {
  Serial.println("--- Type Casting Demonstration ---");

  // Example 1: Data loss when downsizing
  uint16_t big = 1000;
  uint8_t small = big; // implicit cast (data loss)
  Serial.printf("big (uint16_t)   = %u\r\n", (unsigned)big);
  Serial.printf("small (uint8_t)  = %u (data loss!)\r\n", (unsigned)small);
  Serial.printf("Explanation: 1000 = 0x%04X, lower byte = 0x%02X = %u\r\n\r\n",
                (unsigned)big, (unsigned)small, (unsigned)small);

  // Example 2: Negative to unsigned casting
  int16_t temp = -10;
  uint16_t positive = (uint16_t)temp;
  Serial.printf("temp (int16_t)     = %d\r\n", (int)temp);
  Serial.printf("positive (uint16_t)= %u (becomes huge!)\r\n\r\n",
                (unsigned)positive);
}
