/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * Author: Timofei Alekseenko
 * Date:   June 24, 2025
 *
 * This file is a minimal POSIX-based FreeRTOS demo for
 * the "Development of Real-Time Systems" course (EIT Digital).
 * It defines two periodic tasks with different delays.
 *
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

/* Task1: prints every 100ms at priority 3 */
void Task1(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        printf("This is task 1\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* Task2: prints every 500ms at priority 1 */
void Task2(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        printf("This is task 2\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* main(): create tasks, start the FreeRTOS scheduler */
int main(void) {
    xTaskCreate(Task1, "Task1", 1000, NULL, 3, NULL);
    xTaskCreate(Task2, "Task2",  100, NULL, 1, NULL);
    vTaskStartScheduler();

    /* If vTaskStartScheduler() returns, there was insufficient heap */
    printf("Scheduler failed.\n");
    return 0;
}

/* Minimal stub implementations of required FreeRTOS hooks
(to satisfy linker; do nothing or infinite-loop on error) */
void vApplicationMallocFailedHook(void)          { for(;;); }
void vApplicationIdleHook(void)                  { /* nothing */ }
void vApplicationTickHook(void)                  { /* nothing */ }
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pxTask; (void)pcTaskName;
    for(;;);
}

void vAssertCalled(const char *pcFile, unsigned long ulLine)
{
    (void)pcFile; (void)ulLine;
    for(;;);
}

/* Provide memory for idle and timer tasks (static allocation) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTCBBuffer,
                                    StackType_t  **ppxIdleStackBuffer,
                                    configSTACK_DEPTH_TYPE *pulIdleStackSize )
{
    static StaticTask_t xIdleTCB;
    static StackType_t  xIdleStack[ configMINIMAL_STACK_SIZE ];

    *ppxIdleTCBBuffer   = &xIdleTCB;
    *ppxIdleStackBuffer =  xIdleStack;
    *pulIdleStackSize   = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTCBBuffer,
                                     StackType_t  **ppxTimerStackBuffer,
                                     configSTACK_DEPTH_TYPE *pulTimerStackSize )
{
    static StaticTask_t xTimerTCB;
    static StackType_t  xTimerStack[ configTIMER_TASK_STACK_DEPTH ];

    *ppxTimerTCBBuffer    = &xTimerTCB;
    *ppxTimerStackBuffer  =  xTimerStack;
    *pulTimerStackSize    = configTIMER_TASK_STACK_DEPTH;
}

/* Optional hook called when daemon (timer) task starts up */
void vApplicationDaemonTaskStartupHook(void) { /* nothing */ }

