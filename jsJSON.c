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
    if( key != NULL ) {
        json->key = jsJSON_strdup(key);
    } else {
        json->key = NULL;
    }
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
    n->stringValue = jsJSON_strdup(value);
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

void write(char* buffer, char* str, size_t bufferSize, size_t *bytesWritten) {
    //printf("write: [%s], bufferSize: %llu, bytesWritten: %llu\n", str, bufferSize - *bytesWritten, *bytesWritten);
    int count = snprintf((char*)((size_t)buffer + (size_t)(*bytesWritten)), bufferSize - *bytesWritten, "%s", str);
    *bytesWritten += count;
}

static void jsJSON_serializeToStrRecursive(jsJSON* root, char *buffer, size_t bufferSize, size_t *bytesWritten) {
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

size_t jsJSON_serializeToStr(jsJSON* root, char *buffer, size_t bufferSize) {
    size_t bytesWritten = 0;

    jsJSON_serializeToStrRecursive(root, buffer, bufferSize, &bytesWritten);

    write(buffer, "\0", bufferSize, &bytesWritten);
    return bytesWritten;
}

enum jsJSON_TokenType {
    jsJSON_TokenType_SINGLE_CHAR,
    jsJSON_TokenType_STRING,
    jsJSON_TokenType_NUMBER,
    jsJSON_TokenType_BOOLEAN,
    jsJSON_TokenType_NULL_VALUE
}; 

typedef struct jsJSON_Tokenizer {
    char* json;
    size_t index;
    size_t line;
    size_t column;
    size_t jsonLength;
    char token[500];
    //char* last;
    bool isEOF;
    enum jsJSON_TokenType tokenType;
} jsJSON_Tokenizer;

static jsJSON_Tokenizer jsJSON_Tokenizer_new(char* json) {
    jsJSON_Tokenizer tokenizer;
    tokenizer.json = json;
    tokenizer.index = 0;
    tokenizer.line = 1;
    tokenizer.column = 1;
    tokenizer.jsonLength = strlen(json);
    tokenizer.token[0] = '\0';
    //tokenizer.last = NULL;
    tokenizer.isEOF = false;
    tokenizer.tokenType = jsJSON_TokenType_NULL_VALUE;
    return tokenizer;
}

static void jsJSON_Tokenizer_next(jsJSON_Tokenizer* tokenizer) {
    //tokenizer->last  = tokenizer->token;
    tokenizer->token[0] = '\0';
    while( tokenizer->index < tokenizer->jsonLength ) {
        char c = tokenizer->json[tokenizer->index++];
        tokenizer->column++;
        if( c == '{' 
        || c == '}' 
        || c == '[' 
        || c == ']' 
        || c == ':' 
        || c == ',' ) {
            tokenizer->token[0] = c;
            tokenizer->token[1] = '\0';
            //cout << "found single char [" << token << "] " << index << endl;
            tokenizer->tokenType = jsJSON_TokenType_SINGLE_CHAR;
            return;
        } else if( c == '"' ) {
            int counter = 0;
            while( tokenizer->json[tokenizer->index] != '"' ) {
                tokenizer->token[counter++] = tokenizer->json[tokenizer->index++];
            }
            tokenizer->token[counter++] = '\0';
            tokenizer->index++; // jump over the last quote
            tokenizer->tokenType = jsJSON_TokenType_STRING;
            return;
        } else if( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) {
            if( c == '\n' ) {
                tokenizer->line++;
                tokenizer->column = 1;
            }
        } else if( c >= 48 && c <= 57 ) { // number
            int counter = 0;
            tokenizer->token[counter++] = c;
            while( (tokenizer->json[tokenizer->index] >= 48 
                 && tokenizer->json[tokenizer->index] <= 57) 
                 || tokenizer->json[tokenizer->index] == '.') {
                tokenizer->token[counter++] = tokenizer->json[tokenizer->index++];
            }
            tokenizer->token[counter++] = '\0';
            tokenizer->tokenType = jsJSON_TokenType_NUMBER;
            //cout << "found number [" << token << "] " << index << endl;
            return;
        } else if( c == 't' && strlen(tokenizer->json) > tokenizer->index + 3 ) { // boolean, true
            tokenizer->token[0] = 't';
            tokenizer->token[1] = 'r';
            tokenizer->token[2] = 'u';
            tokenizer->token[3] = 'e';
            tokenizer->token[4] = '\0';
            tokenizer->index += 3;
            tokenizer->tokenType = jsJSON_TokenType_BOOLEAN;
            //cout << "found true [" << token << "] " << index << endl;
            return;
        } else if( c == 'f' && strlen(tokenizer->json) > tokenizer->index + 4 ) { // boolean, false
            tokenizer->token[0] = 'f';
            tokenizer->token[1] = 'a';
            tokenizer->token[2] = 'l';
            tokenizer->token[3] = 's';
            tokenizer->token[4] = 'e';
            tokenizer->token[5] = '\0';
            tokenizer->index += 4;
            tokenizer->tokenType = jsJSON_TokenType_BOOLEAN;
            //cout << "found false [" << token << "] " << index << endl;
            return;
        }
    }
    tokenizer->isEOF = true;
}

static void jsJSON_Tokenizer_nextExpectChar(jsJSON_Tokenizer* tokenizer, char c) {
    jsJSON_Tokenizer_next(tokenizer);
    if( tokenizer->token[0] != c ) {
        printf("expected char [%c] but found [%s] at line %zu, column %zu\n", c, tokenizer->token, tokenizer->line, tokenizer->column);
        exit(1);
    }
}

static void jsJSON_Tokenizer_nextExpectTwoOptions(jsJSON_Tokenizer* tokenizer, char c1, char c2) {
    jsJSON_Tokenizer_next(tokenizer);
    if( tokenizer->token[0] != c1 && tokenizer->token[0] != c2 ) {
        printf("expected char [%c] or [%c] but found [%s] at line %zu, column %zu\n", c1, c2, tokenizer->token, tokenizer->line, tokenizer->column);
        exit(1);
    }
}

static void jsJSON_Tokenizer_expectType(jsJSON_Tokenizer* tokenizer, enum jsJSON_TokenType type) {
    if( tokenizer->tokenType != type ) {
        printf("expected token type [%d] but found [%s] at line %zu, column %zu\n", type, tokenizer->token, tokenizer->line, tokenizer->column);
        exit(1);
    }
}

char *jsJSON_strdup(char *src) {
    char *dst = malloc(strlen (src) + 1);  // Space for length plus nul
    if (dst == NULL) return NULL;          // No memory
    strcpy(dst, src);                      // Copy the characters
    return dst;                            // Return the new string
}

static jsJSON* jsJSON_parseArray(jsJSON_Tokenizer* tokenizer, char *key);

static jsJSON* jsJSON_parseObject(jsJSON_Tokenizer* tokenizer, char *key) {
    jsJSON* root = jsJSON_newObject(key);
    jsJSON_Tokenizer_next(tokenizer);
    //printf("jsJSON_parseObject(): going into while loop [%s]\n", tokenizer->token);
    while( tokenizer->token[0] != '}' ) {
        jsJSON_Tokenizer_expectType(tokenizer, jsJSON_TokenType_STRING);
        // we need to duplicate the string because the tokenizer
        // will overwrite the token on the next call to next()
        char* name = jsJSON_strdup(tokenizer->token);
        //printf("   name [%s]\n", name);
        jsJSON_Tokenizer_nextExpectChar(tokenizer, ':'); // jump over string to colon
        jsJSON_Tokenizer_next(tokenizer); // jump over colon
        //printf("jsJSON_parseObject(): matching against [%s] with key [%s]\n", tokenizer->token, name);
        if( tokenizer->token[0] == '{' ) {
            jsJSON* child = jsJSON_parseObject(tokenizer, name);
            jsJSON_add(root, child);
        } else if( tokenizer->token[0] == '[' ) {
            jsJSON* child = jsJSON_parseArray(tokenizer, name);
            jsJSON_add(root, child);
        } else if( tokenizer->tokenType == jsJSON_TokenType_STRING ) {
            jsJSON_addString(root, name, tokenizer->token);
        } else if( tokenizer->tokenType == jsJSON_TokenType_NUMBER ) {
            jsJSON_addNumber(root, name, atof(tokenizer->token));
        } else if( tokenizer->tokenType == jsJSON_TokenType_BOOLEAN ) {
            jsJSON_addBoolean(root, name, tokenizer->token[0] == 't');
        } else {
            printf("Error: unexpected token [%s]\n", tokenizer->token);
            exit(1);
        }
        // we dont need name anymore and thus can free it now
        free(name);
        
        jsJSON_Tokenizer_nextExpectTwoOptions(tokenizer, ',', '}');
        if( tokenizer->token[0] == ',' ) {
            jsJSON_Tokenizer_next(tokenizer);
        }
    }
    return root;
}

static jsJSON* jsJSON_parseArray(jsJSON_Tokenizer* tokenizer, char *key) {
    jsJSON* root = jsJSON_newArray(key);
    jsJSON_Tokenizer_next(tokenizer);
    while( tokenizer->token[0] != ']' ) {
        //printf("jsJSON_parseArray(): matching against [%s]\n", tokenizer->token);
        if( tokenizer->token[0] == '{' ) {
            jsJSON* child = jsJSON_parseObject(tokenizer, NULL);
            jsJSON_add(root, child);
        } else if( tokenizer->token[0] == '[' ) {
            jsJSON* child = jsJSON_parseArray(tokenizer, NULL);
            jsJSON_add(root, child);
        } else if( tokenizer->tokenType == jsJSON_TokenType_STRING ) {
            jsJSON_addString(root, NULL, jsJSON_strdup(tokenizer->token));
        } else if( tokenizer->tokenType == jsJSON_TokenType_NUMBER ) {
            jsJSON_addNumber(root, NULL, atof(tokenizer->token));
        } else if( tokenizer->tokenType == jsJSON_TokenType_BOOLEAN ) {
            jsJSON_addBoolean(root, NULL, tokenizer->token[0] == 't');
        } else {
            printf("Error: unexpected token [%s]\n", tokenizer->token);
            exit(1);
        }
        jsJSON_Tokenizer_nextExpectTwoOptions(tokenizer, ',', ']');
        if( tokenizer->token[0] == ',' ) {
            jsJSON_Tokenizer_next(tokenizer);
        }
    }
    return root;
}

/**
 * Parses a JSON string into a tree structure of jsJSON nodes.
 * Every node copies the key and values (either string, double or bool) 
 * into its own memory so that the original JSON string can be freed.
*/
jsJSON* jsJSON_parse(char *json) {
    jsJSON_Tokenizer tokenizer = jsJSON_Tokenizer_new(json);
    jsJSON_Tokenizer_next(&tokenizer);
    //printf("starting: %s\n", tokenizer.token);
    if( tokenizer.token[0] == '{' ) {
        return jsJSON_parseObject(&tokenizer, NULL);
    } else if( tokenizer.token[0] == '[' ) {
        return jsJSON_parseArray(&tokenizer, NULL);
    } else{
        printf("Error: unexpected token [%s]\n", tokenizer.token);
    }
    return NULL;
}

/**
 * Recursively frees all nodes in the tree including
 * all children, sibblings and stringValues. Note that
 * jsJSON duplicates all strings so that a node tree is
 * self-contained.
*/
jsJSON* jsJSON_free(jsJSON *root) {
    if (root->type == jsJSON_TYPE_STRING) {
        free(root->stringValue);
    }
    if (root->type == jsJSON_TYPE_OBJECT || root->type == jsJSON_TYPE_ARRAY) {
        jsJSON* child = root->children;
        while( child != NULL ) {
            jsJSON* next = child->sibblings;
            jsJSON_free(child);
            child = next;
        }
    }
    free(root);
    return NULL;
}