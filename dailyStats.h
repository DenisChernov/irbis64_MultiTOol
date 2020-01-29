//
// Created by chernovda on 29.01.2020.
//
#ifndef IRBIS64_MULTITOOL_DAILYSTATS_H
#define IRBIS64_MULTITOOL_DAILYSTATS_H

#include <iostream>
#include <string>
#include "tableViewExport.h"
#include "MST.h"
#include "XRF.h"
#include "fileStarter.h"
#include "parsing.h"
#include "tags.h"
#include <map>

class dailyStats {
public:
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

    void fillUserWorkTypeList();
    void fillUserEduTypeList();
    void fillUserAgeTypeList();
};


#endif //IRBIS64_MULTITOOL_DAILYSTATS_H
