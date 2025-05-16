# FreeRTOS_mutex

## How It Works  
1. `TaskSensor` generates data and sends it via a queue.  
2. `TaskPrint` uses a mutex to **safely** print data to Serial.  
3. `TaskBlink` runs independently (no shared resources).  

## Output Example  
Received: 0 (Mutex ensures this line isn’t interrupted)
Received: 1

*simultion
https://wokwi.com/projects/430939732765936641
