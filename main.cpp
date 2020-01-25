// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "XRF.h"
#include "MST.h"
#include "parsing.h"


#define MAX_DEBUG_RECORDS 100000
const size_t CH_B_FIELD = 30;

enum params {CH_B = 1, ERK = 2, REMOVE = 3, ERROR = 0};

/**
 *
 * @param year
 * @param record
 * @return boolean
 */
bool needDelete(int year, const std::vector<std::string>& record);

/**
 *
 * @return
 */
params parseParams(int, char*[]);

void removeUsers(std::fstream &fileRW, std::fstream &fileXRF, std::string mstFile, std::vector<std::map<uint32_t, std::vector<std::string>>> &parsedRecords);
void getERK(uint32_t, std::vector<std::map<uint32_t, std::vector<std::string>>>&);
void getCountCH_B(std::vector<std::map<uint32_t, std::vector<std::string>>> &);

int main(int argc, char* argv[]) {
    if (parseParams(argc, argv) == ERROR) {
        return -1;
    }

    const std::string mstFile = std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".MST");

    MST* mst = new MST(mstFile);
    XRF* xrf = new XRF();
    std::vector<XRF::crossLinks> xrfLinks;

    /**
     * Вся строка записи
     */
    std::vector<std::string> records;

    /**
     * Запись, разобранная на поля
     */
    std::vector<std::map<uint32_t, std::vector<std::string>>> parsedRecords;

    uint32_t maxMFN = 0;

    std::ifstream file = mst->loadFile(mstFile);
    if (file) {
        std::cout << "файл открыт: "<< argv[3] << ".MST" << std::endl;
        MST::mainRecord mstMasterRecord = mst->readMainRecord(&file);
        maxMFN = mst->getMaxMFN(mstMasterRecord);

        std::ifstream fileCrossLink = xrf->loadFile(std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".XRF"));

        if (fileCrossLink) {
            std::cout << "файл открыт: " << argv[3] << ".XRF" << std::endl;

            for (uint32_t i = 1; i < maxMFN; i++) {
                xrfLinks.push_back(xrf->readXRF(&fileCrossLink));
            }

            fileCrossLink.close();
        } else {
            std::cout << "ошибка открытия файла перекрестных ссылок" << std::endl;
            return -1;
        }

        std::cout << "max MFN: " << maxMFN << "\nxrf: " << xrfLinks.size() << std::endl;

        for (XRF::crossLinks& link: xrfLinks) {
            // переходим на следующую запись
            // offset - смещение относительно начала файла
            uint32_t offset = xrf->getOffset(link);
            file.seekg(offset);

            // получаем заголовок записи с служебными данными
            MST::leaderRecord leader = mst->readLeaderRecord(&file);

            // читаем справочник
            std::vector<MST::referenceRecord>refs = mst->readReference(&file, mst->getNumRefs(leader));

            file.seekg(offset + mst->getRecordStart(leader));
            uint32_t length = mst->getRecordLength(leader);

            std::vector<char> buf1(length);
            file.readsome(&buf1[0], length);
            records.emplace_back(buf1.data());

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
            sprintf(s, "%d", xrf->getOffset(link));
            tmp.emplace_back(s);
            tag[10000] = tmp;
            tmp.clear();

            for(it = refs.begin(); it != refs.end(); ++it) {
                tagIt = tag.find(mst->getTAG(it->TAG));
                //std::cout << "tag id: " << mst.getTAG(it->TAG) << std::endl;
                if (mst->getTAG(it->TAG) > 10000) {

                    return -1;
                }

                if (tagIt == tag.end()) {
                    tmp.clear();
                    try {
                        tmp.push_back(std::string(buf1.data()).substr(mst->getPOS(it->POS), mst->getLEN(it->LEN)));
                    } catch (std::out_of_range &e) {

//                        std::cout << "error: " << e.what() << "\nfile: main.cpp: 93" << std::endl;
//                        std::cout << "buf: " << std::string(buf1.data()) << std::endl;
                    }
                    tag[mst->getTAG(it->TAG)] = tmp;
                } else {
                    try {
                        if (std::string(buf1.data()).length()) {
                            tag.find(mst->getTAG(it->TAG))->second.push_back(std::string(buf1.data()).substr(mst->getPOS(it->POS), mst->getLEN(it->LEN)));
                        }
                    } catch (std::out_of_range &e) {
                        std::cout  << "tag: " << it->TAG << std::endl;
                        std::cout << "error: " << e.what() << "\nfile: main.cpp: 110" << std::endl;
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

//            if (htobe32(leader.STATUS) == 1) {
//                std::cout << "Целый" << std::endl;
//            }
//            if (htobe32(leader.STATUS) == 2) {
//                std::cout << "Логически удаленный" << std::endl;
//                //break;
//            }
        }
        file.close();
    } else {
        std::cout << "error read master record. exiting" << std::endl;
        return -1;
    }



    /**
     * Тест-Чтение того, что получилось
     */
    std::map<std::string, std::string> record;
    std::map<std::string, std::string>::iterator subIt;
    parsing parse;

    bool canCheckReRegField;
    unsigned int countToDelete = 0;
//    file = mst->loadFile(std::string(argv[1]) + '/' + std::string(argv[2]) + std::string(".MST"));

    std::fstream fileRW = mst->loadFileRW(std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".MST"));
    std::fstream fileXRF = xrf->loadFile_master(std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".XRF"));

    switch (parseParams(argc, argv)) {
        case REMOVE: {
            removeUsers(fileRW, fileXRF, mstFile, parsedRecords);
            break;
        }
        case ERK: {
            std::time_t result = std::time(nullptr);
            uint32_t lastYear = 1900 + localtime(&result)->tm_year;

            getERK(lastYear - 1, parsedRecords);
            break;
        }
        case CH_B: {
            getCountCH_B(parsedRecords);
            break;
        }
        case ERROR: {

            fileRW.close();
            return -1;
            break;
        }
    }
    fileRW.flush();
    fileRW.close();

    return 0;
}

