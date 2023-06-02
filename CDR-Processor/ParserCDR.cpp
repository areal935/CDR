#include "ParserCDR.h"
#include <cassert>
#include <string>

namespace experis
{

ParserCDR::ParserCDR(DataBase_mt& a_db, TaskQueue_mt<std::vector<std::string>>& a_containerQueries)
	: m_db{a_db}, m_containerCDRs{a_containerQueries}, m_consumerCDRsThread{&ParserCDR::CdrParser, this}
{
}

void ParserCDR::CdrParser()
{
	while(true)
	{
		std::vector<std::string> cdr{};
		this->m_containerCDRs.Pop(cdr);
		//assert(!cdr.empty());
		if (cdr.empty() || cdr.at(USAGE_TYPE) == "U" || cdr.at(USAGE_TYPE) == "B" || cdr.at(USAGE_TYPE) == "X")
		{
			continue;
		}
		else if(cdr.at(USAGE_TYPE) == "D")
		{
			InsertDataToDb(cdr);
		}
		else if (cdr.at(USAGE_TYPE) == "MOC" || cdr.at(USAGE_TYPE) == "MTC")
		{
			InsertCallToDb(cdr);
		}
		else
		{
			InsertSMSToDb(cdr);
		}
	}
}

void ParserCDR::InsertDataToDb(const std::vector<std::string>& a_cdr)
{
	DATA dataReceived = std::stoi(a_cdr.at(Category::BYTES_RECEIVED));
	DATA dataTransmitted = std::stoi(a_cdr.at(Category::BYTES_TRANSMITTED));
	experis::MSISDN msisdn = std::stoull(a_cdr.at(Category::MSISDN));
	experis::IMSI imsi = std::stoull(a_cdr.at(Category::IMSI).substr(0, 5));
	this->m_db.Insert(imsi, msisdn, DI, dataReceived);
	this->m_db.Insert(imsi, msisdn, DO, dataTransmitted);
}

void ParserCDR::InsertCallToDb(const std::vector<std::string>& a_cdr)
{
	DATA duration = std::stoi(a_cdr.at(Category::DURATION));
	experis::MSISDN msisdn = std::stoull(a_cdr.at(Category::MSISDN));
	experis::MSISDN secondMsisdn = std::stoull(a_cdr.at(Category::SECOND_MSISDN - 2));
	experis::IMSI imsi = std::stoi(a_cdr.at(Category::IMSI).substr(0, 5));
	UsageType type = a_cdr.at(USAGE_TYPE) == "MOC" ? MOC : MTC;

	this->m_db.Insert(imsi, msisdn, secondMsisdn, type, duration);
}

void ParserCDR::InsertSMSToDb(const std::vector<std::string>& a_cdr)
{
	experis::MSISDN msisdn = std::stoull(a_cdr.at(Category::MSISDN));
	experis::MSISDN secondMsisdn = std::stoull(a_cdr.at(Category::SECOND_MSISDN - 2));
	experis::IMSI imsi = std::stoi(a_cdr.at(Category::IMSI).substr(0, 5));
	UsageType type = a_cdr.at(USAGE_TYPE) == "SMS_MO" ? SMS_MO : SMS_MT;
	this->m_db.Insert(imsi, msisdn, secondMsisdn, type, 1);
}

}