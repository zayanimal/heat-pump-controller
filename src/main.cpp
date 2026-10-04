#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// === КОНФИГУРАЦИЯ ЗАДАЧ ===
#define TASK_SERIAL_PRIORITY    1   // Низкий приоритет — не критично
#define TASK_LED_PRIORITY       2   // Средний приоритет
#define TASK_MONITOR_PRIORITY   3   // Высокий приоритет
#define TASK_INTERRUPT_PRIORITY 4   // Высший приоритет

// === ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ===
volatile bool led_state = false;

// === ЗАДАЧА 1: Отправка в Serial (низкий приоритет) ===
void task_serial(void *pvParameters) {
    for (;;) {
        Serial.println("RTOS: Task Serial — LED ON");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// === ЗАДАЧА 2: Управление LED (средний приоритет) ===
void task_led(void *pvParameters) {
    for (;;) {
        led_state = !led_state;  // Инвертируем состояние

        if (led_state) {
            Serial.println("RTOS: Task LED — ON");
        } else {
            Serial.println("RTOS: Task LED — OFF");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// === ЗАДАЧА 3: Мониторинг (высокий приоритет) ===
void task_monitor(void *pvParameters) {
    for (;;) {
        Serial.print("RTOS: Task Monitor — LED State: ");
        Serial.println(led_state ? "ON" : "OFF");

        // Здесь можно добавить логику:
        // - проверка состояния датчиков
        // - обработка прерываний
        // - отправка данных на сервер

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// === ЗАДАЧА 4: Обработка прерываний (высший приоритет) ===
void task_interrupt_handler(void *pvParameters) {
    for (;;) {
        // Здесь обрабатываются внешние прерывания (GPIO, таймеры и т.д.)
        // Например: кнопка сброса, датчик движения, приём данных

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// === ЗАПУСК ВСЕХ ЗАДАЧ ===
void app_tasks_init(void) {
    // Задача Serial — низкий приоритет, 1000 мс интервал
    xTaskCreate(
        task_serial,
        "task_serial",
        4096,
        NULL,
        TASK_SERIAL_PRIORITY,
        NULL
    );

    // Задача LED — средний приоритет, 1000 мс интервал
    xTaskCreate(
        task_led,
        "task_led",
        4096,
        NULL,
        TASK_LED_PRIORITY,
        NULL
    );

    // Задача Monitor — высокий приоритет, 100 мс интервал
    xTaskCreate(
        task_monitor,
        "task_monitor",
        4096,
        NULL,
        TASK_MONITOR_PRIORITY,
        NULL
    );

    // Задача Interrupt Handler — высший приоритет, 10 мс интервал
    xTaskCreate(
        task_interrupt_handler,
        "task_interrupt",
        4096,
        NULL,
        TASK_INTERRUPT_PRIORITY,
        NULL
    );

    Serial.println("RTOS: All tasks created successfully!");
}

void setup() {
    Serial.begin(115200);
    delay(100);  // Небольшая задержка для стабильности Serial

    Serial.println("=== ESP32 RTOS System Started ===");
    app_tasks_init();

    // Задержка для инициализации задач перед выводом логов
    delay(500);

    Serial.println("=== Tasks Running ===");
}

void loop() {
    // В RTOS-подходе main loop — это точка входа.
    // Основная логика перенесена в задачи (tasks).
}
