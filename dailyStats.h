//
// Created by chernovda on 29.01.2020.
//
#ifndef IRBIS64_MULTITOOL_DAILYSTATS_H
#define IRBIS64_MULTITOOL_DAILYSTATS_H

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
// Для получения текущей даты
#include <chrono>
#include <iomanip>
#include <sstream>
// **************************
#include "tableViewExport.h"
#include "MST.h"
#include "XRF.h"
#include "fileStarter.h"
#include "parsing.h"
#include "tags.h"

class dailyStats {
public:
    struct structDailyStat {
        // WORK_TYPES
        size_t ITR = 0;
        size_t FINANCE = 0;
        size_t DOCTOR = 0;
        size_t TEACHER = 0;
        size_t LAWYER = 0;
        size_t SPECIAL_PROF = 0;
        size_t WORKER = 0;
        size_t EMPLOYEE = 0;
        size_t OTHER = 0;
        size_t HIGH_EDU = 0;
        size_t MIDDLE_EDU = 0;
        size_t MAIN_EDU = 0;
        size_t BEGINNING_EDU = 0;

        // EDU {
        size_t HIGH = 0;
        size_t MIDDLE = 0;
        size_t BEGINNING_PROF = 0;
        size_t MAIN_FULL = 0;
        size_t MAIN = 0;
        size_t BEGINNING = 0;
        size_t PRESCHOOL = 0;
        size_t DISTANT = 0;

        // AGE {
        size_t BEFORE_14 = 0;
        size_t YOUTH = 0;
        size_t RETIREE = 0;
        size_t DISABLED_ADULT = 0;
        size_t DISABLED_BEFORE_14 = 0;
        size_t DISABLED_YOUTH = 0;
        size_t HOME_SERVICED_DISABLED = 0;
        size_t HOME_SERVICED_RETIREE = 0;
    };

    enum WORK_TYPES {
        NULL_TYPE = 0,
        ITR,
        FINANCE,
        DOCTOR,
        TEACHER,
        LAWYER,
        SPECIAL_PROF,
        WORKER,
        EMPLOYEE,
        OTHER,
        HIGH_EDU,
        MIDDLE_EDU,
        MAIN_EDU,
        BEGINNING_EDU
    };

    enum EDU {
        NULL_EDU = 0,
        HIGH,
        MIDDLE,
        BEGINNING_PROF,
        MAIN_FULL,
        MAIN,
        BEGINNING,
        PRESCHOOL,
        DISTANT
    };

    enum AGE {
        NULL_AGE = 0,
        BEFORE_14,
        YOUTH,
        RETIREE,
        DISABLED_ADULT,
        DISABLED_BEFORE_14,
        DISABLED_YOUTH,
        HOME_SERVICED_DISABLED,
        HOME_SERVICED_RETIREE
    };

    struct userBIO {
        EDU edu;
        WORK_TYPES work;
        std::vector<AGE> age;
    };

    typedef std::map<WORK_TYPES, std::string> mapWorkTypes;
    typedef std::map<AGE, std::string> mapAgeTypes;
    typedef std::map<EDU, std::string> mapEduTypes;


    fileStarter* bdOper;

    explicit dailyStats(const std::string& path);
    virtual ~dailyStats();

    /**
     * Сформировать структуру с необходимыми данными о пользователе
     * @param user
     * @return
     */
    userBIO fillUserData(const MST::userRecord& user);

    WORK_TYPES userWorkType(const std::string& workType);
    EDU userEduType(const std::string& eduType);
    AGE userAgeType(const std::string& ageType);

private:
    mapWorkTypes userWorkTypeList;
    mapAgeTypes userAgeTypeList;
    mapEduTypes userEduTypeList;
    fileStarter::filialName searchingFilial;
    std::string searchingDate;
    std::vector<dailyStats::userBIO> usersForStat;

    std::map<std::string, structDailyStat> dailyStatistics;

public:
    [[nodiscard]] fileStarter::filialName getSearchingFilial();
    void setSearchingFilial(fileStarter::filialName filial);

    [[nodiscard]] const std::string &getSearchingDate() ;
    void setSearchingDate(const std::string &searchingDate);

    /**
     * Проверка на посещение филиала в текущую дату.
     * Дата и филиал берутся из соответствующих приватных переменных
     *
     * @param visitField - массив посещений
     * @return bool
     */
    bool visitedFilial(const std::vector<std::string>& visitField);
private:

    void fillUserWorkTypeList();
    void fillUserEduTypeList();
    void fillUserAgeTypeList();
    static std::string currentData();


    void makeTest();
};


#endif //IRBIS64_MULTITOOL_DAILYSTATS_H
