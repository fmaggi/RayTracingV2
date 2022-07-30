#pragma once

#include <string>

struct ApplicationSpecification {
    std::string outputFile;
    int nThreads;
};

class Application {
public:
    static Application* Create(ApplicationSpecification spec);
    void run();
private:
    Application(ApplicationSpecification spec);
    static inline Application* app = nullptr;

    bool running = false;
    ApplicationSpecification spec;
};
