#pragma once
#include <vector>
#include <string>
#include <mutex>
#include "UsingDB.h"

namespace experis
{

class DataBase_mt
{

public:
	DataBase_mt();

	void Insert(const IMSI& a_imsi, const MSISDN& a_msisdn, const UsageType a_usageType, DATA a_data);
	void Insert(const IMSI& a_imsi, const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, const UsageType a_usageType, DATA a_data);
	DATA GetMsisdn(const MSISDN& a_msisdn, const UsageType a_usageType) const;
	DATA GetOperator(const IMSI& a_imsi, const UsageType a_usageType) const;
	std::list<MSISDN> GetPath(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, std::pair<DATA, DATA>& a_link) const;
	bool ContainsMsisdn(const MSISDN& a_msisdn) const;
	bool ContainsImsi(const IMSI& a_imsi) const;
	bool ContainsLink(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther) const;

private:
	void InitialNewKey(const IMSI& a_imsi, const MSISDN& a_msisdn);
	void AddNeighbors(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, const UsageType a_usageType, DATA a_data);
	std::list<MSISDN> BFS(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther) const;

	DB m_db;
	SUBSCRIBERS m_subscribers;
	LINKED_GRAPH m_neighbors;
	mutable std::mutex m_dbMutex;
};

}