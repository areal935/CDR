#include "QueryGetway.h"
#include "HendlerFunction.h"
#include <istream>
#include <cassert>
#include <string>

namespace experis
{

QueryGetway::QueryGetway(TaskQueue_mt<connection>& a_containerQueries, std::istream* a_whereInput)
	: m_containerQueries{a_containerQueries}, m_whereInput{a_whereInput}, m_fromNetwork{false}
	, m_producerQueriesThread{&QueryGetway::GetQueries, this, std::nullopt}
{
}

QueryGetway::QueryGetway(TaskQueue_mt<connection>& a_containerQueries, std::shared_ptr<simplenet::SimpleNetMT::Connection> c)
	: m_containerQueries{a_containerQueries}, m_whereInput{}, m_fromNetwork{true}
	, m_producerQueriesThread{&QueryGetway::GetQueries, this, c}
{
}

QueryGetway::QueryGetway(const QueryGetway&& a_other)
	: m_containerQueries{a_other.m_containerQueries}, m_whereInput{}, m_fromNetwork{true}
	, m_producerQueriesThread{}
{
	m_producerQueriesThread.swap(a_other.m_producerQueriesThread);
}

void QueryGetway::GetQueries(std::optional<std::shared_ptr<simplenet::SimpleNetMT::Connection>> a_connect)
{
	if(m_fromNetwork)
	{
		assert(a_connect.has_value());
		simplenet::SimpleNetMT::Connection* c = a_connect.value().get();
		//simplenet::SimpleNetMT net{4010};
		while (true)
		{
			try
			{
				//assert(this->m_network.has_value());
				auto [untrust_query, isHttp] = simplenet::TryReadUrlPath(*c);
				std::vector<std::string> query = Split(untrust_query, {'\\', '/'});
				if (isHttp)
				{
					query.push_back("Y");
				}
				else
				{
					query.push_back("N");
				}
				this->m_containerQueries.Push({query, c});
			}
			catch (const simplenet::SimpleNetMT::ConnectionClosed&)
			{
				break;
			}
		}
	}
	else
	{
		while (true)
		{
			assert(m_whereInput.has_value());
			std::string untrust_query = "";
			std::getline(*(m_whereInput.value()), untrust_query);
			std::vector<std::string> query = Split(untrust_query, {'\\'});
			this->m_containerQueries.Push({query, std::nullopt});
		}
	}
}

}