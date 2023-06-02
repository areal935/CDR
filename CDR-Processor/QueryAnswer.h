#pragma once
#include "TaskQueue.h"
#include "DataBase.h"
#include <thread>
#include <optional>
#include "SimpleNetMT.h"
#include "UrlPathReader.h"

namespace experis
{

class QueryAnswer
{
	using connection = std::pair<std::vector<std::string>, std::optional<simplenet::SimpleNetMT::Connection*>>;
public:
	QueryAnswer() = delete;
	QueryAnswer(const DataBase_mt& a_db, TaskQueue_mt<connection>& a_containerQueries, std::ostream* a_whereOutput);
	QueryAnswer(const DataBase_mt& a_db, TaskQueue_mt<connection>& a_containerQueries);
	QueryAnswer(const QueryAnswer& a_other) = delete;
	QueryAnswer& operator=(const QueryAnswer& a_other) = delete;
	~QueryAnswer() = default;

	void Response();

private:
	const std::string KINDS_QUERIES[3] = {"msisdn", "operator", "link"};

	std::string Answer(std::vector<std::string>& query) const;
	std::string Msisdn(const std::string& a_query) const;
	std::string Operator(const std::string& a_query) const;
	std::string Link(const std::string& a_query, const std::string& a_queryOther) const;
	void NetworkResponse(connection query, const std::string& response) const;

	const DataBase_mt& m_db;
	TaskQueue_mt<connection>& m_containerQueries;
	std::optional<std::ostream*> m_whereOutput;
	std::thread m_consumerQueriesThread;
};

}