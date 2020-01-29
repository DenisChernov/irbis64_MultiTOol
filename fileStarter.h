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

    const std::vector<std::map<uint32_t, std::vector<std::string>>> &getParsedRecords() const;

private:

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

public:
    void setMaxMfn(const uint32_t &maxMfn);

    const uint32_t &getMaxMFN() const;

public:

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
};


#endif //IRBIS64_MULTITOOL_FILESTARTER_H
