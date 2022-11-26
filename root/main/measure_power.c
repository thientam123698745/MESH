#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mesh_app.h"
unsigned long ccount;
TaskStatus_t *pxTaskStatusArray;
volatile UBaseType_t uxArraySize, x;
uint32_t ulTotalRunTime, ulStatsAsPercentage;
char RunTime[1024];

void power(void *p)
{
    
    TickType_t xLastWakeTime;
    BaseType_t xWasDelayed;
    const TickType_t xFrequency = iDelay / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount ();
    
    

   
    while (true)
    {
        // uxArraySize = uxTaskGetNumberOfTasks();
        // pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

        // if( pxTaskStatusArray != NULL )
        // {
        //     uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );
        //     ulTotalRunTime /= 100UL;
        //     if( ulTotalRunTime > 0 )
        //     {
        //         for( x = 0; x < uxArraySize; x++ )
        //     {
                
        //         if(strncmp("array_loop", pxTaskStatusArray[ x ].pcTaskName, sizeof(*pxTaskStatusArray[ x ].pcTaskName)) == 0)
        //         {
        //             ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;
        //             break;
        //         }
        //     }
        //     }

        //         // The array is no longer needed, free the memory it consumes.
        // }
        // pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );
        vTaskGetRunTimeStats(RunTime);
        // printf("%s\n", RunTime);  
        // printf("Chu ky: ");
        // printf("%d\n",(int)ccount);
        // BENCHMARK_START();
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        // vTaskDelay(iDelay / portTICK_RATE_MS);
        // BENCHMARK_END(&ccount); 
    }
        vPortFree( pxTaskStatusArray );
}
void power2(void *p)
{
    while(true)
    {
        // printf("power2");
        TickType_t xLastWakeTime;
        const TickType_t xFrequency = iDelay / portTICK_PERIOD_MS;
        BaseType_t xWasDelayed;
        xLastWakeTime = xTaskGetTickCount ();
        char demo[1024] = {0};
        double sensor = ccount * 0.08 * 3.3 / 160000000 *100 / iDelay;
        // printf("%f\n", sensor);
        // printf("Process: %d\n", start_loop);
        // printf("CPU_Usage: %d\n", atoi(RunTime));
        msg_create_power(demo, 1024,  atoi(RunTime));
        mqtt_app_publish("tree/root/powermesh", demo);
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}
void mesure_power_init(void)
{
    xTaskCreatePinnedToCore(power, "power", 8192, NULL, 7, NULL, 1);
    xTaskCreatePinnedToCore(power2, "power2", 8192, NULL, 6, NULL, 0);
}