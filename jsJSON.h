#ifndef JS_JSON_H
#define JS_JSON_H

#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()

enum jsJSON_TYPE {
    jsJSON_TYPE_BOOL,
    jsJSON_TYPE_NUMBER,
    jsJSON_TYPE_STRING,
    jsJSON_TYPE_ARRAY,
    jsJSON_TYPE_OBJECT
};
typedef struct _jsJSON jsJSON;
struct _jsJSON {
    // type
    enum jsJSON_TYPE type;
    char* key;
    // value
    bool boolValue;
    double numberValue;
    char* stringValue;

    jsJSON *sibblings;

    // points towards linked list of children nodes (sibblings)
    jsJSON *children;
};

jsJSON* jsJSON_new(enum jsJSON_TYPE type, char *key);
jsJSON* jsJSON_newObject(char *key);
jsJSON* jsJSON_newArray(char *key);
jsJSON* jsJSON_newString(char *key, char *value);
jsJSON* jsJSON_newNumber(char *key, double value);
jsJSON* jsJSON_newBool(char *key, bool value);

jsJSON* jsJSON_add(jsJSON* parent, jsJSON* child);
jsJSON* jsJSON_addString(jsJSON* parent, char *key, char *value);
jsJSON* jsJSON_addObject(jsJSON* parent, char *key);
jsJSON* jsJSON_addArray(jsJSON* parent, char *key);
jsJSON* jsJSON_addBoolean(jsJSON* parent, char *key, bool value);
jsJSON* jsJSON_addNumber(jsJSON* parent, char *key, double value);

size_t jsJSON_serializeToStr(jsJSON* root, char *buffer, size_t bufferSize);
jsJSON* jsJSON_parse(char *json);


#endif // JS_JSON_H