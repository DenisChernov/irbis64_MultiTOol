// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by chernovda on 09.11.18.
//

#ifndef IRBIS64BOOKIMPORT_MST_H
#define IRBIS64BOOKIMPORT_MST_H
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <locale>

class MST {
private:
    //std::string _path;

public:
    typedef std::map<uint32_t, std::vector<std::string>> userRecord;

    struct mainRecord {
        /**
         * 32 CTLMFN – резерв;
         */
        uint32_t CTLMFN:32;

        /**
         * 32 NXTMFN –номер записи файла документов, назначаемый для следующей записи, создаваемой в базе данных;
         */
        uint32_t NXTMFN:32;

        /**
         * 32 NXT_LOW – младшее слово смещения на свободное место в файле; (всегда указывает на конец файла MST)
         */
        uint32_t NXT_LOW:32;

        /**
         * 32 NXT_HIGH – старшее слово смещения на свободное место в файле
         */
        uint32_t NXT_HIGH:32;

        /**
         * 32 MFTYPE – резерв;
         */
        uint32_t MFN_TYPE:32;

        /**
         * 32 RECCNT – резерв;
         */
        uint32_t RECCNT:32;

        /**
         * 32 MFCXX1 – резерв;
         */
        uint32_t MFCXX1:32;

        /**
         * 32 MFCXX2 – резерв;
         */
        uint32_t MFCXX2:32;

        /**
         * 32 MFCXX3 – индикатор блокировки базы данных (0 – нет, >0 – да).
         */
        uint32_t MFCXX3:32;
    } director;
    struct leaderRecord {
        /**
         * 32 MFN 	номер файла документов
         */
        uint32_t MFN:32;
        /**
         * 32 MFRL 	длина записи (в байтах?) (всегда четное число)
         */
        uint32_t MFRL:32;

        /**
         * 32 MFB_LOW 	ссылка назад (младшее слово в полном смешении)
         */
        uint32_t MFB_LOW:32;

        /**
         * 32 MFB_HIGH 	ссылка назад (старшее слово в полном смешении)
         */
        uint32_t MFB_HIGH:32;

        /**
         * 32 BASE 	смещение (базовый адрес) полей переменной длины (это общая часть лидера и справочника записи в байтах)
         */
        uint32_t BASE:32;

        /**
         * 32 NVF 	число полей в записи (т.е. число входов в справочнике)
         */
        uint32_t NVF:32;

        /**
         * 32 STATUS 	индикатор записи
         * BIT_ALL_ZERO (0) – предыдущий вариант записи
         * BIT_LOG_DEL (1) – логически удаленная запись
         * BIT_NOTACT_REC (8)- неактуализированная запись
         * BIT_LAST_REC (32) – последний экземпляр записи
         */
        uint32_t STATUS:32;

        /**
         * 32 VERSION 	номер версии записи
         */
        uint32_t VERSION:32;
    } leader;
    struct referenceRecord {
        /**
         * 32 TAG 	метка поля
         */
        uint32_t TAG:32;

        /**
         * 32 смещение позиции первого символа данного поля в разделе полей переменной длины
         * (в физической записи смещения упорядочены по возрастанию, у первого поля POS = 0)
         */
        uint32_t POS:32;

        /**
         * 32 длина поля в байтах
         */
        uint32_t LEN:32;
    } reference;

    enum fieldIndexes {
            REG_DATE = 'D',
            REG_DIVISION = 'C',
            VISIT_DATE_GET = 'D',
            VISIT_DATE_PUT = 'F',
            VISIT_FILIAL = 'V'
    };

    enum fieldNumbers {
        REG     = 51,
        REREG   = 52
    };

    MST();

    std::ifstream loadFile(const std::string& path);
    std::ofstream loadFileWrite(const std::string& path);

    std::fstream loadFileRW(const std::string& path);

    mainRecord readMainRecord(std::ifstream *file);
    mainRecord readMainRecord_master(std::fstream *file);
    void printMainRecord(const mainRecord& mainRec);

    leaderRecord readLeaderRecord(std::ifstream *file);
    leaderRecord readLeaderRecord_master(std::fstream *file);
    void writeLeaderRecord(const uint32_t& offset, const leaderRecord& leaderRec);
    void printLeaderRecord(const leaderRecord& leaderRec);

    std::vector<referenceRecord> readReference(std::ifstream* file, int countRef);
    std::vector<referenceRecord> readReference_master(std::fstream* file, int countRef);
    void printReferences(const std::vector<referenceRecord> &refs);

    // master

    uint32_t getMaxMFN(const mainRecord &mainRec);

    // leader

    int getNumRefs(const leaderRecord& leaderRec);
    uint32_t getRecordStart(const leaderRecord& leaderRec);
    uint32_t getRecordMFN(const leaderRecord& leaderRec);
    uint32_t getRecordLength(const leaderRecord& leaderRec);

    // references

    uint32_t getTAG(uint32_t tag);
    uint32_t getPOS(uint32_t pos);
    uint32_t getLEN(uint32_t len);

    // field
    std::map<std::string, std::string> record(std::string line);

    /**
     *
     * @param line
     */
    void convertDelimeter(std::string& line);

    std::map<uint32_t, std::vector<std::string>> getUserByOffset(uint32_t, std::ifstream&);
    std::map<uint32_t, std::vector<std::string>> getUserByOffset_master(uint32_t, std::fstream&);

    /**
     * offset, ref file
     */
    void markUserDeleted(uint32_t, std::ifstream&);

    /**
     * uint32_t: смещение
     * fstream: MST файл
     */
    void markUserDeleted_master(uint32_t, std::fstream&);

    /**
     * В качестве параметра выступает массив (вектор) строк, с содержимым 40го (посещения) поля
     * @return
     */
    bool isDebtor(std::vector<std::string>&);



};
#endif //IRBIS64BOOKIMPORT_MST_H
