# Learn_Code_Assignments

g++ -o ShoppingClient ../src/*.cpp main.cpp -I../inc/  -lcurl -lpthread

g++ -o ShoppingServer ../src/*.cpp main.cpp -I../inc -I../lib -lmysqlclient -lpthread