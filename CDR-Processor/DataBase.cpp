#include "DataBase.h"
#include <exception>
#include <algorithm>
#include <cassert>
#include <optional>
#include <queue>
#include <list>

namespace experis
{

DataBase_mt::DataBase_mt()
	:m_db{}, m_subscribers{}, m_neighbors{}

{
}

void DataBase_mt::Insert(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum, const UsageType a_usageType, QUANTITY a_quantity)
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	InitialNewKey(a_simId, a_phoneNum);
	this->m_db[a_phoneNum].Add(a_usageType, a_quantity);
}

void DataBase_mt::Insert(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, const UsageType a_usageType, QUANTITY a_quantity)
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	InitialNewKey(a_simId, a_phoneNum);
	this->m_db[a_phoneNum].Add(a_usageType, a_quantity);
	AddNeighbors(a_phoneNum, a_phoneNumOther, a_usageType, a_quantity);
	//this->m_db[a_phoneNum].Add(a_phoneNumOther, a_usageType, a_quantity);
}

QUANTITY DataBase_mt::GetPhoneNumInfo(const PHONE_NUM& a_phoneNum, const UsageType a_usageType) const
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	//return this->m_db[a_phoneNum].Get(a_usageType);
	//return this->m_db.find(a_phoneNum)->second.Get(a_usageType);
	return this->m_db.at(a_phoneNum).Get(a_usageType);
}

QUANTITY DataBase_mt::GetOperator(const SIM_ID& a_simId, const UsageType a_usageType) const
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	QUANTITY toReturn = 0;
	for(PHONE_NUM msisdn : this->m_subscribers.at(a_simId))
	{
		toReturn += this->m_db.at(msisdn).Get(a_usageType);
	}
	return toReturn;
}

auto DataBase_mt::GetPath(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, std::pair<QUANTITY, QUANTITY>& a_link) const -> std::list<PHONE_NUM>
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	if(this->m_neighbors.at(a_phoneNum).count(a_phoneNumOther) == 0 || 
		(this->m_neighbors.at(a_phoneNum).at(a_phoneNumOther).first == 0 && this->m_neighbors.at(a_phoneNum).at(a_phoneNumOther).second== 0))
	{
		return BFS(a_phoneNum, a_phoneNumOther);
	}
	else
	{
		a_link = this->m_neighbors.at(a_phoneNum).at(a_phoneNumOther);
		return {};
	}
}

bool DataBase_mt::ContainsPhoneNum(const PHONE_NUM& a_phoneNum) const
{
	return this->m_db.count(a_phoneNum) > 0;
}

bool DataBase_mt::ContainsSimId(const SIM_ID& a_simId) const
{
	return this->m_subscribers.count(a_simId) > 0;
}

bool DataBase_mt::ContainsLink(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther) const
{
	return this->m_neighbors.at(a_phoneNum).count(a_phoneNumOther) > 0;
}

void DataBase_mt::AddNeighbors(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther, const UsageType a_usageType, QUANTITY a_quantity)
{
	if(this->m_neighbors.count(a_phoneNum) == 0 || this->m_neighbors.at(a_phoneNum).count(a_phoneNumOther) == 0)
	{
		this->m_neighbors[a_phoneNum][a_phoneNumOther] = {0, 0};
	}
	if(a_usageType == MOC || a_usageType == MTC)
	{
		this->m_neighbors.at(a_phoneNum).at(a_phoneNumOther).first += a_quantity;
	}
	else if(a_usageType == SMS_MO || a_usageType == SMS_MT)
	{
		this->m_neighbors.at(a_phoneNum).at(a_phoneNumOther).second += a_quantity;
	}
}

void DataBase_mt::InitialNewKey(const SIM_ID& a_simId, const PHONE_NUM& a_phoneNum)
{
	if(this->m_subscribers.count(a_simId) == 0)
	{
		this->m_subscribers.insert({a_simId , {}});
	}
	if(this->m_db.count(a_phoneNum) == 0)
	{
		this->m_db.insert({a_phoneNum, ContactDetails{a_phoneNum}});
	}
	if(std::count(this->m_subscribers[a_simId].begin(), this->m_subscribers[a_simId].end(), a_phoneNum) == 0)
	{
		this->m_subscribers[a_simId].push_back(a_phoneNum);
	}


}

std::list<PHONE_NUM> Path(const PHONE_NUM& a_phoneNumOther, std::unordered_map<PHONE_NUM, std::optional<PHONE_NUM>>& conectes)
{
	if(conectes.count(a_phoneNumOther) == 0)
	{
		return {};
	}
	std::list<PHONE_NUM> path;
	PHONE_NUM current = a_phoneNumOther;
	while(conectes.count(current) > 0 && conectes.at(current).has_value())
	{
		path.push_front(current);
		current = conectes.at(current).value();
	}
	return path;
}

std::list<PHONE_NUM> DataBase_mt::BFS(const PHONE_NUM& a_phoneNum, const PHONE_NUM& a_phoneNumOther) const
{
	std::unordered_map<PHONE_NUM, std::optional<PHONE_NUM>> conectes;
	conectes[a_phoneNum] = {};
	std::queue<PHONE_NUM> q;
	q.push(a_phoneNum);
	while (!q.empty())
	{
		PHONE_NUM closer = q.front();
		if(this->m_neighbors.count(closer))
		{
			for(auto &msisdn : this->m_neighbors.at(closer))
			{
				if(conectes.count(msisdn.first) == 0)
				{
					conectes[msisdn.first] = closer;
					q.push(msisdn.first);
				}
			}
		}
		q.pop();
	}
	return Path(a_phoneNumOther, conectes);
}

} // experis