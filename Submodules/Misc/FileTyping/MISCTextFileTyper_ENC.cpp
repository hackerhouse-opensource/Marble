// MISCTextFileTyper_ENC.cpp
#include "MISCTextFileTyper_ENC.h"
#include <fstream>

TextEncoding MISCTextFileTyper_ENC::GetTextEncoding(WCHAR* wcFileName) {
    std::ifstream file(wcFileName, std::ios::binary);
    char buffer[4] = { 0 };
    file.read(buffer, 4);
    file.close();

    if (buffer[0] == '\xFF' && buffer[1] == '\xFE') {
        if (buffer[2] == '\x00' && buffer[3] == '\x00') {
            return Unicode;
        }
        else {
            return UnicodeBigEndian;
        }
    }
    else if (buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF') {
        return UTF8;
    }
    else {
        return ANSI;
    }
}