bool needDelete(int year, const std::vector<std::string>& record) {
    parsing parse;
    parse.setCurYear(year);
    return parse.regLastYears(year, record);
}

params parseParams(int argc, char* argv[]) {
    if (argc > 3) {
        if (strncmp(argv[1], "chb", 2) == 0) {
            std::cout << "Подсчет количества выданных читательских билетов." << std::endl;
            return CH_B;
        } else if (strncmp(argv[1], "erk", 3) == 0) {
            std::cout << "Подсчет ЕРК." << std::endl;
            return ERK;
        }  else if (strncmp(argv[1], "remove", 6) == 0) {
            std::cout << "Удаление читаталей, непосещавших библиотеку более 3х лет" << std::endl;
            return REMOVE;
        } else {
            return ERROR;
        }
    } else {
        std::cout << "Не указаны параметры для работы приложения" << std::endl;
        std::cout << "Необходимо: " << argv[0] << " МЕТОД_РАБОТЫ [ПУТЬ_К_БАЗЕ БАЗА]" << std::endl;
        return ERROR;
    }
}
void removeUsers(std::fstream &fileRW, std::fstream &fileXRF, std::string mstFile, std::vector<std::map<uint32_t, std::vector<std::string>>> &parsedRecords) {
    bool canCheckReRegField;
    unsigned int countToDelete = 0;
    bool deleteAnyway;

    MST* mst = new MST(std::move(mstFile));
    XRF* xrf = new XRF();

    std::time_t result = std::time(nullptr);
    int lastYear = 1900 + localtime(&result)->tm_year;
    std::cout << "current year:" << lastYear << std::endl;

    for (auto &it: parsedRecords) {
        canCheckReRegField = false;
        uint32_t offset = 0;
        char* deleteRecord;
        for (auto &recordIt: it) {
            if (recordIt.first == 30) {
                deleteRecord = recordIt.second.front().data();
            }
            if (recordIt.first == 51) {
                bool reReg51 = needDelete(lastYear, recordIt.second);
                if (reReg51) {
//                    std::cout << "Регистрации за последние 3 года небыло. Проверяем перерегистрацию" << std::endl;
                    canCheckReRegField = true;
                } else {
//                    std::cout << "Присутствует регистрация. Удаление не требуется" << std::endl;
                }
            }

            if (canCheckReRegField) {

                if (recordIt.first == 52) {
                    bool reReg52 = needDelete(lastYear, recordIt.second);
                    if (reReg52) {
                        uint32_t offset = 0;
                        deleteAnyway = true;
                        for (auto &tag: it) {
                            switch (tag.first) {
                                case 0: {
                                    std::vector<std::string> tmp;
                                    tmp.emplace_back("2");
                                    tag.second = tmp;
                                    break;
                                }
                                case 10000: {
                                    offset = std::stoi(tag.second.front().c_str());
                                    break;
                                }
//                                case 30: {
//                                    std::cout << "30: " << tag.second.front().data() << std::endl;
//                                }
                                case 40: {
                                    deleteAnyway = !mst->isDebtor(tag.second);
                                    break;
                                }
                            }
                        }

                        if (deleteAnyway) {
                            ++countToDelete;
                            std::cout << "delete: " << deleteRecord << std::endl;
                            mst->markUserDeleted_master(offset, fileRW);
                            xrf->markUserDeleted_master(offset, fileXRF);
                        }
                    } else {
//                        std::cout << "Удаление не требуется" << std::endl;
                    }
                }
            }
        }
    }

    free(mst);
    free(xrf);

    std::cout << "Удалено " << countToDelete << " записей" << std::endl;
}

