// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by chernovda on 19.11.2019.
//

#ifndef IRBIS64BOOKIMPORT_PARSING_H
#define IRBIS64BOOKIMPORT_PARSING_H

#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <locale>

#include "MST.h"

class parsing {
public:

    typedef std::map<std::string, uint32_t> map__registeredFilials;

    parsing();
    explicit parsing(uint32_t);

    bool regLastYears(const int &, const std::vector<std::string>&);
    bool reRegLastYears(const std::vector<std::string>&);
private:
    int curYear;
public:
    [[nodiscard]] int getCurYear() const;
    void setCurYear(int curYear);

    void showUser(const std::map<uint32_t, std::vector<std::string>>&);
    uint32_t getYearNumber(const std::string &date);

    /**
     * 1 параметр - год, за который просходит проверка
     * 2 параметр - массив (вектор) строк, с содержимым 51го (посещения) поля
     * @return массив филиалов
     */
    std::vector<std::string> regFilials(uint32_t, std::vector<std::string>&);

    /**
    * 1 параметр - год, за который просходит проверка
    * 2 параметр - массив (вектор) строк, с содержимым 51го (посещения) поля
    * @return массив филиалов
    */
    std::vector<std::string> regRegFilials(uint32_t, std::vector<std::string>&);

    std::string getFieldIndexValue(MST::fieldIndexes field, const std::string&);


    /**
     * Сравнение, входит ли искомый месяц+год в текущую дату
     * @param yearMonth
     * @param date
     * @return
     */
    bool yearMonthEqual(const std::string& yearMonth, const std::string& date);

    /**
     * Проверка - сдана ли книга
     * @param field
     * @return
     */
    bool bookOnHands(const std::string& field);

    /**
     * Проверка существования элемента в массиве
     * @return
     */
    bool notInArray(std::string, map__registeredFilials &);

private:

    std::locale loc;
    MST* mst;
    std::map<std::string, std::string> record;

    bool getYear(const std::string &date);
    bool toLongOut(const std::string &year);
    bool isYear(const std::string &year);
};


#endif //IRBIS64BOOKIMPORT_PARSING_H
