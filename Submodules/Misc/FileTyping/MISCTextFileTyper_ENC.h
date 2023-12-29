// MISCTextFileTyper_ENC.h
#ifndef MISC_TEXT_FILE_TYPER_ENC_H
#define MISC_TEXT_FILE_TYPER_ENC_H

#include <windows.h>

enum TextEncoding {
    UnkownEncoding = -1,
    ANSI = 0,
    Unicode,
    UTF8,
    UnicodeBigEndian
};

class MISCTextFileTyper_ENC {
public:
    TextEncoding GetTextEncoding(WCHAR* wcFileName);
};

#endif // MISC_TEXT_FILE_TYPER_ENC_H