#include <iostream>
#include <fstream>
#include <string>
#include "QueryGetway.h"
#include "QueryAnswer.h"
#include "CDR.h"
#include "ParserCDR.h"
#include "SimpleNetMT.h"
#include "UrlPathReader.h"

using namespace experis;

void JsonFormat(std::string a_query, std::ostream& a_os)
{
    a_os << "{"
        << "\n\t\"msisdn\": " << a_query 
        << "\n\t\"voice-out\": " << 61523
        << "\n\t\"voice-in\": " << 1343
        << "\n\t\"data-out\": " << 9273
        << "\n\t\"data-in\": " << 8215
        << "\n\t\"sms-out\": " << 54
        << "\n\t\"sms-in\": " << 21
        << "\n}";
}

using connection = std::pair<std::vector<std::string>, std::optional<simplenet::SimpleNetMT::Connection*>>;

void FromNetwork(DataBase_mt& a_db)
{
    simplenet::SimpleNetMT net{4010};

   /* while (true)
    {
        simplenet::SimpleNetMT::Connection c = net.WaitForConnection();
        TaskQueue_mt<connection> Queries{};
        QueryGetway getway{Queries, &c};
        QueryAnswer response{a_db, Queries, &c};
    }*/
}

int main()
{
    DataBase_mt db{};
    TaskQueue_mt<std::vector<std::string>> Cdrs{};
    std::ifstream fileCdr{"CdrFile1.txt"};
    CDR getCDRs{Cdrs, fileCdr};
    ParserCDR parser{db, Cdrs};

    //std::thread network_t{&FromNetwork, std::ref(db)};

    TaskQueue_mt<connection> Queries{};
    QueryGetway getway{Queries, &std::cin};
    QueryAnswer response{db, Queries, &std::cout};

    TaskQueue_mt<connection> QueriesNet{};
    QueryGetway getway1{QueriesNet};
    QueryAnswer response2{db, QueriesNet};

    while(true)
        ;

    //network_t.join();
}