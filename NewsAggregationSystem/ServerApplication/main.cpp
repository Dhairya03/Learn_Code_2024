#include "lib/crow/crow_all.h"
#include "controllers/inc/UserController.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/signup").methods("POST"_method)(UserController::signup);

    std::cout << "Server running on port 8080...\n";
    app.port(8080).multithreaded().run();
}
