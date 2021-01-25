#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "qjson.h"
#ifdef __AFL_COMPILER 
#include <json.h>
#endif


void printHelp(FILE *out) {
    fprintf(out, "Usage: qjson <qjson file> | -v | -? | --help\n");
    fprintf(out, "Print the qjson file content converted to JSON to stdout and return 0. "
    "When the qjson is invalid, print an error message to stderr and return 1.\n");
    fprintf(out, "  -v           outputs the version.\n");
    fprintf(out, "  -?, --help   outputs this help message.\n");
    fprintf(out, "\nReturn status is 0 when the convertion was successful, 1 otherwise\n");
}


bool argsContain(int argc, char *argv[], const char *val) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], val) == 0)
            return true;
    }
    return false;
}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "error: require a file name or an option as argument\n");
        printHelp(stderr);
        return 1;
    }

    if (argsContain(argc, argv, "-?") || argsContain(argc, argv, "--help")) {
        printHelp(stdout);
        return 0;
    }

    if (argsContain(argc, argv, "-v")) {
        printf("%s\n", qjson_version());
        return 0;
    }

    const char *fileName = argv[1];
    struct stat st;
    if (stat(fileName, &st) != 0) {
        fprintf(stderr, "error: file '%s' not found\n", fileName);
        return 1;
    }
    
    if (!S_ISREG(st.st_mode)) {
        fprintf(stderr, "error: file '%s' is not a regular file\n", fileName);
        return 1;
    }

    char *qjsonText = malloc(st.st_size+1);
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "error: can’t open file '%s' for reading\n", fileName);
        return 1;
    }

    long n = read(fd, qjsonText, st.st_size);
    if (n != st.st_size) {
        fprintf(stderr, "error: failed to read file '%s'\n", fileName);
        return 1;
    }

    close(fd);

    qjsonText[st.st_size] = '\0';

    char *jsonText = qjson_decode(qjsonText);
    assert(jsonText != NULL && jsonText[0] != '\0');
    free(qjsonText);
    if (jsonText[0] != '{') {
        fprintf(stderr, "qjson: %s\n", jsonText);
        free(jsonText);
        return 1;
    }

#ifdef __AFL_COMPILER 
    // check json validity
    struct json_object *jobj;
    jobj = json_tokener_parse(jsonText);
    if (jobj == NULL) {
        fprintf(stderr, "qjson: invalid json: %s\n", jsonText);
        free(jsonText);
        return 1;
    }
    json_object_put(jobj); // free memory
#endif

    printf("%s\n", jsonText);
    free(jsonText);
    return 0;
}