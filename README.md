# jsJSON
Simple, zero dependency JSON library in pure C. 

Consider the following example:
```C
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
```
This prints
```
{"message": "Hello darkness my old friend!", "some number": 0.003242, "sub messages": ["this is m1", "this is m2asdasdaa", "this is m3"]}
```

This also works the other way around: Parsing is implemented too. Check
out this example.
```C
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
```
Integration of `jsJSON` is dead simple, just copy the two files `jsJSON.h` and `jsJSON.c` into your project.