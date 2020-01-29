// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by chernovda on 09.11.18.
//


#include <jmorecfg.h>
#include "MST.h"
#include "XRF.h"

MST::MST() {
}

std::ifstream MST::loadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::in);

    return file;
}

std::ofstream MST::loadFileWrite(const std::string& path) {
    std::ofstream file(path, std::ios::app | std::ios::binary);

    return file;
}

std::fstream MST::loadFileRW(const std::string& path) {
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);

    return file;
}


MST::mainRecord MST::readMainRecord(std::ifstream* file) {
    file->read(reinterpret_cast<char*>(&director), sizeof(mainRecord));

    return director;
}

MST::mainRecord MST::readMainRecord_master(std::fstream* file) {
    file->read(reinterpret_cast<char*>(&director), sizeof(mainRecord));

    return director;
}


void MST::printMainRecord(const MST::mainRecord& mainRec) {
    std::cout << "\nCTLMFN: " << htobe32(mainRec.CTLMFN) <<
              "\nNXTMFN: "<< htobe32(mainRec.NXTMFN) <<
              "\nNXT_LOW: "<< htobe32(mainRec.NXT_LOW) <<
              "\tNXT_HIGH: "<< htobe32(mainRec.NXT_HIGH) <<
              "\tMFT_TYPE: "<< htobe32(mainRec.MFN_TYPE) <<
              "\tRECCNT: "<< htobe32(mainRec.RECCNT) <<
              "\tMFCXX1: " <<  htobe32(mainRec.MFCXX1) <<
              "\tMFCXX2: " <<  htobe32(mainRec.MFCXX2) <<
              "\nMFCXX3: " <<  htobe32(mainRec.MFCXX3) << std::endl;
}

MST::leaderRecord MST::readLeaderRecord(std::ifstream* file) {
    file->read(reinterpret_cast<char*>(&leader), sizeof(leaderRecord));
    return leader;
}

MST::leaderRecord MST::readLeaderRecord_master(std::fstream* file) {
    file->read(reinterpret_cast<char*>(&leader), sizeof(leaderRecord));
    return leader;
}

void MST::writeLeaderRecord(const uint32_t &offset, const MST::leaderRecord &leaderRec) {
    printLeaderRecord(leaderRec);
    std::ofstream file = loadFileWrite("/tmp/out_leader_1.txt");
    file.write((char*) &leaderRec, sizeof(leaderRecord));
    file.close();
}


void MST::printLeaderRecord(const MST::leaderRecord& leaderRec) {
    std::cout << "\nMFN: " << htobe32(leaderRec.MFN) <<
              "\nMFRL: "<< htobe32(leaderRec.MFRL) <<
              "\nMFB_LOW: "<< htobe32(leaderRec.MFB_LOW) <<
              "\tMFB_HIGH: "<< htobe32(leaderRec.MFB_HIGH) <<
              "\nBASE: "<< htobe32(leaderRec.BASE) <<
              "\tNVF: "<< htobe32(leaderRec.NVF) << " (" << htobe32(leaderRec.NVF) / 12 << " записи в справочнике)" <<
              "\nSTATUS: " <<  htobe32(leaderRec.STATUS) <<
              "\tVERSION: " <<  htobe32(leaderRec.VERSION) << std::endl;
}

std::vector<MST::referenceRecord> MST::readReference(std::ifstream* file, int countRef) {
    std::vector<MST::referenceRecord> refs;

    for (int i = 0; i < countRef; i++) {
        file->read(reinterpret_cast<char*>(&reference), sizeof(referenceRecord));
        refs.push_back(reference);
    }
    return refs;
}

std::vector<MST::referenceRecord> MST::readReference_master(std::fstream* file, int countRef) {
    std::vector<MST::referenceRecord> refs;

    for (int i = 0; i < countRef; i++) {
        file->read(reinterpret_cast<char*>(&reference), sizeof(referenceRecord));
        refs.push_back(reference);
    }
    return refs;
}


void MST::printReferences(const std::vector<MST::referenceRecord>& refs) {
    for (MST::referenceRecord ref: refs) {
        std::cout << "TAG: " << htobe32(ref.TAG) <<
                  "\t\tPOS: "<< htobe32(ref.POS) <<
                  "\t\tLEN: "<< htobe32(ref.LEN) << std::endl;
    }
}

