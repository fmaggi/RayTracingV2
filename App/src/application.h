#pragma once

class Application {
public:
    static Application* Create();
    void run();
private:
    Application();
    void Init();
    static inline Application* app = nullptr;

    void* window = nullptr;
};
