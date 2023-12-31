#pragma once
#include "TaskQueue.h"
#include <thread>
#include <optional>
#include "SimpleNetMT.h"
#include "UrlPathReader.h"

namespace experis
{

class QueryGetway
{
	using connection = std::pair<std::vector<std::string>, std::optional<simplenet::SimpleNetMT::Connection*>>;

public:
	QueryGetway() = delete;
	QueryGetway(TaskQueue_mt<connection>& a_containerQueries, std::istream* a_whereInput);
	QueryGetway(TaskQueue_mt<connection>& a_containerQueries, std::shared_ptr<simplenet::SimpleNetMT::Connection> c);
	QueryGetway(const QueryGetway& a_other) = delete;
	QueryGetway(const QueryGetway&& a_other);
	QueryGetway& operator=(const QueryGetway& a_other) = delete;
	~QueryGetway() = default;

	void GetQueries(std::optional<std::shared_ptr<simplenet::SimpleNetMT::Connection>> c);

private:
	TaskQueue_mt<connection>& m_containerQueries;
	std::optional<std::istream*> m_whereInput;
	bool m_fromNetwork;
	mutable std::thread m_producerQueriesThread;
};

}