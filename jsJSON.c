#include "jsJSON.h"
#include <stdio.h>
#include <stdlib.h> // malloc(), free()
#include <string.h> // strcmp()
#include <stdbool.h> // bool
#include <stdint.h> // uint64_t


jsJSON* jsJSON_new(enum jsJSON_TYPE type, char *key) {
    jsJSON* json = malloc(sizeof(jsJSON));
    json->type = type;
    json->boolValue = false;
    json->key = key;
    json->numberValue = 0;
    json->stringValue = NULL;
    json->children = NULL;
    json->sibblings = NULL;
    return json;
}

jsJSON* jsJSON_newObject(char *key) {
    return jsJSON_new(jsJSON_TYPE_OBJECT, key);
}

jsJSON* jsJSON_newArray(char *key) {
    return jsJSON_new(jsJSON_TYPE_ARRAY, key);
}

jsJSON* jsJSON_newString(char *key, char *value) {
    jsJSON* n = jsJSON_new(jsJSON_TYPE_STRING, key);
    n->stringValue = value;
    return n;
}

jsJSON* jsJSON_newNumber(char *key, double value) {
    jsJSON* n = jsJSON_new(jsJSON_TYPE_NUMBER, key);
    n->numberValue = value;
    return n;
}

jsJSON* jsJSON_newBool(char *key, bool value) {
    jsJSON* n = jsJSON_new(jsJSON_TYPE_BOOL, key);
    n->boolValue = value;
    return n;
}

jsJSON* jsJSON_add(jsJSON* parent, jsJSON* child) {

    // children are stored in a linked list
    // thus, if the parent has no children yet, this child
    // iniatiates the linked list
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        // otherwise, we need to find the last child in the
        // linked list and append the new child to it
        jsJSON* last = parent->children;
        while (last->sibblings != NULL) {
            last = last->sibblings;
        }
        last->sibblings = child;
    }
    return child;
}

jsJSON* jsJSON_addString(jsJSON* parent, char *key, char *value) {
    jsJSON* json = jsJSON_newString(key, value);
    return jsJSON_add(parent, json);
}

jsJSON* jsJSON_addObject(jsJSON* parent, char *key) {
    return jsJSON_add(parent, jsJSON_newObject(key));
}

jsJSON* jsJSON_addArray(jsJSON* parent, char *key) {
    return jsJSON_add(parent, jsJSON_newArray(key));
}

jsJSON* jsJSON_addBoolean(jsJSON* parent, char *key, bool value) {
    return jsJSON_add(parent, jsJSON_newBool(key, value));
}

jsJSON* jsJSON_addNumber(jsJSON* parent, char *key, double value) {
    return jsJSON_add(parent, jsJSON_newNumber(key, value));
}

void write(char* buffer, char* str, size_t bufferSize, uint64_t *bytesWritten) {
    //printf("write: [%s], bufferSize: %llu, bytesWritten: %llu\n", str, bufferSize - *bytesWritten, *bytesWritten);
    int count = snprintf((char*)((size_t)buffer + (size_t)(*bytesWritten)), bufferSize - *bytesWritten, "%s", str);
    *bytesWritten += count;
}

static void jsJSON_serializeToStrRecursive(jsJSON* root, char *buffer, size_t bufferSize, uint64_t *bytesWritten) {
    if (root->type == jsJSON_TYPE_OBJECT) {
        write(buffer, "{", bufferSize, bytesWritten);
        jsJSON* child = root->children;
        while( child != NULL ) {
            write(buffer, "\"", bufferSize, bytesWritten);
            write(buffer, child->key, bufferSize, bytesWritten);
            write(buffer, "\": ", bufferSize, bytesWritten);
            jsJSON_serializeToStrRecursive(child, buffer, bufferSize, bytesWritten);
            if( child->sibblings != NULL ) {
                write(buffer, ", ", bufferSize, bytesWritten);
            }
            child = child->sibblings;
        }
        write(buffer, "}", bufferSize, bytesWritten);
    } else if (root->type == jsJSON_TYPE_ARRAY) {
        write(buffer, "[", bufferSize, bytesWritten);
        jsJSON* child = root->children;
        while( child != NULL ) {
            jsJSON_serializeToStrRecursive(child, buffer, bufferSize, bytesWritten);
            if( child->sibblings != NULL ) {
                write(buffer, ", ", bufferSize, bytesWritten);
            }
            child = child->sibblings;
        }
        write(buffer, "]", bufferSize, bytesWritten);
    } else if (root->type == jsJSON_TYPE_STRING) {
        int count = snprintf(
            (char*)((size_t)buffer + (size_t)(*bytesWritten)), 
            bufferSize - *bytesWritten, 
            "\"%s\"", root->stringValue);
        *bytesWritten += count;
    } else if (root->type == jsJSON_TYPE_NUMBER) {
        int count = snprintf(
            (char*)((size_t)buffer + (size_t)(*bytesWritten)), 
            bufferSize - *bytesWritten, 
            "%f", root->numberValue);
        *bytesWritten += count;
    } else if (root->type == jsJSON_TYPE_BOOL) {
        if (root->boolValue) {
            write(buffer, "true", bufferSize, bytesWritten);
        } else {
            write(buffer, "false", bufferSize, bytesWritten);
        }
    }
}

uint64_t jsJSON_serializeToStr(jsJSON* root, char *buffer, size_t bufferSize) {
    uint64_t bytesWritten = 0;

    jsJSON_serializeToStrRecursive(root, buffer, bufferSize, &bytesWritten);

    write(buffer, "\0", bufferSize, &bytesWritten);
    return bytesWritten;
}