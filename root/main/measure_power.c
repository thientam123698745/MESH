#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mesh_app.h"
unsigned long ccount;

void power(void *p)
{
    
    TickType_t xLastWakeTime;
    BaseType_t xWasDelayed;
    const TickType_t xFrequency = iDelay / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount ();
    while (true)
    {
        // printf("Chu ky: ");
        printf("%d\n",(int)ccount);
        BENCHMARK_START();
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        // vTaskDelay(iDelay / portTICK_RATE_MS);
        BENCHMARK_END(&ccount); 
    }
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
        msg_create_power(demo, 1024, sensor);
        mqtt_app_publish("tree/root/powermesh", demo);
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}
void mesure_power_init(void)
{
    xTaskCreatePinnedToCore(power, "power", 8192, NULL, 7, NULL, 0);
    xTaskCreatePinnedToCore(power2, "power2", 8192, NULL, 6, NULL, 1);
}