#ifndef MINIARGPARSE_H
#define MINIARGPARSE_H

#define MINIARGPARSE_OPT(storeValue, _shortName, _longName, _valCount, _description)    \
    struct {                                                                            \
        const char *shortName;                                                          \
        const char *longName;                                                           \
        unsigned int valCount;                                                          \
        const char *description;                                                        \
        unsigned short used;                                                            \
    } storeValue = {                                                                    \
        .shortName = _shortName,                                                        \
        .longName = _longName,                                                          \
        .valCount = _valCount,                                                          \
        .description = _description,                                                    \
        .used = 0 };

#endif // MINIARGPARSE_H
