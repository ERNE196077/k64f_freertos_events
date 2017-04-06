/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define T0 (1 << 0)
#define T1 (1 << 1)
#define T2 (1 << 2)
#define T3 (1 << 3)
#define T4 (1 << 4)

#define TasksBits (T0 | T1 | T2 | T3 | T4)


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void write_task_1(void *pvParameters);
static void write_task_2(void *pvParameters);
static void write_task_3(void *pvParameters);
static void write_task_4(void *pvParameters);
static void write_task_5(void *pvParameters);
static void read_task(void *pvParameters);
/*******************************************************************************
 * Globals
 ******************************************************************************/
static EventGroupHandle_t event_group = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
	BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    event_group = xEventGroupCreate();
    xTaskCreate(write_task_1, "WRITE_TASK_1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(write_task_2, "WRITE_TASK_2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(write_task_3, "WRITE_TASK_3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(write_task_4, "WRITE_TASK_4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(write_task_5, "WRITE_TASK_5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(read_task, "READ_TASK", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    /* Start scheduling. */
    PRINTF("####################################\n\r");
    PRINTF("####################################\n\r");
    PRINTF("####################################\n\r");
    PRINTF("####################################\n\r");
    PRINTF("####################################\n\r");

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief write_task_1 function
 */
static void write_task_1(void *pvParameters)
{
    while (1)
    {
        PRINTF("Task T0 Activated. Flag 0 set.\n\r");
        xEventGroupSetBits(event_group, T0);
        vTaskSuspend(NULL);
    }
}

/*!
 * @brief write_task_2 function
 */
static void write_task_2(void *pvParameters)
{
    while (1)
    {
        PRINTF("Task T1 Activated. Flag 1 set.\n\r");
        xEventGroupSetBits(event_group, T1);
        vTaskSuspend(NULL);

    }
}

/*!
 * @brief write_task_2 function
 */
static void write_task_3(void *pvParameters)
{
    while (1)
    {
        PRINTF("Task T2 Activated. Flag 2 set.\n\r");
        xEventGroupSetBits(event_group, T2);
        vTaskSuspend(NULL);
    }
}
/*!
 * @brief write_task_2 function
 */
static void write_task_4(void *pvParameters)
{
    while (1)
    {
        PRINTF("Task T3 Activated. Flag 3 set.\n\r");
        xEventGroupSetBits(event_group, T3);
        vTaskSuspend(NULL);
    }
}
/*!
 * @brief write_task_2 function
 */
static void write_task_5(void *pvParameters)
{
    while (1)
    {
        PRINTF("Task T4 Activated. Flag 4 set.\n\r");
        xEventGroupSetBits(event_group, T4);
        vTaskSuspend(NULL);
    }
}

static void read_task(void *pvParameters)
{
    EventBits_t event_bits;
    while (1)
    {
        event_bits = xEventGroupWaitBits(event_group,    /* The event group handle. */
                                         TasksBits,        /* The bit pattern the event group is waiting for. */

										 /* Uncomment to OR condition */
										 pdTRUE,         /* BIT_0 and BIT_4 will be cleared automatically. */
										 pdFALSE,        /* No wait for ALL bits, either bit unblock task. */

										 /* Uncomment to AND condition*/
                                         //pdFALSE,        /* BIT_0 and BIT_4 will NOT be cleared. */
                                         //pdTRUE,        /* WAIT for ALL bits, either bit unblock task. */
                                         
                                         portMAX_DELAY); /* Block indefinitely to wait for the condition to be met. */

        PRINTF("\n\n\rEVENT BIT CONDITION\n\n\r");
        if ((event_bits & (TasksBits)) == (TasksBits))
        {
            PRINTF("All tasks activated\n\r");
        }
        else 
        {
            PRINTF("At least one task \n\r");
        }
    }
}