uint32_t MST::getMaxMFN(const MST::mainRecord& mainRec) {
    return htobe32(mainRec.NXTMFN);
}

int MST::getNumRefs(const MST::leaderRecord& leaderRec) {
    return htobe32(leaderRec.NVF);
}

uint32_t MST::getRecordStart(const MST::leaderRecord& leaderRec) {
    return htobe32(leaderRec.BASE);
}

uint32_t MST::getRecordLength(const MST::leaderRecord& leaderRec) {
    return htobe32(leaderRec.MFRL);
}

uint32_t MST::getRecordMFN(const MST::leaderRecord& leaderRec) {
    return htobe32(leaderRec.MFN);
}

uint32_t MST::getTAG(uint32_t tag) {
    return htobe32(tag);
}

uint32_t MST::getPOS(uint32_t pos) {
    return htobe32(pos);
}

uint32_t MST::getLEN(uint32_t len) {
    return htobe32(len);
}

std::map<std::string, std::string> MST::record(std::string line) {
    std::map<std::string, std::string> record;
    int pos = 0;

    if (line.find('\x3F') != std::string::npos) {
        convertDelimeter(line);
    }
//    std::cout << "\t" << line << std::endl;
    if (line.find('^', pos) != std::string::npos) {
        std::regex re("\\^(.)([^\\^]*)");
        std::sregex_iterator next(line.begin(), line.end(), re);
        std::sregex_iterator end;
        while (next != end) {
            std::smatch match = *next;
            record[match[1]] = match[2];
            ++next;
        }
    } else {
        record["_"] = line;
    }

    return record;
}

void MST::convertDelimeter(std::string& line) {
    for (char& linePos: line) {
        if (linePos == '\x3F') {
            linePos = '\x5E';
        }
    }
}

std::map<uint32_t, std::vector<std::string>> MST::getUserByOffset(uint32_t offset, std::ifstream& file) {
    std::map<uint32_t, std::vector<std::string>> user;

    if (!file)
        return user;

    file.seekg(offset);

    // получаем заголовок записи с служебными данными
    leaderRecord recordLeader = readLeaderRecord(&file);

    // читаем справочник
    std::vector<MST::referenceRecord>refs = MST::readReference(&file, getNumRefs(leader));

    file.seekg(offset + getRecordStart(leader));
    uint32_t length = getRecordLength(leader);

    std::vector<char> buf1(length);
    file.readsome(&buf1[0], length);

    std::vector<MST::referenceRecord>::iterator it;
    std::map<uint32_t, std::vector<std::string>> tag;
    std::map<uint32_t, std::vector<std::string>>::iterator tagIt;

    std::vector<std::string> tmp;
    if (htobe32(leader.STATUS) == 1) { //
        tmp.emplace_back("1");
    } else if (htobe32(leader.STATUS) == 2) {
        tmp.emplace_back("2");
    }
    tag[0] = tmp;
    tmp.clear();
    char s[100];
    sprintf(s, "%d", offset);
    tmp.emplace_back(s);
    tag[10000] = tmp;
    tmp.clear();

    for(it = refs.begin(); it != refs.end(); ++it) {
        tagIt = tag.find(getTAG(it->TAG));
//        std::cout << "tag id: " << getTAG(it->TAG) << std::endl;
        if (getTAG(it->TAG) > 10000) {
            return user;
        }

        if (tagIt == tag.end()) {
            tmp.clear();
            try {
                tmp.push_back(std::string(buf1.data()).substr(getPOS(it->POS), getLEN(it->LEN)));
            } catch (std::out_of_range &e) {

//                        std::cout << "error: " << e.what() << "\nfile: main.cpp: 93" << std::endl;
//                        std::cout << "buf: " << std::string(buf1.data()) << std::endl;
            }
            tag[getTAG(it->TAG)] = tmp;
        } else {
            try {
                if (std::string(buf1.data()).length()) {
                    tag.find(getTAG(it->TAG))->second.push_back(std::string(buf1.data()).substr(getPOS(it->POS), getLEN(it->LEN)));
                }
            } catch (std::out_of_range &e) {
                std::cout  << "tag: " << it->TAG << std::endl;
                std::cout << "error: " << e.what() << "\nfile: main.cpp: 110" << std::endl;
                std::cout << "strlen buf: " << std::string(buf1.data()).length() << std::endl;
                std::cout << "buf: " << std::string(buf1.data()) << std::endl;
                std::cout << std::flush << std::endl;
                return user;
            }
        }
    }

    return tag;
}

