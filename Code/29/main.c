#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "element.h"
#include "visitor.h"

extern int printf(const char *format, ...);

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

static void visit_all(element_t **elements, int count, visitor_t *visitor)
{
    int i;

    if (elements == 0 || visitor == 0 || count <= 0) {
        return;
    }

    for (i = 0; i < count; ++i)
    {
        if (elements[i] != 0 && elements[i]->accept != 0) {
            elements[i]->accept(elements[i], visitor);
        }
    }
}

int main(void)
{
    temp_sensor_t temp;
    pressure_sensor_t pressure;
    motor_t motor;
    info_visitor_t info;
    selftest_visitor_t selftest;
    element_t *elements[3];

    init_console_utf8();

    /* 固定元素集合：TempSensor(25)、PressureSensor(100)、Motor(1500)。 */
    temp_sensor_init(&temp, 25);
    pressure_sensor_init(&pressure, 100);
    motor_init(&motor, 1500);

    elements[0] = &temp.base;
    elements[1] = &pressure.base;
    elements[2] = &motor.base;

    info_visitor_init(&info);
    selftest_visitor_init(&selftest);

    printf("========================================\n");
    printf("[场景1] 使用 InfoVisitor 遍历所有元素\n");
    visit_all(elements, 3, &info.base);

    printf("========================================\n");
    printf("[场景2] 使用 SelfTestVisitor 遍历所有元素\n");
    visit_all(elements, 3, &selftest.base);

    printf("========================================\n");
    return 0;
}
