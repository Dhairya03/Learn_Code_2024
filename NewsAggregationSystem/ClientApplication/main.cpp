// #include "MenuHandler.h"
// #include "Client.h"

// int main() {
//     Client client("http://localhost:8080");
//     MenuHandler menu(client);
//     menu.run();
//     return 0;
// }

// Entry point for the Client Application
#include "Core/inc/Client.h"
#include "Core/inc/Session.h"
#include "Menu/inc/HomeMenu.h"

int main() {
    Client client("http://localhost:8080");
    Session session;
    HomeMenu homeMenu(client, session);
    homeMenu.display();
    return 0;
}