std::map<uint32_t, std::vector<std::string>> MST::getUserByOffset_master(uint32_t offset, std::fstream& file) {
    std::map<uint32_t, std::vector<std::string>> user;

    if (!file)
        return user;

    file.seekg(offset);

    // получаем заголовок записи с служебными данными
    leaderRecord recordLeader = readLeaderRecord_master(&file);

    // читаем справочник
    std::vector<MST::referenceRecord>refs = MST::readReference_master(&file, getNumRefs(leader));

    file.seekg(offset + getRecordStart(leader));
    uint32_t length = getRecordLength(leader);

    std::vector<char> buf1(length);
    file.readsome(&buf1[0], length);

    std::vector<MST::referenceRecord>::iterator it;
    std::map<uint32_t, std::vector<std::string>> tag;
    std::map<uint32_t, std::vector<std::string>>::iterator tagIt;

    std::vector<std::string> tmp;
    if (htobe32(leader.STATUS) == 1) { //
        tmp.emplace_back("1");
    } else if (htobe32(leader.STATUS) == 2) {
        tmp.emplace_back("2");
    }
    tag[0] = tmp;
    tmp.clear();
    char s[100];
    sprintf(s, "%d", offset);
    tmp.emplace_back(s);
    tag[10000] = tmp;
    tmp.clear();

    for(it = refs.begin(); it != refs.end(); ++it) {
        tagIt = tag.find(getTAG(it->TAG));
//        std::cout << "tag id: " << getTAG(it->TAG) << std::endl;
        if (getTAG(it->TAG) > 10000) {
            return user;
        }

        if (tagIt == tag.end()) {
            tmp.clear();
            try {
                tmp.push_back(std::string(buf1.data()).substr(getPOS(it->POS), getLEN(it->LEN)));
            } catch (std::out_of_range &e) {

//                        std::cout << "error: " << e.what() << "\nfile: main.cpp: 93" << std::endl;
//                        std::cout << "buf: " << std::string(buf1.data()) << std::endl;
            }
            tag[getTAG(it->TAG)] = tmp;
        } else {
            try {
                if (std::string(buf1.data()).length()) {
                    tag.find(getTAG(it->TAG))->second.push_back(std::string(buf1.data()).substr(getPOS(it->POS), getLEN(it->LEN)));
                }
            } catch (std::out_of_range &e) {
                std::cout  << "tag: " << it->TAG << std::endl;
                std::cout << "error: " << e.what() << "\nfile: main.cpp: 110" << std::endl;
                std::cout << "strlen buf: " << std::string(buf1.data()).length() << std::endl;
                std::cout << "buf: " << std::string(buf1.data()) << std::endl;
                std::cout << std::flush << std::endl;
                return user;
            }
        }
    }

    return tag;
}

void MST::markUserDeleted(uint32_t offset, std::ifstream &file) {
    if (file) {
        file.seekg(offset);
        std::cout << "main offset: " << file.tellg() << std::endl;
        printMainRecord(readMainRecord(&file));
        // получаем заголовок записи с служебными данными
        leaderRecord recordLeader = readLeaderRecord(&file);
        recordLeader.STATUS = be32toh(2);

        writeLeaderRecord(offset, recordLeader);
    }
}

void MST::markUserDeleted_master(uint32_t offset, std::fstream &file) {
    uint32_t offsetBegin = offset;
    if (file) {
        file.seekp(offset);
        readMainRecord_master(&file);
        offset = file.tellp();
        leaderRecord recordLeader = readLeaderRecord_master(&file);
        file.seekp(offset);
        recordLeader.STATUS = be32toh(1);
        file.write((char*) &recordLeader, sizeof(leaderRecord));
        file.flush();
    }
}

bool MST::isDebtor(std::vector<std::string> &visits) {
    for (auto visit: visits) {
        std::regex re("\\^([Ff])([^\\^]*)");
        std::sregex_iterator next(visit.begin(), visit.end(), re);
        std::sregex_iterator end;
        while (next != end) {
            std::smatch match = *next;
            if (match[2] == "******") {

                return true;
            }
            ++next;
        }
    }

    return false;
}


