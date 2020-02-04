//
// Created by chernovda on 29.01.2020.
//

#include "dailyStats.h"

dailyStats::dailyStats(const std::string& path) {
    std::cout << "daily" << std::endl;
    fillUserWorkTypeList();
    fillUserAgeTypeList();
    fillUserEduTypeList();

    setSearchingFilial(fileStarter::F_24_IO);
//    setSearchingDate(currentData());
    setSearchingDate("20191020");
    bdOper = new fileStarter(path, fileStarter::READ_ONE);
    makeTest();

//    bdOper = new fileStarter(path, fileStarter::READ_ONE);
//    bdOper->readRecord(41860, bdOper->loadXRF());
//    bdOper->printRecord();
//    if (visitedFilial(bdOper->getUserRecord().at(TAGS::VISIT))) {
//        usersForStat.push_back(fillUserData(bdOper->getUserRecord()));
//    }


//    bdOper = new fileStarter(path, fileStarter::READ_ALL);
//    int id = 0;
//    std::string ids = "[";
//    for (auto record: bdOper->getParsedRecords()) {
//        try {
//            if (visitedFilial(record.at(TAGS::VISIT))) {
//                usersForStat.push_back(fillUserData(record));
//                ids += std::to_string(id) + ",";
//            }
//        }
//        catch (std::out_of_range &e) {
//            /**
//             * Тега 40 нет в записи
//             */
//             // TODO: выводить список записей без посещений
//        }
//        ++id;
//    }
//    ids += "]";
//    std::cout << "count users: " << usersForStat.size() << std::endl;
//    std::cout << ids << std::endl;

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
        return visitor;
    }
    return dailyStats::userBIO();
}

