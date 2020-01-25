// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by chernovda on 09.11.18.
//

#include "XRF.h"

std::ifstream XRF::loadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);

    return file;
}

std::fstream XRF::loadFile_master(const std::string& path) {
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);

    return file;
}

XRF::crossLinks XRF::readXRF(std::ifstream* fileCrossLink) {
    fileCrossLink->read(reinterpret_cast<char*>(&crossLink), sizeof(crossLinks));

    return crossLink;
}

XRF::crossLinks XRF::readXRF_master(std::fstream* fileCrossLink) {
    fileCrossLink->read(reinterpret_cast<char*>(&crossLink), sizeof(crossLinks));

    return crossLink;
}

void XRF::printXRF(const XRF::crossLinks& xrf) {
    std::ios_base::fmtflags f( std::cout.flags() );
//    std::cout << "XRF_LOW => sign32: "  << htobe32(xrf.XRF_LOW) << "\t\thex: " << std::hex << xrf.XRF_LOW << std::endl;
    std::cout << "XRF_LOW => sign32: "  << htobe32(xrf.XRF_LOW) << std::endl;
    std::cout.flags(f);
    std::cout << "XRF_HIGH => sign32: " << htobe32(xrf.XRF_HIGH) << std::endl;
    std::cout.flags(f);
    std::cout << "XRF_FLAGS: "<< htobe32(xrf.XRF_FLAGS) << std::endl;
    std::cout << std::flush << std::endl;
}

uint32_t XRF::getOffset(const XRF::crossLinks& xrf) {
    return htobe32(xrf.XRF_LOW);
}

void XRF::markUserDeleted_master(uint32_t offset, std::fstream &file) {
    uint32_t previousOffset = 0;
    if (file) {
        while (!file.eof()) {
            crossLink = readXRF_master(&file);

            if (offset == getOffset(crossLink)) {

                file.seekp(previousOffset);
//                printXRF(crossLink);
                crossLink.XRF_FLAGS = be32toh(1);
//                printXRF(crossLink);
                file.write((char*) &crossLink, sizeof(crossLinks));
                file.flush();

                break;
            }
            previousOffset = file.tellp();
        }
    }
}
