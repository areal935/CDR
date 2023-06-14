#include "QueryAnswer.h"
#include "HendlerFunction.h"
#include "QueriesKinds.h"
#include <cassert>
#include <sstream>
#include <ostream>

namespace experis
{

const std::string KINDS_QUERIES[3] = {"msisdn", "operator", "link"};

QueryAnswer::QueryAnswer(const DataBase_mt& a_db, TaskQueue_mt<connection>& a_containerQueries, std::ostream* a_whereOutput)
	: m_db{a_db}, m_containerQueries{a_containerQueries}
	, m_whereOutput{a_whereOutput}, m_consumerQueriesThread{&QueryAnswer::Response, this}
{
} 

QueryAnswer::QueryAnswer(const DataBase_mt& a_db, TaskQueue_mt<connection>& a_containerQueries)
	: m_db{a_db}, m_containerQueries{a_containerQueries}
	, m_whereOutput{}, m_consumerQueriesThread{&QueryAnswer::Response, this}
{
}

QueryAnswer::QueryAnswer(const QueryAnswer&& a_other)
	: m_db{a_other.m_db}, m_containerQueries{a_other.m_containerQueries}
	, m_whereOutput{}, m_consumerQueriesThread{}
{
	m_consumerQueriesThread.swap(a_other.m_consumerQueriesThread);
}

struct IllegalQueryException : public std::exception
{
	IllegalQueryException(const std::string a_query, const char* a_mes) : m_query{a_query}, m_mes{a_mes} {}

private:
	const std::string m_query;
	const char* m_mes;
};

void QueryAnswer::Response()
{
	while(true)
	{
		/*std::vector<std::string> query{};
		std::optional<simplenet::SimpleNetMT::Connection*> socket;*/
		connection query;
		this->m_containerQueries.Pop(query);
		std::string response = Answer(query.first);

		if(query.second.has_value())
		{
			assert(!m_whereOutput.has_value());
			NetworkResponse(query, response);
			//delete query.second.value();
		}
		else
		{
			assert(m_whereOutput.has_value());
			*(this->m_whereOutput.value()) << response;
		}
	}
}

std::string QueryAnswer::Answer(std::vector<std::string>& a_query) const
{
	std::string query = experis::QUERIES.count(a_query.at(0)) > 0 ? a_query.at(0) : "error";
	return experis::QUERIES.at(query)->Response(a_query, this->m_db);
}

void QueryAnswer::NetworkResponse(connection query, const std::string& response) const
{
	if(query.first.back() == "Y")
	{
		simplenet::EmitHttpResponse(*(query.second.value()), response);
	}
	else
	{
		query.second.value()->Write(response);
	}
}

}