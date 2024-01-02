
#include "../jsJSON.h" 
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()

int main() {
    // let's take another silly JSON example string and
    // print it first on the console
    char* json = "{\"message\":\"Hello!\",\"some number\":0.003242,\"sub messages\":[\"this is m1\",\"this is another element\",\"this is m3\"]}";
    printf("%s\n", json);   

    // here is the magic of parsing the JSON string into a tree
    jsJSON* root = jsJSON_parse(json);

    // now we serialize the tree back to a string buffer
    // and print it on the console
    char buffer[1000];
    jsJSON_serializeToStr(root, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    // free the tree
    jsJSON_free(root);

    return 0;
}

