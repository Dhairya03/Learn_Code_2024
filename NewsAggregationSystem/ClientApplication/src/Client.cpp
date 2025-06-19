#include "Client.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void signupUser() {
    httplib::Client client("localhost", 8080);

    json body = {
        {"username", "dhairya"},
        {"email", "dhairya@gmail.com"},
        {"password", "password"}
    };

    auto res = client.Post("/signup", body.dump(), "application/json");

    if (res && res->status == 200) {
        std::cout << "Signup: " << res->body << std::endl;
    } else {
        std::cerr << "Signup failed\n";
    }
}
