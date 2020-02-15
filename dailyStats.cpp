//
// Created by chernovda on 29.01.2020.
//
// До модификации:

// real    12m53,125s
// user    12m51,493s
// sys     0m1,502s

// После 1й модификации (установка нового начального индекса парсинга даты + оставнов цикла парсинга, если дата меньше проверяемой):

// real    0m23,435s
// user    0m22,050s
// sys     0m1,381s

//
//


#include "dailyStats.h"

dailyStats::dailyStats(const std::string &path) {
    std::cout << "daily" << std::endl;
    parse = new parsing();

    fillUserWorkTypeList();
    fillUserAgeTypeList();
    fillUserEduTypeList();

    setSearchingFilial(fileStarter::F_24_IO);

    setSearchingDate("20191001");
    bdOper = new fileStarter(path, fileStarter::READ_ONE);
//    bdOper = new fileStarter(path, fileStarter::READ_ALL);
    prepareIDsForParsing(WORK_MODE::MONTH);


//    setSearchingDate(currentData());

//    bdOper = new fileStarter(path, fileStarter::READ_ONE);

//    makeTest();

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

dailyStats::userBIO dailyStats::fillUserData(const MST::userRecord &user) {
    if (!user.empty()) {
        userBIO visitor{};
        for (const auto &tag: user) {
            switch (tag.first) {
                case TAGS::AGE : {
                    for (auto &age: tag.second) {
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

dailyStats::WORK_TYPES dailyStats::userWorkType(const std::string &workType) {
    if (!workType.empty()) {
        for (auto &it : userWorkTypeList) {
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
        for (auto &it : userEduTypeList) {
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
        for (auto &it : userAgeTypeList) {
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

bool dailyStats::visitedFilial(const std::vector<std::string> &visitField) {
//    std::cout << "search in: " << getSearchingDate() << std::endl;

    for (const auto& field: visitField) {
        if (parse->getFieldIndexValue(MST::fieldIndexes::VISIT_DATE_GET, field) == getSearchingDate()) {
//            std::cout << "day equal" << std::endl;
//            std::cout << *field << std::endl;
            if (parse->getFieldIndexValue(MST::fieldIndexes::VISIT_FILIAL, field) == bdOper->filialTitle(getSearchingFilial())) {
//                std::cout << "filial equal" << std::endl;
                return true;
            }
        }
    }
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
    for (int id: ids) {
        bdOper->readRecord(id, bdOper->loadXRF());

        switch (fillUserData(bdOper->getUserRecord()).work) {
            case WORK_TYPES::ITR: {
                dailyStatistics[getSearchingDate()].ITR++;
                break;
            }
            case WORK_TYPES::FINANCE: {
                dailyStatistics[getSearchingDate()].FINANCE++;
                break;
            }
            case WORK_TYPES::DOCTOR: {
                dailyStatistics[getSearchingDate()].DOCTOR++;
                break;
            }
            case WORK_TYPES::TEACHER: {
                dailyStatistics[getSearchingDate()].TEACHER++;
                break;
            }
            case WORK_TYPES::LAWYER: {
                dailyStatistics[getSearchingDate()].LAWYER++;
                break;
            }
            case WORK_TYPES::SPECIAL_PROF: {
                dailyStatistics[getSearchingDate()].SPECIAL_PROF++;
                break;
            }
            case WORK_TYPES::WORKER: {
                dailyStatistics[getSearchingDate()].WORKER++;
                break;
            }
            case WORK_TYPES::EMPLOYEE: {
                dailyStatistics[getSearchingDate()].EMPLOYEE++;
                break;
            }
            case WORK_TYPES::OTHER: {
                dailyStatistics[getSearchingDate()].OTHER++;
                break;
            }
            case WORK_TYPES::HIGH_EDU: {
                dailyStatistics[getSearchingDate()].HIGH_EDU++;
                break;
            }
            case WORK_TYPES::MIDDLE_EDU: {
                dailyStatistics[getSearchingDate()].MIDDLE_EDU++;
                break;
            }
            case WORK_TYPES::MAIN_EDU: {
                dailyStatistics[getSearchingDate()].MAIN_EDU++;
                break;
            }
            case WORK_TYPES::BEGINNING_EDU: {
                dailyStatistics[getSearchingDate()].BEGINNING_EDU++;
                break;
            }
            default: {
                break;
            }
        }

        switch (fillUserData(bdOper->getUserRecord()).edu) {
            case EDU::HIGH: {
                dailyStatistics[getSearchingDate()].HIGH++;
                break;
            }
            case EDU::MIDDLE: {
                dailyStatistics[getSearchingDate()].MIDDLE++;
                break;
            }
            case EDU::BEGINNING_PROF: {
                dailyStatistics[getSearchingDate()].BEGINNING_PROF++;
                break;
            }
            case EDU::MAIN_FULL: {
                dailyStatistics[getSearchingDate()].MAIN_FULL++;
                break;
            }
            case EDU::MAIN: {
                dailyStatistics[getSearchingDate()].MAIN++;
                break;
            }
            case EDU::BEGINNING: {
                dailyStatistics[getSearchingDate()].BEGINNING++;
                break;
            }
            case EDU::PRESCHOOL: {
                dailyStatistics[getSearchingDate()].PRESCHOOL++;
                break;
            }
            case EDU::DISTANT: {
                dailyStatistics[getSearchingDate()].DISTANT++;
                break;
            }
            default: {
                break;
            }
        }

        for (auto age: fillUserData(bdOper->getUserRecord()).age) {
            switch (age) {
                case AGE::BEFORE_14: {
                    dailyStatistics[getSearchingDate()].BEFORE_14++;
                    break;
                }
                case AGE::YOUTH: {
                    dailyStatistics[getSearchingDate()].YOUTH++;
                    break;
                }
                case AGE::RETIREE: {
                    dailyStatistics[getSearchingDate()].RETIREE++;
                    break;
                }
                case AGE::DISABLED_ADULT: {
                    dailyStatistics[getSearchingDate()].DISABLED_ADULT++;
                    break;
                }
                case AGE::DISABLED_BEFORE_14: {
                    dailyStatistics[getSearchingDate()].DISABLED_BEFORE_14++;
                    break;
                }
                case AGE::DISABLED_YOUTH: {
                    dailyStatistics[getSearchingDate()].DISABLED_YOUTH++;
                    break;
                }
                case AGE::HOME_SERVICED_DISABLED: {
                    dailyStatistics[getSearchingDate()].HOME_SERVICED_DISABLED++;
                    break;
                }
                case AGE::HOME_SERVICED_RETIREE: {
                    dailyStatistics[getSearchingDate()].HOME_SERVICED_RETIREE++;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    auto *twExport = new tableViewExport("/tmp/test.xlsx", tableViewExport::FILE_MODE::OPEN);
    for (const auto &it: dailyStatistics) {
//        std::cout << "date: " << it.first << std::endl;
        std::regex re(".*(\\d{2})$");
        std::smatch match;
        if (std::regex_search(it.first, match, re)) {
            std::string day = match[1];
//            std::cout << "day: " << day << std::endl;
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

std::vector<int> dailyStats::prepareIDsForParsing(dailyStats::WORK_MODE workMode) {
    std::vector<int> ids = {
//            431, 622, 939, 1560, 1732, 6561, 9842, 9873, 14230, 14236, 14481, 14931, 23061, 27928,  32495, 36339, 43179, 44133, 46998, 56338
//            27928
            23, 46, 49, 57, 113, 170, 179, 205, 244, 248, 266, 291, 350, 352, 389, 410, 418, 423, 431, 440, 441, 446,
            457, 481, 489, 497, 544, 567, 587, 622, 644, 649, 660, 673, 700, 704, 719, 727, 736, 740, 799, 817, 819,
            864, 897, 909, 915, 939, 944, 991, 1021, 1029, 1044, 1047, 1072, 1087, 1090, 1140, 1162, 1169, 1191, 1203,
            1207, 1210, 1259, 1262, 1280, 1291, 1308, 1313, 1324, 1326, 1327, 1335, 1336, 1357, 1372, 1379, 1407, 1419,
            1452, 1494, 1528, 1535, 1547, 1558, 1560, 1591, 1593, 1596, 1609, 1617, 1629, 1655, 1680, 1720, 1721, 1732,
            1745, 1755, 1765, 1775, 1796, 1803, 1835, 1934, 1997, 2005, 2008, 2016, 2048, 2185, 2206, 2418, 2437, 2458,
            2471, 2526, 2539, 2578, 2584, 2590, 2677, 2798, 2922, 2955, 3029, 3032, 3109, 3120, 3124, 3190, 3224, 3225,
            3233, 3304, 3316, 3370, 3404, 3434, 3504, 3515, 3555, 3594, 3653, 3667, 3766, 3823, 3855, 3894, 3933, 3957,
            4036, 4070, 4090, 4153, 4176, 4235, 4268, 5133, 5147, 5150, 5159, 5179, 5200, 5213, 5220, 5238, 5239, 5269,
            5278, 5358, 5369, 5388, 5407, 5430, 5431, 5435, 5459, 5488, 5491, 5537, 5617, 5669, 5691, 5696, 5717, 5751,
            5797, 5821, 5828, 5832, 6001, 6108, 6142, 6206, 6367, 6371, 6392, 6453, 6466, 6512, 6561, 6681, 6821, 7310,
            7357, 7453, 7909, 8170, 8191, 8266, 8373, 8419, 8685, 8704, 8727, 8794, 8813, 8972, 9033, 9063, 9077, 9221,
            9253, 9308, 9391, 9456, 9489, 9498, 9533, 9636, 9649, 9653, 9671, 9707, 9722, 9788, 9822, 9823, 9824, 9833,
            9835, 9836, 9842, 9844, 9845, 9846, 9847, 9848, 9849, 9850, 9851, 9852, 9853, 9854, 9855, 9857, 9858, 9872,
            9873, 9876, 9878, 9881, 9888, 9889, 9890, 9891, 9892, 9894, 9896, 9903, 9907, 9910, 9919, 9922, 9931, 9932,
            9934, 9937, 9938, 9947, 9949, 9950, 9951, 9952, 9953, 9954, 9964, 9974, 9975, 9976, 9977, 9978, 9985, 9987,
            9990, 9991, 9992, 9994, 9998, 9999, 10004, 10007, 10009, 10017, 10030, 10031, 10032, 10033, 10038, 10044,
            10045, 10046, 10047, 10049, 10051, 10052, 10065, 10068, 10073, 10097, 10107, 10108, 10110, 10116, 10125,
            10127, 10131, 10134, 10142, 10150, 10155, 10165, 10167, 10182, 10184, 10185, 10187, 10256, 10257, 10286,
            10292, 10293, 10316, 10321, 10334, 10347, 10348, 10349, 10351, 10352, 10353, 10364, 10368, 10369, 10370,
            10371, 10373, 10377, 10396, 10399, 10438, 10447, 10459, 10460, 10463, 10466, 10487, 10488, 10499, 10501,
            10505, 10508, 10521, 10524, 10527, 10533, 10542, 10567, 10576, 10606, 10611, 10638, 10640, 10653, 10681,
            10685, 10686, 10695, 10710, 10716, 10725, 10729, 10745, 10763, 10764, 10772, 10813, 10885, 10897, 10904,
            10928, 10953, 10954, 10966, 11001, 11051, 11052, 11114, 11131, 11132, 11152, 11153, 11154, 11162, 11227,
            11243, 11278, 11316, 11373, 11458, 11484, 11485, 11555, 11562, 11594, 11628, 11689, 11840, 11842, 11845,
            11853, 11863, 11884, 11885, 11886, 11887, 11930, 11957, 11960, 11961, 11973, 11992, 12049, 12089, 12091,
            12108, 12109, 12110, 12111, 12141, 12144, 12146, 12147, 12148, 12149, 12150, 12151, 12152, 12191, 12250,
            12260, 12313, 12329, 12331, 12363, 12370, 12427, 12481, 12506, 12569, 12575, 12592, 12594, 12604, 12615,
            12728, 12776, 12796, 12867, 12893, 12902, 12917, 12956, 13092, 13097, 13100, 13103, 13192, 13379, 13393,
            13438, 13461, 13509, 13545, 13609, 13634, 13669, 13700, 13762, 13782, 13783, 13794, 13815, 13845, 13860,
            13864, 13898, 13928, 13950, 13955, 13965, 13986, 14036, 14049, 14097, 14114, 14128, 14133, 14153, 14156,
            14172, 14176, 14177, 14180, 14181, 14193, 14203, 14205, 14208, 14211, 14212, 14217, 14221, 14228, 14229,
            14230, 14231, 14233, 14236, 14244, 14245, 14259, 14261, 14262, 14263, 14264, 14271, 14272, 14296, 14297,
            14298, 14300, 14301, 14303, 14304, 14307, 14314, 14318, 14319, 14322, 14326, 14327, 14330, 14332, 14336,
            14337, 14345, 14347, 14349, 14355, 14357, 14358, 14360, 14361, 14363, 14370, 14371, 14372, 14373, 14382,
            14385, 14386, 14389, 14390, 14398, 14400, 14402, 14403, 14404, 14405, 14409, 14410, 14413, 14414, 14416,
            14417, 14418, 14421, 14428, 14430, 14440, 14443, 14445, 14447, 14449, 14454, 14457, 14458, 14459, 14460,
            14461, 14463, 14477, 14481, 14482, 14495, 14499, 14501, 14502, 14507, 14510, 14511, 14512, 14513, 14523,
            14540, 14544, 14551, 14554, 14556, 14558, 14559, 14561, 14563, 14570, 14574, 14578, 14580, 14585, 14586,
            14588, 14590, 14593, 14595, 14601, 14605, 14613, 14619, 14621, 14623, 14626, 14633, 14634, 14635, 14636,
            14643, 14644, 14646, 14665, 14671, 14673, 14675, 14676, 14678, 14683, 14684, 14686, 14688, 14695, 14703,
            14704, 14705, 14709, 14714, 14717, 14727, 14741, 14742, 14745, 14746, 14749, 14751, 14753, 14754, 14755,
            14756, 14761, 14762, 14764, 14765, 14766, 14767, 14769, 14771, 14772, 14775, 14777, 14780, 14781, 14782,
            14784, 14785, 14797, 14805, 14812, 14821, 14852, 14854, 14855, 14867, 14869, 14870, 14871, 14872, 14876,
            14881, 14882, 14898, 14901, 14905, 14909, 14910, 14911, 14921, 14925, 14931, 14940, 14942, 14944, 14948,
            14949, 14952, 14953, 14955, 14956, 14961, 14972, 14977, 14983, 14986, 14989, 14994, 14995, 14996, 15013,
            15015, 15026, 15027, 15034, 15039, 15087, 15090, 15091, 15094, 15110, 15114, 15123, 15127, 15128, 15144,
            15201, 15210, 15255, 15280, 15290, 15297, 15431, 15471, 15498, 15507, 15530, 15539, 15540, 15542, 15558,
            15585, 15588, 15589, 15627, 15630, 15632, 15633, 15637, 15660, 15706, 15710, 15712, 15732, 15747, 15751,
            15765, 15773, 15782, 15806, 15808, 15811, 15813, 15832, 15841, 15872, 15903, 15939, 15940, 15955, 15972,
            16049, 16053, 16066, 16070, 16084, 16103, 16110, 16163, 16165, 16193, 16196, 16200, 16205, 16218, 16264,
            16269, 16364, 16373, 16401, 16432, 16445, 16462, 16475, 16574, 16592, 16637, 16646, 16647, 16662, 16670,
            16678, 16682, 16720, 16725, 16730, 16758, 16771, 16782, 16813, 16833, 16842, 16856, 16857, 16887, 16889,
            16893, 16894, 16903, 16907, 16911, 16915, 16934, 16978, 16979, 17013, 17064, 17065, 17067, 17080, 17083,
            17114, 17152, 17154, 17179, 17182, 17191, 17204, 17221, 17272, 17291, 17316, 17339, 17340, 17347, 17416,
            17463, 17465, 17503, 17504, 17507, 17514, 17530, 17531, 17540, 17546, 17569, 17570, 17578, 17685, 17738,
            17749, 17802, 17861, 17863, 18087, 18113, 18139, 18158, 18159, 18162, 18163, 18164, 18229, 18337, 18421,
            18422, 18424, 18461, 18462, 18481, 18528, 18536, 18539, 18544, 18549, 18614, 18632, 18636, 18669, 18670,
            18724, 18748, 18837, 18865, 18881, 18934, 18937, 19170, 19207, 19229, 19250, 19261, 19298, 19420, 19422,
            19443, 19444, 19447, 19448, 19449, 19450, 19459, 19534, 19637, 19639, 19640, 19641, 19642, 19643, 19644,
            19695, 19698, 19760, 19761, 19763, 19772, 19777, 19778, 19779, 19782, 19784, 19852, 19860, 19890, 19891,
            19894, 19895, 19901, 19903, 19904, 19906, 19907, 19941, 19973, 19975, 19978, 19982, 20056, 20057, 20058,
            20059, 20060, 20061, 20062, 20063, 20064, 20077, 20082, 20224, 20265, 20266, 20343, 20376, 20462, 20497,
            20519, 20521, 20537, 20593, 20704, 20705, 20706, 20707, 20708, 20709, 20710, 20712, 21275, 21423, 21473,
            21545, 21655, 21800, 21817, 21939, 21947, 22091, 22206, 22207, 22208, 22209, 22210, 22212, 22215, 22216,
            22245, 22279, 22685, 22905, 22946, 23061, 23201, 23219, 23268, 23361, 23367, 23378, 23508, 23677, 23869,
            23883, 24045, 24046, 24047, 24048, 24049, 24050, 24051, 24052, 24053, 24054, 24055, 24056, 24057, 24058,
            24059, 24060, 24061, 24062, 24063, 24064, 24065, 24066, 24067, 24068, 24069, 24089, 24126, 24416, 24425,
            24494, 24971, 25228, 25396, 25404, 25436, 25497, 25498, 25512, 25525, 25552, 25822, 25835, 26163, 26259,
            26368, 26683, 26696, 26881, 27020, 27082, 27140, 27460, 27530, 27533, 27737, 27885, 27928, 28090, 28104,
            28153, 28163, 28164, 28165, 28191, 28218, 28241, 28242, 28261, 28262, 28378, 28458, 28526, 28527, 28528,
            28531, 28566, 28601, 28654, 28683, 28836, 29088, 29112, 29194, 29209, 29234, 29236, 29329, 29343, 29349,
            29439, 29486, 29569, 29581, 29764, 29781, 29820, 29915, 30048, 30050, 30051, 30158, 30220, 30223, 30224,
            30341, 30357, 30363, 30381, 30382, 30400, 30495, 30496, 30500, 30502, 30539, 30547, 30561, 30585, 30641,
            30803, 30806, 31018, 31038, 31082, 31083, 31247, 31262, 31267, 31309, 31341, 31559, 31560, 31769, 31784,
            31824, 31835, 31859, 31871, 31953, 32281, 32495, 32523, 32538, 32556, 32572, 32623, 32898, 32977, 32988,
            32990, 32991, 32992, 33020, 33057, 33080, 33082, 33093, 33168, 33171, 33172, 33249, 33256, 33268, 33285,
            33358, 33404, 33416, 33421, 33423, 33597, 33612, 33719, 33751, 33847, 33936, 33937, 33940, 33975, 33991,
            34045, 34048, 34134, 34178, 34179, 34180, 34181, 34182, 34183, 34187, 34190, 34201, 34348, 34353, 34355,
            34366, 34458, 34571, 34597, 34727, 34739, 34740, 34749, 34757, 34784, 34787, 34835, 34838, 34839, 34853,
            34876, 34941, 34973, 34978, 34986, 34993, 35009, 35012, 35091, 35136, 35137, 35138, 35362, 35405, 35421,
            35429, 35537, 35560, 35649, 35686, 35719, 35790, 35828, 35839, 35946, 35947, 35948, 35949, 35950, 35951,
            35978, 35993, 36020, 36021, 36057, 36059, 36286, 36339, 36369, 36377, 36378, 36450, 36460, 36473, 36477,
            36487, 36548, 36576, 36578, 36654, 36658, 36682, 36781, 36823, 36851, 36852, 36970, 37117, 37184, 37413,
            37558, 37916, 38147, 38531, 38582, 38780, 38781, 39188, 39338, 39499, 40065, 40143, 40148, 40149, 40154,
            40156, 40157, 40164, 40471, 40546, 40579, 40641, 40866, 40887, 41389, 41395, 41443, 41447, 41448, 41451,
            41452, 41454, 41545, 41659, 41735, 41747, 41847, 41908, 41915, 41960, 41961, 41968, 41970, 41971, 42220,
            42253, 42280, 42282, 42285, 42286, 42290, 42411, 42415, 42586, 42587, 42656, 42662, 42729, 42829, 42945,
            42955, 43158, 43179, 43387, 43388, 43395, 43397, 43398, 43400, 43401, 43404, 43757, 43788, 43846, 43848,
            43856, 44045, 44056, 44067, 44074, 44076, 44080, 44081, 44133, 44295, 44316, 44371, 44372, 44435, 44864,
            45005, 45065, 45437, 45477, 45551, 45877, 45878, 46008, 46035, 46214, 46241, 46248, 46469, 46777, 46790,
            46792, 46799, 46805, 46857, 46859, 46881, 46998, 47106, 47259, 47346, 47348, 47368, 47377, 47384, 47386,
            47423, 47449, 47485, 47493, 47495, 47635, 47711, 47723, 47748, 47845, 47848, 48086, 48129, 48224, 48225,
            48279, 48335, 48681, 48694, 48879, 48893, 49078, 49083, 49087, 49096, 49113, 49114, 49115, 49186, 49198,
            49417, 49425, 49760, 49821, 50099, 50107, 50108, 50110, 50111, 50114, 50115, 50136, 50278, 50343, 50504,
            50567, 50592, 50759, 50789, 50790, 50792, 50794, 50798, 50806, 50858, 50914, 50953, 51153, 51156, 51222,
            51340, 51611, 51712, 51948, 51951, 52068, 52072, 52203, 52205, 52412, 52428, 52478, 52579, 52697, 52710,
            52737, 52741, 52742, 52781, 52793, 52959, 53064, 53065, 53095, 53380, 53443, 53616, 53769, 53788, 53823,
            54122, 54350, 54405, 54606, 54841, 54983, 55214, 55886, 55888, 55927, 56274, 56338, 56591, 56701, 56738,
            57070, 57323, 57567, 57684, 57823, 57826, 57868, 58203, 58242, 58411, 58451, 58452, 58755, 59499, 59502,
            59636, 59661, 59662, 59703, 59805, 59964, 59973, 59974, 59999, 60015, 60112, 60267, 60276, 60278, 60284,
            60285, 60286, 60500, 60501, 60809, 60884, 61231, 61327, 61355, 61356, 61358, 61439, 61687, 61844, 62097,
            62113, 62114, 62116, 62117, 62338, 62635, 62880, 63274, 63369, 63477, 64025, 64112, 64113, 64138, 64155,
            64197, 64382, 64394, 64401, 64560, 64561, 64590, 64646, 64655, 64665, 64673, 64686, 64687, 64688, 64689,
            64690, 64691, 64692, 64693, 64709, 64806, 64865, 64910, 64933, 64934, 64959, 65001, 65018, 65053, 65071,
            65074, 65137, 65159, 65208, 65209
    };

    switch (workMode) {
        case WORK_MODE::DAY: {

            break;
        }
        case WORK_MODE::MONTH: {
            std::regex re("^(\\d{6}).*$");
            std::smatch match;
            std::string baseDate;
            if (std::regex_search(getSearchingDate(), match, re)) {
                baseDate = match[1];
            }


            /**
             * Заполнение массива, данными о статистике
             * Перебор месяца с первого числа по 31
             * TODO: определение максимального числа в текущий месяц, чтобы съэкономить времени на парсинг
             *       проводить чтение с конца, до того момента, пока распарсенный день не будет меньше, чем текущий месяц
             */
//            size_t id = 0;
//            std::string ids = "[";
//            int lastID = 0;
            for (auto &id: ids) { // перебираем каждого пользователя
                bdOper->readRecord(id, bdOper->loadXRF());
                std::vector<std::string> visits = prepareFieldVisitsByMonth(getSearchingDate().substr(0, 6));
//                bdOper->printRecord();
//                for (auto record: bdOper->getParsedRecords()) {

                std::cout << "parsing: " << id << std::endl;
                for (size_t day = 31; day >= 1; day--) { // на проверке каждый день в обратном порядке
                    setSearchingDate(baseDate + (day < 10 ? "0" : "") + std::to_string(day));
//                    std::cout << "parsing user for: " << getSearchingDate() << std::endl;
                    try {
                        /**
                         * Если пользователь посещал филиал в данный день, то заносим обсчитываем его
                         */

                        /**
                         * TODO: Возврат из функции:
                         *     -  1. индекс строки, в которой был найден день
                         *     -  2. имеет ли смысл искать дальше, или в оставшихся элементах массива, дата меньше чем первое число месяца
                         */

                        if (visitedFilial(visits)) {
//                            if (getSearchingDate() == "20191021") {
//                                std::cout << "фамилия: " << bdOper->getUserRecord().at(TAGS::LAST_NAME).front().c_str() << std::endl;
//                            }
                            switch (fillUserData(bdOper->getUserRecord()).work) {
                                case WORK_TYPES::ITR: {
                                    dailyStatistics[getSearchingDate()].ITR++;
                                    break;
                                }
                                case WORK_TYPES::FINANCE: {
                                    dailyStatistics[getSearchingDate()].FINANCE++;
                                    break;
                                }
                                case WORK_TYPES::DOCTOR: {
                                    dailyStatistics[getSearchingDate()].DOCTOR++;
                                    break;
                                }
                                case WORK_TYPES::TEACHER: {
                                    dailyStatistics[getSearchingDate()].TEACHER++;
                                    break;
                                }
                                case WORK_TYPES::LAWYER: {
                                    dailyStatistics[getSearchingDate()].LAWYER++;
                                    break;
                                }
                                case WORK_TYPES::SPECIAL_PROF: {
                                    dailyStatistics[getSearchingDate()].SPECIAL_PROF++;
                                    break;
                                }
                                case WORK_TYPES::WORKER: {
                                    dailyStatistics[getSearchingDate()].WORKER++;
                                    break;
                                }
                                case WORK_TYPES::EMPLOYEE: {
                                    dailyStatistics[getSearchingDate()].EMPLOYEE++;
                                    break;
                                }
                                case WORK_TYPES::OTHER: {
                                    dailyStatistics[getSearchingDate()].OTHER++;
                                    break;
                                }
                                case WORK_TYPES::HIGH_EDU: {
                                    dailyStatistics[getSearchingDate()].HIGH_EDU++;
                                    break;
                                }
                                case WORK_TYPES::MIDDLE_EDU: {
                                    dailyStatistics[getSearchingDate()].MIDDLE_EDU++;
                                    break;
                                }
                                case WORK_TYPES::MAIN_EDU: {
                                    dailyStatistics[getSearchingDate()].MAIN_EDU++;
                                    break;
                                }
                                case WORK_TYPES::BEGINNING_EDU: {
                                    dailyStatistics[getSearchingDate()].BEGINNING_EDU++;
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }

                            switch (fillUserData(bdOper->getUserRecord()).edu) {
                                case EDU::HIGH: {
                                    dailyStatistics[getSearchingDate()].HIGH++;
                                    break;
                                }
                                case EDU::MIDDLE: {
                                    dailyStatistics[getSearchingDate()].MIDDLE++;
                                    break;
                                }
                                case EDU::BEGINNING_PROF: {
                                    dailyStatistics[getSearchingDate()].BEGINNING_PROF++;
                                    break;
                                }
                                case EDU::MAIN_FULL: {
                                    dailyStatistics[getSearchingDate()].MAIN_FULL++;
                                    break;
                                }
                                case EDU::MAIN: {
                                    dailyStatistics[getSearchingDate()].MAIN++;
                                    break;
                                }
                                case EDU::BEGINNING: {
                                    dailyStatistics[getSearchingDate()].BEGINNING++;
                                    break;
                                }
                                case EDU::PRESCHOOL: {
                                    dailyStatistics[getSearchingDate()].PRESCHOOL++;
                                    break;
                                }
                                case EDU::DISTANT: {
                                    dailyStatistics[getSearchingDate()].DISTANT++;
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }

                            for (auto age: fillUserData(bdOper->getUserRecord()).age) {
                                switch (age) {
                                    case AGE::BEFORE_14: {
                                        dailyStatistics[getSearchingDate()].BEFORE_14++;
                                        break;
                                    }
                                    case AGE::YOUTH: {
                                        dailyStatistics[getSearchingDate()].YOUTH++;
                                        break;
                                    }
                                    case AGE::RETIREE: {
//                                        if (getSearchingDate() == "20191021") {
//                                            std::cout << "+1 Пенсионер" << std::endl;
//                                        }
                                        dailyStatistics[getSearchingDate()].RETIREE++;
                                        break;
                                    }
                                    case AGE::DISABLED_ADULT: {
                                        dailyStatistics[getSearchingDate()].DISABLED_ADULT++;
                                        break;
                                    }
                                    case AGE::DISABLED_BEFORE_14: {
                                        dailyStatistics[getSearchingDate()].DISABLED_BEFORE_14++;
                                        break;
                                    }
                                    case AGE::DISABLED_YOUTH: {
                                        dailyStatistics[getSearchingDate()].DISABLED_YOUTH++;
                                        break;
                                    }
                                    case AGE::HOME_SERVICED_DISABLED: {
                                        dailyStatistics[getSearchingDate()].HOME_SERVICED_DISABLED++;
                                        break;
                                    }
                                    case AGE::HOME_SERVICED_RETIREE: {
                                        dailyStatistics[getSearchingDate()].HOME_SERVICED_RETIREE++;
                                        break;
                                    }
                                    default: {
                                        break;
                                    }
                                }
                            }
//
//                            if (lastID != id) {
//                                ids += std::to_string(id) + ",";
//                                lastID = id;
//                            }
                        }
                    }
                    catch (std::out_of_range &e) {
                        std::cout << "parse error: " << e.what() << std::endl;
                    }
                }
            }

            exportTableFile("/tmp/test.xlsx");
//            ids += "]";
//            std::cout << ids << std::endl;
            break;
        }
    }

    return std::vector<int>();
}

std::vector<int> dailyStats::prepareIDsForTest_day() {
    std::vector<int> ids;
    int id = 0;
    for (auto record: bdOper->getParsedRecords()) {
        try {
            if (visitedFilial(record.at(TAGS::VISIT))) {
                ids.push_back(id);
            }
        }
        catch (std::out_of_range &e) {
        }
        ++id;
    }

    return ids;
}

void dailyStats::exportTableFile(const std::string &path) {
    auto *twExport = new tableViewExport("/tmp/test.xlsx", tableViewExport::FILE_MODE::OPEN);
    for (const auto &it: dailyStatistics) {
        std::cout << "date: " << it.first << std::endl;
        std::regex re(".*(\\d{2})$");
        std::smatch match;
        if (std::regex_search(it.first, match, re)) {
            std::string day = match[1];
            std::cout << "day: " << day << std::endl;
            twExport->setRow(std::atoi(day.c_str()) + 2);

            twExport->putDayStat("Sheet1", "B", it.second.ITR);
            twExport->putDayStat("Sheet1", "C", it.second.FINANCE);
            twExport->putDayStat("Sheet1", "D", it.second.DOCTOR);
            twExport->putDayStat("Sheet1", "E", it.second.TEACHER);
            twExport->putDayStat("Sheet1", "F", it.second.LAWYER);
            twExport->putDayStat("Sheet1", "G", it.second.OTHER);
            twExport->putDayStat("Sheet1", "H", it.second.WORKER);
            twExport->putDayStat("Sheet1", "I", it.second.EMPLOYEE);
//            twExport->putDayStat("Sheet1", "J", it.second.OTHER);
            twExport->putDayStat("Sheet1", "K", it.second.HIGH_EDU);
            twExport->putDayStat("Sheet1", "L", it.second.MIDDLE_EDU);
            twExport->putDayStat("Sheet1", "M", it.second.MAIN_EDU);
            twExport->putDayStat("Sheet1", "N", it.second.BEGINNING_EDU);
            twExport->putDayStat("Sheet1", "O", it.second.BEFORE_14);
            twExport->putDayStat("Sheet1", "P", it.second.YOUTH);
            twExport->putDayStat("Sheet1", "Q", it.second.RETIREE);
            twExport->putDayStat("Sheet1", "R", it.second.DISABLED_ADULT);
            twExport->putDayStat("Sheet1", "S", it.second.DISABLED_BEFORE_14);
            twExport->putDayStat("Sheet1", "T", it.second.DISABLED_YOUTH);
            twExport->putDayStat("Sheet1", "U", it.second.HOME_SERVICED_DISABLED);
            twExport->putDayStat("Sheet1", "V", it.second.HOME_SERVICED_RETIREE);
//            twExport->putDayStat("Sheet1", "W", it.second.FINANCE);
            twExport->putDayStat("Sheet1", "X", it.second.HIGH);
            twExport->putDayStat("Sheet1", "Y", it.second.MIDDLE);
            twExport->putDayStat("Sheet1", "Z", it.second.BEGINNING_PROF);
            twExport->putDayStat("Sheet1", "AA", it.second.MAIN_FULL);
            twExport->putDayStat("Sheet1", "AB", it.second.MAIN);
            twExport->putDayStat("Sheet1", "AC", it.second.BEGINNING);
            twExport->putDayStat("Sheet1", "AD", it.second.PRESCHOOL);
            twExport->putDayStat("Sheet1", "AE", it.second.DISTANT);
//            twExport->putDayStat("Лист1", "AF", it.second.FINANCE);

            twExport->putDayStat("Sheet1", "AK", it.second.BEFORE_14);
            twExport->putDayStat("Sheet1", "AL", it.second.YOUTH);
            twExport->putDayStat("Sheet1", "AM", it.second.RETIREE);
            twExport->putDayStat("Sheet1", "AN", it.second.DISABLED_ADULT);
            twExport->putDayStat("Sheet1", "AO", it.second.DISABLED_BEFORE_14);
            twExport->putDayStat("Sheet1", "AP", it.second.DISABLED_YOUTH);
            twExport->putDayStat("Sheet1", "AR", it.second.HOME_SERVICED_DISABLED);
            twExport->putDayStat("Sheet1", "AS", it.second.HOME_SERVICED_RETIREE);

        }
    }
}

std::vector<std::string> dailyStats::prepareFieldVisitsByMonth(const std::string& month) {
    std::vector<std::string> allVisits = bdOper->getUserRecord().at(TAGS::VISIT);
    std::vector<std::string> newVisits = std::vector<std::string>();
    for (const auto& field: allVisits) {
        if (parse->yearMonthEqual(month, field)) {
            newVisits.push_back(field);
        }
    }

    return newVisits;
}






