//
// Created by chernovda on 09.11.18.
//

#ifndef IRBIS64BOOKIMPORT_XRF_H
#define IRBIS64BOOKIMPORT_XRF_H
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

class XRF {
public:

    struct crossLinks {
        uint32_t XRF_LOW:32;
        uint32_t XRF_HIGH:32;
        uint32_t XRF_FLAGS:32;
    } crossLink;

    XRF() {};
    ~XRF() {};


    std::ifstream loadFile(const std::string& path);
    std::fstream loadFile_master(const std::string& path);
    crossLinks readXRF(std::ifstream *fileCrossLink);
    crossLinks readXRF_master(std::fstream *fileCrossLink);
    void printXRF(const crossLinks& xrf);

    uint32_t getOffset(const crossLinks& xrf);

    void markUserDeleted_master(uint32_t, std::fstream&);
};

#endif //IRBIS64BOOKIMPORT_XRF_H
