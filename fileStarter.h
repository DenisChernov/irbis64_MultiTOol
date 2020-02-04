//
// Created by chernovda on 26.01.2020.
//

#ifndef IRBIS64_MULTITOOL_FILESTARTER_H
#define IRBIS64_MULTITOOL_FILESTARTER_H

#define MAX_DEBUG_RECORDS 100000

#include <string>
#include <iostream>
#include <cstdint>
#include <stdint-gcc.h>
#include <cstdlib>

#include "MST.h"
#include "XRF.h"

class fileStarter {
private:
    MST* mst;
    XRF* xrf;

    /**
     * Ссылки на записи
     */
    std::vector<XRF::crossLinks> xrfLinks;

    /**
    * Строка записи
    */
    std::vector<std::string> records;

    /**
    * Все записи, разобранные на поля
    */
    std::vector<std::map<uint32_t, std::vector<std::string>>> parsedRecords;


public:
    enum mode {
        READ_ALL, READ_ONE
    };

    enum filialName {
        ALL_OF_THEM = 0,
        F_01_AB, F_01_IO,
        F_02_AB, F_02_IO,
        F_03,
        F_04_SK, F_04_AB,
        F_05_AB, F_05_DO, F_05_IZ,
        F_06_AB, F_06_IO, F_06_OOD,
        F_07_IO, F_07_AB,
        F_08_IO, F_08_OOD,
        F_09,
        F_10,
        F_11_AB, F_11_DO, F_11_IZ,
        F_12_IO, F_12_OOD,
        F_13_AB, F_13_IO,
        F_14_AB, F_14_CZ,
        F_15_AB, F_15_CZ,
        F_22_AB, F_22_CZ, F_22_IO, F_22_OOD,
        F_23_AB, F_23_IZ,
        F_24_IO, F_24_AB,
        F_25_AB, F_25_CZ, F_25_IO,
        CGB_CODI, CGB_AB
    };

    const std::vector<std::map<uint32_t, std::vector<std::string>>> &getParsedRecords() const;

private:

    std::map<filialName, std::string> filials;

    /**
     * Путь до файла MST
     */
    std::string mstFile;

    /**
     * Путь до файла XRF
     */
    std::string xrfFile;

    uint32_t maxMFN = 0;

    std::ifstream fileMST;
    std::ifstream fileCrossLink;
    MST::userRecord userRecord;
public:
    const MST::userRecord &getUserRecord() const;
    void setMaxMfn(const uint32_t &maxMfn);

    const uint32_t &getMaxMFN() const;

    fileStarter(const std::string& filespath, mode operationMode);
    virtual ~fileStarter();

    uint32_t readBD();
    std::vector<XRF::crossLinks> loadXRF();


    /**
     * Возврат выборочной записи из БД.
     * Используется массив перекрестных ссылок, полученный при общем парсинге базы
     *
     * @param mfn
     * @return record
     */
    std::vector<std::string> readRecord(uint32_t mfn);

    /**
     * Заполняется переменная userRecord с записью о читателе.
     * Используется массив перекрестных ссылок, полученный ручной загрузкой
     *
     * @param mfn
     * @param links
     * @return
     */
    void readRecord(uint32_t mfn, std::vector<XRF::crossLinks> links);

    /**
     * Вывод в консоль записи содержащейся в userRecord
     */
    void printRecord();

    std::string userCategory(std::string);

    void associateFilials(std::string filepath);
    std::string filialTitle(filialName);
};


#endif //IRBIS64_MULTITOOL_FILESTARTER_H
