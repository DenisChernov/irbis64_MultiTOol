//
// Created by chernovda on 26.01.2020.
//

#include <cstdint>
#include "fileStarter.h"

fileStarter::fileStarter(const std::string& filespath) {
    mstFile = filespath + ".MST";
    xrfFile = filespath + ".XRF";

    mst = new MST();
    xrf = new XRF();
    
    fileMST = mst->loadFile(mstFile);
    fileCrossLink = xrf->loadFile(xrfFile);
    
    if (fileMST.is_open() && fileCrossLink.is_open()) {
        std::cout << "файл открыт: "<< mstFile << std::endl;
        std::cout << "файл открыт: "<< xrfFile << std::endl;
        
        std::cout << "max MFN: " << readBD() << std::endl;

        fileMST.close();
        fileCrossLink.close();
    } else {
        if (!fileMST.is_open()) {
            perror("Ошибка открытия файла типа MST");
        }
        if (!fileCrossLink.is_open()) {
            perror("ошибка открытия файла перекрестных ссылок");
        }
    }
}

fileStarter::~fileStarter() = default;

uint32_t fileStarter::readBD() {
    MST::mainRecord mstMasterRecord = mst->readMainRecord(&fileMST);
    setMaxMfn(mst->getMaxMFN(mstMasterRecord));
    /**
     * Считываем смещения в файле XRF
     */
    for (uint32_t i = 1; i < maxMFN; i++) {
        xrfLinks.push_back(xrf->readXRF(&fileCrossLink));
    }

    for (XRF::crossLinks& link: xrfLinks) {
        // переходим на следующую запись
        // offset - смещение относительно начала файла
        uint32_t offset = xrf->getOffset(link);
        fileMST.seekg(offset);

        // получаем заголовок записи с служебными данными
        MST::leaderRecord leader = mst->readLeaderRecord(&fileMST);

        // читаем справочник
        std::vector<MST::referenceRecord>refs = mst->readReference(&fileMST, mst->getNumRefs(leader));

        fileMST.seekg(offset + mst->getRecordStart(leader));
        uint32_t length = mst->getRecordLength(leader);

        std::vector<char> buf1(length);
        fileMST.readsome(&buf1[0], length);
        records.emplace_back(buf1.data());

        std::vector<MST::referenceRecord>::iterator it;
        std::map<uint32_t, std::vector<std::string>> tag;
        std::map<uint32_t, std::vector<std::string>>::iterator tagIt;

        std::vector<std::string> tmp;
        if (htobe32(leader.STATUS) == 1) {
            tmp.emplace_back("1");
        } else if (htobe32(leader.STATUS) == 2) {
            tmp.emplace_back("2");
        }
        tag[0] = tmp;
        tmp.clear();
        char s[100];
        sprintf(s, "%d", xrf->getOffset(link));
        tmp.emplace_back(s);
        tag[10000] = tmp;
        tmp.clear();

        for(it = refs.begin(); it != refs.end(); ++it) {
            tagIt = tag.find(mst->getTAG(it->TAG));
            if (mst->getTAG(it->TAG) > 10000) {
                // TODO: непредвиденная ситуация
                return -1;
            }

            if (tagIt == tag.end()) {
                tmp.clear();
                try {
                    tmp.push_back(std::string(buf1.data()).substr(mst->getPOS(it->POS), mst->getLEN(it->LEN)));
                } catch (std::out_of_range &e) {
                    // TODO: Обработка out_of_range ошибки
                }
                tag[mst->getTAG(it->TAG)] = tmp;
            } else {
                try {
                    if (std::string(buf1.data()).length()) {
                        tag.find(mst->getTAG(it->TAG))->second.push_back(std::string(buf1.data()).substr(mst->getPOS(it->POS), mst->getLEN(it->LEN)));
                    }
                } catch (std::out_of_range &e) {
                    std::cout  << "tag: " << it->TAG << std::endl;
                    std::cout << "error: " << e.what() << std::endl;
                    std::cout << "strlen buf: " << std::string(buf1.data()).length() << std::endl;
                    std::cout << "buf: " << std::string(buf1.data()) << std::endl;
                    std::cout << std::flush << std::endl;
                    return 0;
                }
            }
        }
        parsedRecords.emplace_back(tag);
        tag.clear();
        buf1.clear();

        //std::cout << "Количество отработанных записей: " << parsedRecords.size() << " / " << maxMFN << std::endl;
        if (parsedRecords.size() >= MAX_DEBUG_RECORDS) {
            break;
        }
    }

    return getMaxMFN();
}

void fileStarter::setMaxMfn(const uint32_t &maxMfn) {
    maxMFN = maxMfn;
}

const uint32_t &fileStarter::getMaxMFN() const {
    return maxMFN;
}

const std::vector<std::map<uint32_t, std::vector<std::string>>> &fileStarter::getParsedRecords() const {
    return parsedRecords;
}
