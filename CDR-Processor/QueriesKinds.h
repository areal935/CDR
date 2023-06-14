#pragma once
#include <string>
#include <vector>
#include "DataBase.h"

namespace experis
{

//const std::string KINDS_QUERIES[3] = {"msisdn", "operator", "link"};

struct Query
{
	virtual std::string Response(std::vector<std::string>& query, const DataBase_mt& a_db) const;
};

struct Msisdn : public Query
{
	virtual std::string Response(std::vector<std::string>& query, const DataBase_mt& a_db) const override;
};

struct Operator : public Query
{
	virtual std::string Response(std::vector<std::string>& query, const DataBase_mt& a_db) const override;
};

struct Link : public Query
{
	virtual std::string Response(std::vector<std::string>& query, const DataBase_mt& a_db) const override;
};

struct Start : public Query
{
	virtual std::string Response(std::vector<std::string>& query, const DataBase_mt& a_db) const override;
};

namespace detail
{
	bool IsNumber(const std::string& a_text);

	static const Msisdn MSISDN;
	static const Operator OPERATOR;
	static const Link LINK;
	static const Query INVALID;
	static const Start START;
}

static const std::unordered_map<std::string, const Query*> QUERIES = {
	{"msisdn", &detail::MSISDN},
	{"operator", &detail::OPERATOR},
	{"link", &detail::LINK},
	{"error", &detail::INVALID},
	{"start", &detail::START}
};

}