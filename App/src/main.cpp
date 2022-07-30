#include "application.h"

static ApplicationSpecification parseArgs(int argc, char** argv) {
    if (argc != 2) {
        std::printf("Usage:\n  raytrace <output file>\n");
        exit(-1);
    }

    ApplicationSpecification spec;
    spec.outputFile = std::string(argv[1]);
    return spec;
}

int main(int argc, char** argv) {
    ApplicationSpecification spec = parseArgs(argc, argv);

    auto app = Application::Create(spec);
    app->run();
    delete app;
}
