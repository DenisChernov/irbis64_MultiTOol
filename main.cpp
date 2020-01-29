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
#include "tableViewExport.h"
#include "singleRegistrationCard.h"
#include "dailyStats.h"

const size_t CH_B_FIELD = 30;

enum params {CH_B = 1, ERK = 2, REMOVE = 3, DAILY_STATS = 4, ERROR = 0};

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


// erk ../RDR rdr
int main(int argc, char* argv[]) {
    if (parseParams(argc, argv) == ERROR) {
        return -1;
    }

    std::string filePathBD = std::string(argv[2]) + '/' + std::string(argv[3]);
    std::string bdName = argv[3];

    /**
     * Тест-Чтение того, что получилось
     */
    std::map<std::string, std::string> record;
    std::map<std::string, std::string>::iterator subIt;
    parsing parse;

//    std::fstream fileRW = mst->loadFileRW(std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".MST"));
//    std::fstream fileXRF = xrf->loadFile_master(std::string(argv[2]) + '/' + std::string(argv[3]) + std::string(".XRF"));

    switch (parseParams(argc, argv)) {
//        case REMOVE: {
//            removeUsers(fileRW, fileXRF, mstFile, parsedRecords);
//            break;
//        }
        case ERK: {
//            getERK(lastYear - 1, parsedRecords);
            singleRegistrationCard* erk = new singleRegistrationCard(filePathBD, bdName);
            erk->getERK();
            break;
        }
//        case CH_B: {
//            getCountCH_B(parsedRecords);
//            break;
//        }
        case DAILY_STATS: {
            dailyStats* dayStatistics = new dailyStats(filePathBD);
        }
        case ERROR: {

//            fileRW.close();
            return -1;
            break;
        }
    }
//    fileRW.flush();
//    fileRW.close();

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
            return ERK;
        } else if (strncmp(argv[1], "remove", 6) == 0) {
            std::cout << "Удаление читаталей, непосещавших библиотеку более 3х лет" << std::endl;
            return REMOVE;
        } else if (strncmp(argv[1], "daily-stat", 10) == 0) {
            return DAILY_STATS;
        }
        return ERROR;
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

    MST* mst = new MST();
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


