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
    const char* key;

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
char* jsJSON_strdup(const char* src);

/**
 * Creates a new JSON node. Duplicates the key string.
*/
jsJSON* jsJSON_new(enum jsJSON_TYPE type, const char *key);

/**
 * Recursively frees the JSON tree including all children and string values.
*/
void jsJSON_free(jsJSON *root);

/**
 * Creates a new object JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newObject(const char *key);

/**
 * Creates a new array JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newArray(const char *key);

/**
 * Creates a new string JSON node. Duplicates the key and value string valuesm unless NULL.
*/
jsJSON* jsJSON_newString(const char *key, const char *value);

/**
 * Creates a new number JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newNumber(const char *key, double value);

/**
 * Creates a new boolean JSON node. Duplicates the key string, unless NULL.
*/
jsJSON* jsJSON_newBool(const char *key, bool value);

/**
 * Adds a child node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_add(jsJSON* parent, jsJSON* child);

/**
 * Adds a string node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addString(jsJSON* parent, const char *key, const char *value);

/**
 * Adds an object node to the parent node. 
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addObject(jsJSON* parent, const char *key);

/**
 * Adds an array node to the parent node.
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addArray(jsJSON* parent, const char *key);

/**
 * Adds a boolean node to the parent node.
 * The child node is added to the end of the children list.
*/
jsJSON* jsJSON_addBoolean(jsJSON* parent, const char *key, bool value);

/**
 * Adds a number node to the parent node.
*/
jsJSON* jsJSON_addNumber(jsJSON* parent, const char *key, double value);

/**
 * Serializes the JSON tree to a string buffer.
*/
size_t jsJSON_serializeToStr(const jsJSON* root, char *buffer, size_t bufferSize);

/**
 * Parses a JSON string and returns the root node of the tree. Allocates memory internally
 * for all nodes and strings so that the buffer can be savely discarded after parsing.
*/
jsJSON* jsJSON_parse(const char *json);

/**
 * Returns the string value for the given key in the given object node. Returns a reference.
*/
char*  jsJSON_getString(const jsJSON* objectNode, const char* key);

/**
 * Returns the number value for the given key in the given object node.
*/
double jsJSON_getNumber(const jsJSON* objectNode, const char* key);

/**
 * Returns the boolean value for the given key in the given object node.
*/
bool   jsJSON_getBoolean(const jsJSON* objectNode, const char* key);

/**
 * Returns the child object node for the given key in the given object node.
*/
jsJSON* jsJSON_getObject(const jsJSON* objectNode, const char* key);

/**
 * Duplicates the given JSON tree with all children and string values.
*/
jsJSON* jsJSON_duplicate(const jsJSON* root);


#endif // JS_JSON_H