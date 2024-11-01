#include "../include/main.hpp"

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    Object* obj = new Object(av);
    obj->renderer();
    return 0;
}
