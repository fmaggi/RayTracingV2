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
    void Init();
    static inline Application* app = nullptr;

    ApplicationSpecification spec;
    void* window = nullptr;
    uint32_t width=0, height=0;
};
