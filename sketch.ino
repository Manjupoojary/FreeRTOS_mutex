#include <Arduino.h>  
#include <Arduino_FreeRTOS.h>  
#include <task.h>  
#include <queue.h>  
#include <semphr.h>  // Required for mutex  

// Globals  
QueueHandle_t xQueue;  
SemaphoreHandle_t xSerialMutex;  // Mutex for Serial  

// Task 1: Blink LED  
void TaskBlink(void *pvParam) {  
  pinMode(13, OUTPUT);  
  while (1) {  
    digitalWrite(13, !digitalRead(13));  
    vTaskDelay(500 / portTICK_PERIOD_MS);  
  }  
}  

// Task 2: Print safely using mutex  
void TaskPrint(void *pvParam) {  
  int receivedValue = 0;  
  while (1) {  
    if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdTRUE) {  
      if (xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdTRUE) {  // Lock  
        Serial.print("Received: ");  
        Serial.println(receivedValue);  
        xSemaphoreGive(xSerialMutex);  // Unlock  
      }  
    }  
  }  
}  

// Task 3: Generate data  
void TaskSensor(void *pvParam) {  
  int counter = 0;  
  while (1) {  
    xQueueSend(xQueue, &counter, 0);  
    counter++;  
    vTaskDelay(1000 / portTICK_PERIOD_MS);  
  }  
}  

void setup() {  
  Serial.begin(9600);  
  xQueue = xQueueCreate(5, sizeof(int));  
  xSerialMutex = xSemaphoreCreateMutex();  // Create mutex  
  
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);  
  xTaskCreate(TaskPrint, "Print", 128, NULL, 2, NULL);  // Higher priority  
  xTaskCreate(TaskSensor, "Sensor", 128, NULL, 1, NULL);  
}  

void loop() {}  