
#include "../jsJSON.h" 
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()
int main() {

    jsJSON* root = jsJSON_newObject("root");
    jsJSON_addString(root, "message", "hellosssss!");
    jsJSON_addNumber(root, "some number", 0.003242);
    jsJSON* arr = jsJSON_addArray(root, "sub messages");
    jsJSON_addString(arr, NULL, "this is m1");
    jsJSON_addString(arr, NULL, "this is m2asdasdaa");
    jsJSON_addString(arr, NULL, "this is m3");

    char buffer[1000];
    jsJSON_serializeToStr(root, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    return 0;
}

