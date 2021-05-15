#ifndef MINIARGPARSE_H
#define MINIARGPARSE_H

#include <string.h>
#include <assert.h>

typedef struct MiniargparseInfoBits {
    unsigned int hasValue   : 1;
    unsigned int hasErr     : 1;
    unsigned int used       : 1;
    unsigned int duplicate  : 1;
} miniargparseInfoBits;

typedef struct MiniargparseOpt {
    const char *shortName;
    const char *longName;
    const char *description;
    const char *value;
    const char *errValMsg;
    unsigned int index;
    miniargparseInfoBits infoBits;
    struct MiniargparseOpt *next;
} miniargparseOpt;

#define STR_MATCH(str1, str2) (strcmp(str1, str2) == 0)
#define SUBSTR_MATCH(str1, str2) (strstr(str1, str2) != NULL)
#define STR_USED(val) (!(val == NULL) && !(strcmp(val, "") == 0))

// Global error strings
enum miniargparseErrIndexes {
    ERR_MALFORMED_OPT_VAL,
    ERR_OPT_VAL_END_ARGV,
    ERR_VAL_IS_OPT
};
static const char *g_miniargparseErrStrs[] = {
    "Malformed --<option>=<value>",
    "Option already at end of argv",
    "Value has option syntax (i.e. -, --)"
};

// Global ptr to head of options
static miniargparseOpt *g_miniargparseHead;

#define MINIARGPARSE_OPT(storeVal, sName, lName, hasVal, desc)                                          \
    assert(!STR_MATCH("-", sName) &&                                                                    \
        "[miniargparse]: ERROR - The '-' character is not permitted as a shortName");                   \
    assert(!(strlen(sName) > 1) &&                                                                      \
        "[miniargparse]: ERROR - The shortName string can only have 1 character");                      \
    assert(!((strlen(lName) == 0) && (strlen(sName)) == 0) &&                                           \
        "[miniargparse]: ERROR - Both shortName and longName are empty");                               \
    miniargparseOpt storeVal =                                                                          \
        { "-" sName, "--" lName, desc, "", "", 0, { hasVal, 0, 0 }, NULL };                             \
    if (strcmp(storeVal.shortName, "-") == 0) { storeVal.shortName = ""; }                              \
    if (strcmp(storeVal.longName, "--") == 0) { storeVal.longName = ""; }                               \
    do {                                                                                                \
        if (g_miniargparseHead == NULL) {                                                               \
            g_miniargparseHead = &storeVal;                                                             \
        }                                                                                               \
        else {                                                                                          \
            miniargparseOpt *tmp = g_miniargparseHead;                                                  \
            while (tmp->next != NULL) {                                                                 \
                tmp = tmp->next;                                                                        \
            }                                                                                           \
            tmp->next = &storeVal;                                                                      \
        }                                                                                               \
    } while(0)

static int miniargparseParse(int argc, char *argv[]) {
    miniargparseOpt *tmp;
    int firstUnknownOpt = 0;
    for (size_t i=1; i<argc; ++i) {
        // Check if arg is not a option-type
        if (argv[i][0] != '-') { continue; }

        int isLongOpt = 0;
        int validOpt = 0;
        tmp = g_miniargparseHead;

        // Look for opt in opts list
        while (tmp != NULL) {
            if (STR_USED(tmp->shortName) && STR_MATCH(tmp->shortName, argv[i])) {
                if (tmp->infoBits.used) { tmp->infoBits.duplicate = 1; }
                tmp->infoBits.used = 1;
                tmp->index = i;
                validOpt = 1;
            }
            else if (STR_USED(tmp->longName) && SUBSTR_MATCH(argv[i], tmp->longName)) {
                if (tmp->infoBits.used) { tmp->infoBits.duplicate = 1; }
                isLongOpt = 1;
                tmp->infoBits.used = 1;
                tmp->index = i;
                validOpt = 1;
            }
            if (tmp->infoBits.used && tmp->infoBits.hasValue && validOpt) {
                if (isLongOpt) {
                    char *val;
                    size_t offset;
                    val = strchr(argv[i], '=');
                    if (val == NULL) {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[ERR_MALFORMED_OPT_VAL];
                        tmp->value = argv[i];
                    }
                    else {
                        offset = (int)(val - argv[i]);
                        tmp->value = &argv[i][offset+1];
                    }
                }
                else {
                    if ((i+1) >= argc) {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[ERR_OPT_VAL_END_ARGV];
                    }
                    else if (argv[i+1][0] == '-') {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[ERR_VAL_IS_OPT];
                        tmp->value = argv[i+1];
                    }
                    else {
                        tmp->value = argv[i+1];
                        ++i;
                    }
                }
            }
            if (tmp->infoBits.used && validOpt) { break; }
            tmp = tmp->next;
        }

        // Track first occurance of unknown opt
        if (!validOpt && firstUnknownOpt == 0) { firstUnknownOpt = i; }
    }
    return firstUnknownOpt;
}

static int miniargparseGetPositionalArg(int argc, char *argv[], size_t argvOffset) {
    size_t i;
    for (i=argvOffset+1; i<argc; ++i) {
        // Skip if opt-type value
        if (argv[i][0] == '-') { continue; }

        // Otherwise check if arg is opt-value type or not
        int isOptValue = 0;
        miniargparseOpt *tmp = g_miniargparseHead;
        while (tmp != NULL) {
            if (tmp->infoBits.used && tmp->infoBits.hasValue && tmp->index == i-1) {
                isOptValue = 1;
            }
            tmp = tmp->next;
        }

        // Found positional arg
        if (!isOptValue) { return i; }
    }
    return 0;
}

#endif // MINIARGPARSE_H
