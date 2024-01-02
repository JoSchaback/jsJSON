#include "../jsJSON.h" 
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()

int main() {

    typedef struct Message {
        char* type;
        char* timestamp;
        char* sender;
        char* field1;
        char* field2;
    } Message;

    // let's take another silly JSON example string and
    // print it first on the console
    char* json = "{\"type\":\"chatter message\",\"timestamp\":\"2023-12-12 22:22:22\",\"sender\":\"Alice\",\"payload\": {\"field1\":\"value1\",\"field2\":\"value2\"}}";
    printf("%s\n", json);   
    jsJSON* root = jsJSON_parse(json);

    Message msg;
    msg.type      = jsJSON_getString(root, "type");
    msg.timestamp = jsJSON_getString(root, "timestamp");
    msg.sender    = jsJSON_getString(root, "sender");
    msg.field1    = jsJSON_getString(jsJSON_getObject(root, "payload"), "field1");
    msg.field1    = jsJSON_getString(jsJSON_getObject(root, "payload"), "field2");
    // here is the magic of parsing the JSON string into a tree

    jsJSON* newJSON = jsJSON_newObject(NULL);
    jsJSON_addString(newJSON, "type", msg.type);
    jsJSON_addString(newJSON, "timestamp", msg.timestamp);
    jsJSON_addString(newJSON, "sender", msg.sender);
    jsJSON* payload = jsJSON_addObject(newJSON, "payload");
    jsJSON_addString(payload, "field1", msg.field1);
    jsJSON_addString(payload, "field2", msg.field2);

    // now we serialize the tree back to a string buffer
    // and print it on the console
    char buffer[1000];
    jsJSON_serializeToStr(newJSON, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    jsJSON_free(root);
    jsJSON_free(newJSON);

    return 0;
}
