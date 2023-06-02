#include "QueryAnswer.h"
#include "HendlerFunction.h"
#include <cassert>
#include <sstream>
#include <ostream>

namespace experis
{

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
			delete query.second.value();
		}
		else
		{
			assert(m_whereOutput.has_value());
			*(this->m_whereOutput.value()) << response;
		}
	}
}

std::string QueryAnswer::Answer(std::vector<std::string>& query) const
{
	std::string response{};
	if(query.at(0) == KINDS_QUERIES[0])
	{
		response = Msisdn(query.at(1));
	}
	else if(query.at(0) == KINDS_QUERIES[1])
	{
		response = Operator(query.at(1));
	}
	else if(query.at(0) == KINDS_QUERIES[2])
	{
		response = Link(query.at(1), query.at(2));
	}
	else
	{
		throw IllegalQueryException{query.at(0), "is illegal query"};
	}
	return response;
}

std::string QueryAnswer::Msisdn(const std::string& a_query) const
{
	std::vector<std::pair<std::string, DATA>> data{};
	MSISDN msisdn = std::stoull(a_query);
	if(!this->m_db.ContainsMsisdn(msisdn))
	{
		return "subscirbe " + std::to_string(msisdn) + " is not exiest in system\n";
	}
	data.push_back({"\"msisdn\"" , msisdn});
	data.push_back({"\"voice-out\"" , this->m_db.GetMsisdn(msisdn, MOC)});
	data.push_back({"\"voice-in\"" , this->m_db.GetMsisdn(msisdn, MTC)});
	data.push_back({"\"data-out\"" , this->m_db.GetMsisdn(msisdn, DO)});
	data.push_back({"\"data-in\"" , this->m_db.GetMsisdn(msisdn, DI)});
	data.push_back({"\"sms-out\"" , this->m_db.GetMsisdn(msisdn, SMS_MO)});
	data.push_back({"\"sms-in\"" , this->m_db.GetMsisdn(msisdn, SMS_MT)});

	return JsonFormat(data);
}

std::string QueryAnswer::Operator(const std::string& a_query) const
{
	std::vector<std::pair<std::string, DATA>> data{};
	IMSI imsi = std::stoi(a_query);
	if(!this->m_db.ContainsImsi(imsi))
	{
		return "subscirbe " + std::to_string(imsi) + " is not exiest in system\n";
	}
	data.push_back({"\"voice-out\"" , this->m_db.GetOperator(imsi, MOC)});
	data.push_back({"\"voice-in\"" , this->m_db.GetOperator(imsi, MTC)});
	data.push_back({"\"sms-out\"" , this->m_db.GetOperator(imsi, SMS_MO)});
	data.push_back({"\"sms-in\"" , this->m_db.GetOperator(imsi, SMS_MT)});

	return JsonFormat(data);
}

std::string ReturnConaction(std::pair<DATA, DATA> a_conection)
{
	std::vector<std::pair<std::string, DATA>> data{};
	data.push_back({"\"voice-call\"" , a_conection.first});
	data.push_back({"\"sms-volume\"" , a_conection.second});
	return JsonFormat(data);
}

std::string ReturnPath(const std::list<MSISDN>& a_path)
{
	std::string path{};
	for(MSISDN msisdn : a_path)
	{
		path += "->" + msisdn;
	}
	path += "\n";
	return path;
}

std::string QueryAnswer::Link(const std::string& a_query, const std::string& a_queryOther) const
{
	MSISDN msisdn1 = std::stoull(a_query);
	if(!this->m_db.ContainsMsisdn(msisdn1))
	{
		return "subscirbe " + std::to_string(msisdn1) + " is not exiest in system\n";
	}
	MSISDN msisdn2 = std::stoull(a_queryOther);
	if(!this->m_db.ContainsLink(msisdn1, msisdn2))
	{
		return "subscirbe " + std::to_string(msisdn2) + " is not exiest in system\n";
	}
	std::pair<DATA, DATA> conection{};
	std::list<MSISDN> path = this->m_db.GetPath(msisdn1, msisdn2, conection);
	if(path.empty() && conection.first == 0 && conection.second == 0)
	{
		return "there are not path between " + std::to_string(msisdn1) + " to " + std::to_string(msisdn2) + "\n";
	}
	return path.empty() ? ReturnConaction(conection) : ReturnPath(path);
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