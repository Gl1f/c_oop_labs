#include "Application.h"

// Вся работа программы находится в менеджере.
int main(int argumentCount, char* arguments[]) {
    Application application;
    return application.run(argumentCount, arguments);
}
