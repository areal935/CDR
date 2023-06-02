#include "QueryGetway.h"
#include "HendlerFunction.h"
#include <istream>
#include <cassert>
#include <string>

namespace experis
{

QueryGetway::QueryGetway(TaskQueue_mt<connection>& a_containerQueries, std::istream* a_whereInput)
	: m_containerQueries{a_containerQueries}, m_whereInput{a_whereInput}, m_fromNetwork{false}
	, m_producerQueriesThread{&QueryGetway::GetQueries, this}
{
}

QueryGetway::QueryGetway(TaskQueue_mt<connection>& a_containerQueries)
	: m_containerQueries{a_containerQueries}, m_whereInput{}, m_fromNetwork{true}
	, m_producerQueriesThread{&QueryGetway::GetQueries, this}
{
}

void QueryGetway::GetQueries()
{
	if(m_fromNetwork)
	{
		simplenet::SimpleNetMT net{4010};
		while (true)
		{
			simplenet::SimpleNetMT::Connection* c = new simplenet::SimpleNetMT::Connection{net.WaitForConnection()};
			try
			{
				//assert(this->m_network.has_value());
				auto [untrust_query, isHttp] = simplenet::TryReadUrlPath(*c);
				std::vector<std::string> query = Split(untrust_query, {'\\'});
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