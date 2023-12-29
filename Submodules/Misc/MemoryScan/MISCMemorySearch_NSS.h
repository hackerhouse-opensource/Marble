// MISCMemorySearch_NSS.h
#ifndef MISC_MEMORY_SEARCH_NSS_H
#define MISC_MEMORY_SEARCH_NSS_H

#include <windows.h>

#define ERROR_SEQUENCE_DOESNT_EXIST -1

class MISCMemorySearch_NSS {
public:
    static INT64 FindIndexOfSequenceInMemory(CHAR* cMemToSearch, DWORD dwMemLen, CHAR* cSequence, DWORD dwSeqLen);
    static INT64 FindIndexOfSequenceInMemoryReverse(CHAR* cMemToSearch, DWORD dwMemLen, CHAR* cSequence, DWORD dwSeqLen);
};

#endif // MISC_MEMORY_SEARCH_NSS_H