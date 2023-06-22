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

	void Insert(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum, const UsageType a_usageType, QUANTITY a_quantity);
	void Insert(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, const UsageType a_usageType, QUANTITY a_quantity);
	QUANTITY GetPhoneNumInfo(const PHONE_NUM& a_phoneNum, const UsageType a_usageType) const;

	QUANTITY GetOperator(const SIM_ID& a_simId, const UsageType a_usageType) const;
	std::list<PHONE_NUM> GetPath(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, std::pair<QUANTITY, QUANTITY>& a_link) const;
	bool ContainsPhoneNum(const PHONE_NUM& a_phoneNum) const;
	bool ContainsSimId(const SIM_ID& a_simId) const;
	bool ContainsLink(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther) const;

private:
	void InitialNewKey(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum);
	void AddNeighbors(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, const UsageType a_usageType, QUANTITY a_quantity);
	std::list<PHONE_NUM> BFS(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther) const;

	std::unordered_map<PHONE_NUM, ContactDetails> m_db;
	SUBSCRIBERS m_subscribers;
	LINKED_GRAPH m_neighbors;
	mutable std::mutex m_dbMutex;
};

}