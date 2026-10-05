#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TASK_PRIORITY       1

void task_hello_world(void *pvParameters) {
    while (1) {
        puts("Hello world!");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_tasks_init(void) {

    xTaskCreate(
        task_hello_world,
        "task_hello_world",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    puts("RTOS: All tasks created successfully!");
}

extern "C" void app_main(void) {
    // Инициализация задач
    app_tasks_init();
}
