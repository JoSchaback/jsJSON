#ifndef JS_JSON_H
#define JS_JSON_H

#include <stdbool.h> // bool
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc(), free()

/**
 * jsJSON node types
*/
enum jsJSON_TYPE {
    jsJSON_TYPE_BOOL,
    jsJSON_TYPE_NUMBER,
    jsJSON_TYPE_STRING,
    jsJSON_TYPE_ARRAY,
    jsJSON_TYPE_OBJECT
};

typedef struct _jsJSON jsJSON;

/**
 * jsJSON node structure
*/
struct _jsJSON {
    // type of the node
    enum jsJSON_TYPE type;

    // the key of the node, only relevant for object nodes
    // but not array
    char* key;

    // values
    bool boolValue;
    double numberValue;
    char* stringValue;

    // points towards linked list of sibblings
    jsJSON *sibblings;

    // points towards linked list of children nodes
    jsJSON *children;
};

/**
 * Own little version of strdup() because 
 * I want to avoid issues on Windows where strdup is flagged as
 * deprecated.
*/
char* jsJSON_strdup(char* src);

/**
 * Creates a new JSON node. Duplicates the key string.
*/
jsJSON* jsJSON_new(enum jsJSON_TYPE type, char *key);

/**
 * Recursively frees the JSON tree including all children and string values.
*/
jsJSON* jsJSON_free(jsJSON *root);

/**
 * Creates a new object JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newObject(char *key);

/**
 * Creates a new array JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newArray(char *key);

/**
 * Creates a new string JSON node. Duplicates the key and value string valuesm unless NULL.
*/
jsJSON* jsJSON_newString(char *key, char *value);

/**
 * Creates a new number JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newNumber(char *key, double value);

/**
 * Creates a new boolean JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newBool(char *key, bool value);

/**
 * Adds a child node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_add(jsJSON* parent, jsJSON* child);

/**
 * Adds a string node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addString(jsJSON* parent, char *key, char *value);

/**
 * Adds an object node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addObject(jsJSON* parent, char *key);

/**
 * Adds an array node to the parent node.
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addArray(jsJSON* parent, char *key);

/**
 * Adds a boolean node to the parent node.
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addBoolean(jsJSON* parent, char *key, bool value);

/**
 * Adds a number node to the parent node.
*/
jsJSON* jsJSON_addNumber(jsJSON* parent, char *key, double value);

/**
 * Serializes the JSON tree to a string buffer.
*/
size_t jsJSON_serializeToStr(jsJSON* root, char *buffer, size_t bufferSize);

/**
 * Parses a JSON string and returns the root node of the tree. Allocates memory internally
 * for all nodes and strings so that the buffer can be savely discarded after parsing.
*/
jsJSON* jsJSON_parse(char *json);

#endif // JS_JSON_H