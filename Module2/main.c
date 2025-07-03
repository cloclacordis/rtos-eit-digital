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
 * Date:   July 3, 2025
 *
 * This file is a minimal POSIX-based FreeRTOS Dynamic Priority Manager
 * for the "Development of Real-Time Systems" course (EIT Digital).
 *
 * Implements:
 *  - matrix_task: heavy CPU load
 *  - communication_task: I/O, 200 ms period
 *  - priority_set_task: dynamic priority adjustment via TickHook
 *
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Task handles */
TaskHandle_t matrix_handle        = NULL;
TaskHandle_t communication_handle = NULL;

/* For measuring communication_task execution time (in ticks) */
volatile TickType_t comm_start_tick    = 0;
volatile TickType_t comm_last_duration = 0;
volatile bool        comm_measuring    = false;

/* ========================= matrix_task ========================= */
#define SIZE 10
#define ROW SIZE
#define COL SIZE
static void matrix_task() {
    int i;

    /* Allocate matrices a, b, c */
    double **a = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) a[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **b = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) b[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **c = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) c[i] = (double *)pvPortMalloc(COL * sizeof(double));
    
    double sum = 0.0;
    int j, k, l;

    /* Initialize a and b */
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            a[i][j] = 1.5;
            b[i][j] = 2.6;
        }
    }

    while (1) {
        /* Dummy delay for PC simulator */
        long simulationdelay;
        for (simulationdelay = 0; simulationdelay < 1000000000; simulationdelay++)
            ;

        /* Zero matrix c */
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                c[i][j] = 0.0;
            }
        }

        /* Matrix multiplication */
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                sum = 0.0;
                for (k = 0; k < SIZE; k++) {
                    for (l = 0; l < 10; l++) {
                        sum = sum + a[i][k] * b[k][j];
                    }
                }
                c[i][j] = sum;
            }
        }
        vTaskDelay(100);
    }
}

/* ====================== communication_task ===================== */
static void communication_task(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        printf("Sending data...\n");
        fflush(stdout);
        vTaskDelay(100);
        printf("Data sent!\n");
        fflush(stdout);
        vTaskDelay(100);
    }
}

/* ===================== priority_set_task ======================= */
static void priority_set_task(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        TickType_t dur = comm_last_duration;
        if (dur > 1000) {
            /* execution > 1000 ms -> raise priority */
            vTaskPrioritySet(communication_handle, 4);
        } else if (dur < 200) {
            /* execution < 200 ms -> lower priority */
            vTaskPrioritySet(communication_handle, 2);
        }
        vTaskDelay(100);
    }
}

/* ========================= main() ============================== */
int main(void) {
    xTaskCreate((pdTASK_CODE)matrix_task,        (const char *)"Matrix",       1000,                     NULL, 3, &matrix_handle);
    xTaskCreate((pdTASK_CODE)communication_task, (const char *)"Communication", configMINIMAL_STACK_SIZE, NULL, 1, &communication_handle);
    xTaskCreate((pdTASK_CODE)priority_set_task,  (const char *)"PrioSetter",   configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    vTaskStartScheduler();
    /* Should never reach here */
    printf("Scheduler failed.\n");
    return 0;
}

/* ========================= Hook functions ========================= */
void vApplicationMallocFailedHook(void)          { for(;;); }
void vApplicationIdleHook(void)                  { /* nothing */ }
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) { (void)pxTask; (void)pcTaskName; for(;;); }
void vAssertCalled(const char *pcFile, unsigned long ulLine)            { (void)pcFile; (void)ulLine; for(;;); }
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTCBBuffer, StackType_t **ppxIdleStackBuffer, configSTACK_DEPTH_TYPE *pulIdleStackSize) {
    static StaticTask_t xIdleTCB; static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
    *ppxIdleTCBBuffer = &xIdleTCB; *ppxIdleStackBuffer = xIdleStack; *pulIdleStackSize = configMINIMAL_STACK_SIZE;
}
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTCBBuffer, StackType_t **ppxTimerStackBuffer, configSTACK_DEPTH_TYPE *pulTimerStackSize) {
    static StaticTask_t xTimerTCB; static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
    *ppxTimerTCBBuffer = &xTimerTCB; *ppxTimerStackBuffer = xTimerStack; *pulTimerStackSize = configTIMER_TASK_STACK_DEPTH;
}
void vApplicationDaemonTaskStartupHook(void)     { /* nothing */ }

