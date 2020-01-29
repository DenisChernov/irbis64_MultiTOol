//
// Created by chernovda on 29.01.2020.
//

#include "dailyStats.h"

dailyStats::dailyStats(const std::string& path) {
    std::cout << "daily" << std::endl;
    fillUserWorkTypeList();
    fillUserAgeTypeList();
    fillUserEduTypeList();

    bdOper = new fileStarter(path, fileStarter::READ_ONE);
    bdOper->readRecord(1, bdOper->loadXRF());
    bdOper->printRecord();

    fillUserData(bdOper->getUserRecord());
}

dailyStats::~dailyStats() = default;

dailyStats::userBIO dailyStats::fillUserData(const MST::userRecord& user) {
    if (!user.empty()) {
        userBIO visitor{};

        for (const auto& tag: user) {
            switch (tag.first) {
                case TAGS::AGE : {
                    for (auto& age: tag.second) {
                        visitor.age.push_back(userAgeType(age));
                    }
                    break;
                }
                case TAGS::WORK_TYPE: {
                    visitor.work = userWorkType(tag.second.front());
                    break;
                }
                case TAGS::EDU: {
                    visitor.edu = userEduType(tag.second.front());
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return dailyStats::userBIO();
}

dailyStats::WORK_TYPES dailyStats::userWorkType(const std::string& workType) {
    if (!workType.empty()) {
        for (auto & it : userWorkTypeList) {
            if (std::strncmp(it.second.c_str(), workType.c_str(), strlen(it.second.c_str())) == 0) {
                std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
                return it.first;
            }
        }
    }

    return WORK_TYPES::NULL_TYPE;
}

dailyStats::EDU dailyStats::userEduType(const std::string &eduType) {
    if (!eduType.empty()) {
        for (auto & it : userEduTypeList) {
            if (std::strncmp(it.second.c_str(), eduType.c_str(), strlen(it.second.c_str())) == 0) {
                std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
                return it.first;
            }
        }
    }

    return EDU::NULL_EDU;
}

dailyStats::AGE dailyStats::userAgeType(const std::string &ageType) {
    if (!ageType.empty()) {
        for (auto & it : userAgeTypeList) {
            if (std::strncmp(it.second.c_str(), ageType.c_str(), strlen(it.second.c_str())) == 0) {
                std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
                return it.first;
            }
        }
    }

    return AGE::NULL_AGE;
}

void dailyStats::fillUserWorkTypeList() {
    userWorkTypeList.insert(std::pair(WORK_TYPES::ITR, "ИТР"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::FINANCE, "Планово-финансовые работники, экономисты"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::DOCTOR, "Врачи, ср. мед. персонал"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::TEACHER, "Преподаватели, воспитатели"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::LAWYER, "Юристы"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::SPECIAL_PROF, "Спец. др. профессий"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::WORKER, "Рабочие"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::EMPLOYEE, "Служащие"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::OTHER, "Прочие"));
    userWorkTypeList.insert(std::pair(WORK_TYPES::HIGH_EDU, "Учащиеся высших проф."));
    userWorkTypeList.insert(std::pair(WORK_TYPES::MIDDLE_EDU, "Учащиеся средних проф."));
    userWorkTypeList.insert(std::pair(WORK_TYPES::MAIN_EDU, "Учащиеся общеобраз."));
    userWorkTypeList.insert(std::pair(WORK_TYPES::BEGINNING_EDU, "Учащиеся нач. проф."));
}

void dailyStats::fillUserEduTypeList() {
    userEduTypeList.insert(std::pair(EDU::HIGH, "высшее проф."));
    userEduTypeList.insert(std::pair(EDU::MIDDLE, "среднее проф."));
    userEduTypeList.insert(std::pair(EDU::BEGINNING_PROF, "нач проф."));
    userEduTypeList.insert(std::pair(EDU::MAIN_FULL, "среднее (полное) общее"));
    userEduTypeList.insert(std::pair(EDU::MAIN, "осн. общее"));
    userEduTypeList.insert(std::pair(EDU::BEGINNING, "начальное"));
    userEduTypeList.insert(std::pair(EDU::PRESCHOOL, "дошкольники"));
    userEduTypeList.insert(std::pair(EDU::DISTANT, "з/о"));
}

void dailyStats::fillUserAgeTypeList() {
    userAgeTypeList.insert(std::pair(AGE::BEFORE_14, "Дети до 14 лет вкл"));
    userAgeTypeList.insert(std::pair(AGE::YOUTH, "Молодежь 15-30 лет вкл"));
    userAgeTypeList.insert(std::pair(AGE::RETIREE, "Пенсионеры"));
    userAgeTypeList.insert(std::pair(AGE::DISABLED_ADULT, "Взрослые инвалиды"));
    userAgeTypeList.insert(std::pair(AGE::DISABLED_BEFORE_14, "Инвалиды дети до 14 л."));
    userAgeTypeList.insert(std::pair(AGE::DISABLED_YOUTH, "Инвалиды 15-30 лет вкл."));
    userAgeTypeList.insert(std::pair(AGE::HOME_SERVICED_DISABLED, "внестационарно инвалиды"));
    userAgeTypeList.insert(std::pair(AGE::HOME_SERVICED_RETIREE, "внестационарно пенсионеры"));
}


