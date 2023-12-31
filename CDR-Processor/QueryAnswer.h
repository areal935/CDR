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
	QueryAnswer(const QueryAnswer&& a_other);
	QueryAnswer& operator=(const QueryAnswer& a_other) = delete;
	~QueryAnswer() = default;

	void Response();

private:

	std::string Answer(std::vector<std::string>& a_query) const;
	void NetworkResponse(connection query, const std::string& response) const;

	const DataBase_mt& m_db;
	TaskQueue_mt<connection>& m_containerQueries;
	std::optional<std::ostream*> m_whereOutput;
	mutable std::thread m_consumerQueriesThread;
};

}