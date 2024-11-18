#include "files.h"
#include <Windows.h>


File openFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname) {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    strncpy_s(szFile, defaultname.c_str(), 260);

    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = handle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return { ofn.lpstrFile, ofn.nFilterIndex };
    }

    return { "", 0 };
}

File saveFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname) {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    strncpy_s(szFile, defaultname.c_str(), 260);

    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = handle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        return { ofn.lpstrFile, ofn.nFilterIndex };
    }

    return { "", 0 };
}

