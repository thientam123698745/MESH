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
        // printf("power1");
        BENCHMARK_START();
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        // vTaskDelay(iDelay / portTICK_RATE_MS);
        BENCHMARK_END(&ccount); 
    }
}
void power2(void *p)
{
    TickType_t xLastWakeTime;
    BaseType_t xWasDelayed;
    const TickType_t xFrequency = iDelay / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount ();

    mesh_data_t data;
    char demo[1024] = {0};

    data.proto = MESH_PROTO_BIN;
    data.tos = MESH_TOS_P2P;
    // const mesh_opt_t opt = {
    //     .type = MESH_OPT_SEND_GROUP,
    // };
    data.size = sizeof(demo);
    data.data = (uint8_t *)demo;
    while (true)
    {
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        double sensor = ccount * 0.08 * 3.3 / 160000000 * 100 /iDelay;
        // ESP_LOGI("MESH_POWER", "power=%02f", sensor);
        data.tos = 0;
        msg_create_power(demo, 1024, sensor);
        if (my_layer == 3)
        {
            mesh_addr_t parent;
            esp_err_t err = esp_mesh_get_parent_bssid(&parent);
            if (!err)
            {
                parent.addr[5] = parent.addr[5] - 1;
                // printf("child: " MACSTR "\n", MAC2STR(device_node.mac));
                esp_err_t err = esp_mesh_send(&parent, &data, MESH_DATA_P2P, NULL, 0);

                // printf("%s\n", (char *)data.data);

                if (err != ESP_OK)
                {
                    ESP_LOGE("POWER", "send err:0x%x, size:%d", err, data.size);
                    continue;
                }
            }
        }
        else
            esp_mesh_send(NULL, &data, MESH_DATA_TODS, NULL, 0);
    }

}
void mesure_power_init(void)
{
    xTaskCreatePinnedToCore(power, "power", 8192, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(power2, "power2", 8192, NULL, 0, NULL, 1);
}