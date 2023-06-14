#include <iostream>
#include <fstream>
#include <string>
#include <list>
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

    std::vector<std::shared_ptr<simplenet::SimpleNetMT::Connection>> connected;
    std::list<TaskQueue_mt<connection>> queues;
    std::list<QueryGetway> getwayQuerys;
    std::list<QueryAnswer> AnswerQuerys;

    while (true)
    {
        std::shared_ptr<simplenet::SimpleNetMT::Connection> c = std::make_shared<simplenet::SimpleNetMT::Connection>(net.WaitForConnection());
        connected.push_back(c);
        //TaskQueue_mt<connection> Queries{};
        queues.push_back({});
        getwayQuerys.push_back({queues.back(), c});
        AnswerQuerys.push_back({a_db, queues.back()});
        /*QueryGetway getway{Queries, c};
        QueryAnswer response{a_db, Queries};*/
    }
}

int main()
{
    DataBase_mt db{};
    TaskQueue_mt<std::vector<std::string>> Cdrs{};
    std::ifstream fileCdr{"CdrFile1.txt"};
    CDR getCDRs{Cdrs, fileCdr};
    ParserCDR parser{db, Cdrs};

    std::thread network_t{&FromNetwork, std::ref(db)};

    TaskQueue_mt<connection> Queries{};
    QueryGetway getway{Queries, &std::cin};
    QueryAnswer response{db, Queries, &std::cout};

    /*TaskQueue_mt<connection> QueriesNet{};
    QueryGetway getway1{QueriesNet};
    QueryAnswer response2{db, QueriesNet};*/

    /*while(true)
        ;*/

    network_t.join();
}