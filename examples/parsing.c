
#include "../jsJSON.h" 
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()

int main() {

    char* json = "{\"message\":\"hellosssss!\",\"some number\":0.003242,\"sub messages\":[\"this is m1\",\"this is m2asdasdaa\",\"this is m3\"]}";
    printf("%s\n", json);   
    jsJSON* root = jsJSON_parse(json);
    char buffer[1000];
    jsJSON_serializeToStr(root, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    return 0;
}

