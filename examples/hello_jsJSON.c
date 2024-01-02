
#include "../jsJSON.h" 
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()
int main() {
    // first we create a root JSON node. Since this is the root
    // node, there is no parent and thus no key necessary. That's
    // why we pass NULL as the key.
    jsJSON* root = jsJSON_newObject(NULL);

    // Let's add to children nodes to the root with values and 
    // on array node.
    jsJSON_addString(root, "message", "Hello darkness my old friend!");
    jsJSON_addNumber(root, "some number", 0.003242);
    jsJSON* arr = jsJSON_addArray(root, "sub messages");

    // we add three string nodes to the array node. Note again that
    // we dont need to provide keys, hence the NULL parameters
    jsJSON_addString(arr, NULL, "this is m1");
    jsJSON_addString(arr, NULL, "this is m2asdasdaa");
    jsJSON_addString(arr, NULL, "this is m3");

    // serialize JSON tree to a string buffer
    char buffer[1000];
    jsJSON_serializeToStr(root, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    return 0;
}