void getCountCH_B(std::vector<std::map<uint32_t, std::vector<std::string>>> &parsedRecords) {
    size_t countCh_B = 0;
    for (auto &it: parsedRecords) {
        for (auto &recordIt: it) {
//            if (recordIt.first == CH_B_FIELD && (strlen(recordIt.second.front().data()) == LENGTH_CH_B) {
            if (recordIt.first == CH_B_FIELD &&
                (
                    strncmp(recordIt.second.front().data(), "ф", 1) == 0 ||
                    strncmp(recordIt.second.front().data(), "Ф", 1) == 0 ||
                    strncmp(recordIt.second.front().data(), "a", 1) == 0 ||
                    strncmp(recordIt.second.front().data(), "A", 1) == 0
                )
            ) {
               ++countCh_B;
            }
        }
    }

    std::cout << "Количество выданных читательских билетов: " << countCh_B << std::endl;
}

void getERK(uint32_t year, std::vector<std::map<uint32_t, std::vector<std::string>>> &parsedRecords) {
    parsing* parse = new parsing(year);
    std::string regDate;
    std::string division;
    parsing::map__registeredFilials regInFilials;

    for (auto &it: parsedRecords) {
        for (auto &recordIt: it) {
            switch (recordIt.first) {
                case MST::REG: {
                    for (const auto& regField: recordIt.second) {
                        // Сравниваем искомый год с записью в поле регистрации
                        regDate = parse->getFieldIndexValue(MST::REG_DATE, regField);
                        if (year == parse->getYearNumber(regDate)) {
                            /**
                             * Если год совпадает
                             */
                            division = parse->getFieldIndexValue(MST::REG_DIVISION, regField);
                            if (parse->notInArray(division, regInFilials)) { // проверка на вхождение в массив филиалов
                                if (strlen(division.c_str()) == 0) {
                                    std::cout << regDate << std::endl;
                                }
                                std::cout << "Новый филиал: <" << division << "> len: " << strlen(division.c_str()) << std::endl;
                                regInFilials[division] = 1;
                            } else {
                                regInFilials[division] = ++regInFilials[division]; // увеличение количества
                            }
                        }
                    }
                    break;
                }
                case MST::REREG: {
                    for (const auto& regField: recordIt.second) {
                        // Сравниваем искомый год с записью в поле регистрации
                        regDate = parse->getFieldIndexValue(MST::REG_DATE, regField);
                        if (year == parse->getYearNumber(regDate)) {
                            /**
                             * Если год совпадает
                             */
                            division = parse->getFieldIndexValue(MST::REG_DIVISION, regField);
                            if (parse->notInArray(division, regInFilials)) { // проверка на вхождение в массив филиалов
                                std::cout << "Новый филиал: <" << division << ">"<< std::endl;
                                regInFilials[division] = 1;
                            } else {
                                regInFilials[division] = ++regInFilials[division]; // увеличение количества
                            }
                        }
                    }
                    break;
                }
            }
        }

    }

    free(parse);

    for (const auto& filial: regInFilials) {
        std::cout << filial.first << ": " << filial.second << std::endl;
    }


}
