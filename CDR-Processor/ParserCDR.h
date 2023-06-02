#pragma once
#include "TaskQueue.h"
#include "DataBase.h"
#include <thread>

namespace experis
{

class ParserCDR
{
public:
	ParserCDR() = delete;
	ParserCDR(DataBase_mt& a_db, TaskQueue_mt<std::vector<std::string>>& a_containerQueries);
	ParserCDR(const ParserCDR& a_other) = delete;
	ParserCDR& operator=(const ParserCDR& a_other) = delete;
	~ParserCDR() = default;

	void CdrParser();

private:
	enum Category
	{
		IMSI,
		IMEI,
		USAGE_TYPE,
		MSISDN,
		CALL_DATE,
		CALL_TIME,
		DURATION,
		BYTES_RECEIVED,
		BYTES_TRANSMITTED,
		SECOND_MSISDN,
		CATEGORY_SIZE
	};

	void InsertDataToDb(const std::vector<std::string>& a_cdr);
	void InsertCallToDb(const std::vector<std::string>& a_cdr);
	void InsertSMSToDb(const std::vector<std::string>& a_cdr);

	DataBase_mt& m_db;
	TaskQueue_mt<std::vector<std::string>>& m_containerCDRs;
	std::thread m_consumerCDRsThread;
};

} // experis