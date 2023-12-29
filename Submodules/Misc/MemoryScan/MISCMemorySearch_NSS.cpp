// MISCMemorySearch_NSS.cpp
#include "MISCMemorySearch_NSS.h"

INT64 MISCMemorySearch_NSS::FindIndexOfSequenceInMemory(CHAR* cMemToSearch, DWORD dwMemLen, CHAR* cSequence, DWORD dwSeqLen) {
    for (DWORD i = 0; i <= dwMemLen - dwSeqLen; i++) {
        if (memcmp(cMemToSearch + i, cSequence, dwSeqLen) == 0) {
            return i;
        }
    }
    return ERROR_SEQUENCE_DOESNT_EXIST;
}

INT64 MISCMemorySearch_NSS::FindIndexOfSequenceInMemoryReverse(CHAR* cMemToSearch, DWORD dwMemLen, CHAR* cSequence, DWORD dwSeqLen) {
    for (DWORD i = dwMemLen - dwSeqLen; i != -1; i--) {
        if (memcmp(cMemToSearch + i, cSequence, dwSeqLen) == 0) {
            return i;
        }
    }
    return ERROR_SEQUENCE_DOESNT_EXIST;
}