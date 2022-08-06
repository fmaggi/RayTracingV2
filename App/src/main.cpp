#include "application.h"

int main() {
    auto app = Application::Create();
    app->run();
    delete app;
}