dailyStats::WORK_TYPES dailyStats::userWorkType(const std::string& workType) {
    if (!workType.empty()) {
        for (auto & it : userWorkTypeList) {
            if (std::strncmp(it.second.c_str(), workType.c_str(), strlen(it.second.c_str())) == 0) {
                //std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
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
                //std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
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
                //std::cout << "found: " << it.second << "\nindex: " << it.first << std::endl;
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

void dailyStats::setSearchingFilial(fileStarter::filialName filial) {
    dailyStats::searchingFilial = filial;
}

fileStarter::filialName dailyStats::getSearchingFilial() {
    return searchingFilial;
}

void dailyStats::setSearchingDate(const std::string &date) {
    dailyStats::searchingDate = date;
}

const std::string &dailyStats::getSearchingDate() {
    return searchingDate;
}

std::string dailyStats::currentData() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y%m%d");
    return ss.str();
}

bool dailyStats::visitedFilial(const std::vector<std::string>& visitField) {
    parsing* parse = new parsing();
    for (auto& field: visitField) {
         if (parse->getFieldIndexValue(MST::fieldIndexes::VISIT_DATE_GET, field) == getSearchingDate()) {
             if (parse->getFieldIndexValue(MST::fieldIndexes::VISIT_FILIAL, field) == bdOper->filialTitle(getSearchingFilial())) {
                 free(parse);
                 return true;
             }
         }
    }
    free(parse);
    return false;
}

void dailyStats::makeTest() {
    std::vector<int> ids = {
            205, 244, 423, 431, 644, 649, 719, 736, 864, 939, 991, 1072, 1191, 1203, 1207, 1291, 1313, 1336, 1407, 1419,
            1560, 1596, 1655, 1755, 1765, 2008, 2048, 2526, 2922, 2955, 3555, 3653, 5617, 6206, 6371, 6392, 6512, 8727,
            8813, 9498, 9649, 9876, 9938, 9978, 9987, 9990, 9991, 10030, 10068, 10125, 10142, 10286, 10321, 10351,
            10371, 10611, 10638, 10763, 10772, 10885, 11132, 11154, 11316, 11484, 11555, 11961, 11992, 12091, 12111,
            12141, 12147, 12329, 12615, 12728, 12796, 13092, 13100, 13103, 13393, 13509, 13669, 13783, 13794, 13986,
            14236, 14261, 14262, 14349, 14459, 14481, 14495, 14507, 14512, 14513, 14574, 14578, 14626, 14871, 14872,
            15732, 15811, 16053, 16103, 17083, 17802, 18113, 18159, 18422, 18881, 19534, 19761, 19763, 19782, 19860,
            19903, 20056, 21275, 22208, 22215, 23677, 24047, 24052, 26683, 30050, 30382, 31560, 32538, 32988, 33358,
            33421, 34993, 35949, 35978, 38147, 40065, 40156, 40157, 43757, 46035, 63477, 64025, 64693, 65018
    };
    std::cout << "total: " << ids.size() << std::endl;
    for (int id: ids) {
        bdOper->readRecord(id, bdOper->loadXRF());

        switch (fillUserData(bdOper->getUserRecord()).work) {
            case WORK_TYPES::ITR: { dailyStatistics[getSearchingDate()].ITR++; break;}
            case WORK_TYPES::FINANCE: { dailyStatistics[getSearchingDate()].FINANCE++; break;}
            case WORK_TYPES::DOCTOR: { dailyStatistics[getSearchingDate()].DOCTOR++; break;}
            case WORK_TYPES::TEACHER: { dailyStatistics[getSearchingDate()].TEACHER++; break;}
            case WORK_TYPES::LAWYER: { dailyStatistics[getSearchingDate()].LAWYER++; break;}
            case WORK_TYPES::SPECIAL_PROF: { dailyStatistics[getSearchingDate()].SPECIAL_PROF++; break;}
            case WORK_TYPES::WORKER: { dailyStatistics[getSearchingDate()].WORKER++; break;}
            case WORK_TYPES::EMPLOYEE: { dailyStatistics[getSearchingDate()].EMPLOYEE++; break;}
            case WORK_TYPES::OTHER: { dailyStatistics[getSearchingDate()].OTHER++; break;}
            case WORK_TYPES::HIGH_EDU: { dailyStatistics[getSearchingDate()].HIGH_EDU++; break;}
            case WORK_TYPES::MIDDLE_EDU: { dailyStatistics[getSearchingDate()].MIDDLE_EDU++; break;}
            case WORK_TYPES::MAIN_EDU: { dailyStatistics[getSearchingDate()].MAIN_EDU++; break;}
            case WORK_TYPES::BEGINNING_EDU: { dailyStatistics[getSearchingDate()].BEGINNING_EDU++; break;}
            default: {break;}
        }

        switch (fillUserData(bdOper->getUserRecord()).edu) {
            case EDU::HIGH: { dailyStatistics[getSearchingDate()].HIGH++; break;}
            case EDU::MIDDLE: { dailyStatistics[getSearchingDate()].MIDDLE++; break;}
            case EDU::BEGINNING_PROF: { dailyStatistics[getSearchingDate()].BEGINNING_PROF++; break;}
            case EDU::MAIN_FULL: { dailyStatistics[getSearchingDate()].MAIN_FULL++; break;}
            case EDU::MAIN: { dailyStatistics[getSearchingDate()].MAIN++; break;}
            case EDU::BEGINNING: { dailyStatistics[getSearchingDate()].BEGINNING++; break;}
            case EDU::PRESCHOOL: { dailyStatistics[getSearchingDate()].PRESCHOOL++; break;}
            case EDU::DISTANT: { dailyStatistics[getSearchingDate()].DISTANT++; break;}
            default: {break;}
        }

        for (auto age: fillUserData(bdOper->getUserRecord()).age) {
            switch (age) {
                case AGE::BEFORE_14: {dailyStatistics[getSearchingDate()].BEFORE_14++; break;}
                case AGE::YOUTH: {dailyStatistics[getSearchingDate()].YOUTH++; break;}
                case AGE::RETIREE: {dailyStatistics[getSearchingDate()].RETIREE++; break;}
                case AGE::DISABLED_ADULT: {dailyStatistics[getSearchingDate()].DISABLED_ADULT++; break;}
                case AGE::DISABLED_BEFORE_14: {dailyStatistics[getSearchingDate()].DISABLED_BEFORE_14++; break;}
                case AGE::DISABLED_YOUTH: {dailyStatistics[getSearchingDate()].DISABLED_YOUTH++; break;}
                case AGE::HOME_SERVICED_DISABLED: {dailyStatistics[getSearchingDate()].HOME_SERVICED_DISABLED++; break;}
                case AGE::HOME_SERVICED_RETIREE: {dailyStatistics[getSearchingDate()].HOME_SERVICED_RETIREE++; break;}
                default: {break;}
            }
        }
    }

    auto* twExport = new tableViewExport("/tmp/test.xlsx", tableViewExport::FILE_MODE::OPEN);
    for (const auto& it: dailyStatistics) {
        std::cout << "date: " << it.first << std::endl;
        std::regex re(".*(\\d{2})$");
        std::smatch match;
        if (std::regex_search(it.first, match, re)) {
            std::string day = match[1];
            std::cout << "day: " << day << std::endl;
            twExport->setRow(std::atoi(day.c_str()) + 2);

            twExport->putDayStat("Лист1", "B", it.second.ITR);
            twExport->putDayStat("Лист1", "C", it.second.FINANCE);
            twExport->putDayStat("Лист1", "D", it.second.DOCTOR);
            twExport->putDayStat("Лист1", "E", it.second.TEACHER);
            twExport->putDayStat("Лист1", "F", it.second.LAWYER);
            twExport->putDayStat("Лист1", "G", it.second.OTHER);
            twExport->putDayStat("Лист1", "H", it.second.WORKER);
            twExport->putDayStat("Лист1", "I", it.second.EMPLOYEE);
//            twExport->putDayStat("Лист1", "J", it.second.OTHER);
            twExport->putDayStat("Лист1", "K", it.second.HIGH_EDU);
            twExport->putDayStat("Лист1", "L", it.second.MIDDLE_EDU);
            twExport->putDayStat("Лист1", "M", it.second.MAIN_EDU);
            twExport->putDayStat("Лист1", "N", it.second.BEGINNING_EDU);
            twExport->putDayStat("Лист1", "O", it.second.BEFORE_14);
            twExport->putDayStat("Лист1", "P", it.second.YOUTH);
            twExport->putDayStat("Лист1", "Q", it.second.RETIREE);
            twExport->putDayStat("Лист1", "R", it.second.DISABLED_ADULT);
            twExport->putDayStat("Лист1", "S", it.second.DISABLED_BEFORE_14);
            twExport->putDayStat("Лист1", "T", it.second.DISABLED_YOUTH);
            twExport->putDayStat("Лист1", "U", it.second.HOME_SERVICED_DISABLED);
            twExport->putDayStat("Лист1", "V", it.second.HOME_SERVICED_RETIREE);
//            twExport->putDayStat("Лист1", "W", it.second.FINANCE);
            twExport->putDayStat("Лист1", "X", it.second.HIGH);
            twExport->putDayStat("Лист1", "Y", it.second.MIDDLE);
            twExport->putDayStat("Лист1", "Z", it.second.BEGINNING_PROF);
            twExport->putDayStat("Лист1", "AA", it.second.MAIN_FULL);
            twExport->putDayStat("Лист1", "AB", it.second.MAIN);
            twExport->putDayStat("Лист1", "AC", it.second.BEGINNING);
            twExport->putDayStat("Лист1", "AD", it.second.PRESCHOOL);
            twExport->putDayStat("Лист1", "AE", it.second.DISTANT);
//            twExport->putDayStat("Лист1", "AF", it.second.FINANCE);
        }
    }

    twExport->sheetbookClose();
}






