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

void DataBase_mt::Insert(const IMSI& a_imsi, const MSISDN& a_msisdn, const UsageType a_usageType, DATA a_data)
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	InitialNewKey(a_imsi, a_msisdn);
	this->m_db[a_msisdn].Add(a_usageType, a_data);
}

void DataBase_mt::Insert(const IMSI& a_imsi, const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, const UsageType a_usageType, DATA a_data)
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	InitialNewKey(a_imsi, a_msisdn);
	this->m_db[a_msisdn].Add(a_usageType, a_data);
	AddNeighbors(a_msisdn, a_msisdnOther, a_usageType, a_data);
	//this->m_db[a_msisdn].Add(a_msisdnOther, a_usageType, a_data);
}

DATA DataBase_mt::GetMsisdn(const MSISDN& a_msisdn, const UsageType a_usageType) const
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	//return this->m_db[a_msisdn].Get(a_usageType);
	//return this->m_db.find(a_msisdn)->second.Get(a_usageType);
	return this->m_db.at(a_msisdn).Get(a_usageType);
}

DATA DataBase_mt::GetOperator(const IMSI& a_imsi, const UsageType a_usageType) const
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	DATA toReturn = 0;
	for(MSISDN msisdn : this->m_subscribers.at(a_imsi))
	{
		toReturn += this->m_db.at(msisdn).Get(a_usageType);
	}
	return toReturn;
}

auto DataBase_mt::GetPath(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, std::pair<DATA, DATA>& a_link) const -> std::list<MSISDN>
{
	std::unique_lock<std::mutex> dbMutexLocker(this->m_dbMutex);
	if(this->m_neighbors.at(a_msisdn).count(a_msisdnOther) == 0 || 
		(this->m_neighbors.at(a_msisdn).at(a_msisdnOther).first == 0 && this->m_neighbors.at(a_msisdn).at(a_msisdnOther).second== 0))
	{
		return BFS(a_msisdn, a_msisdnOther);
	}
	else
	{
		a_link = this->m_neighbors.at(a_msisdn).at(a_msisdnOther);
		return {};
	}
}

bool DataBase_mt::ContainsMsisdn(const MSISDN& a_msisdn) const
{
	return this->m_db.count(a_msisdn) > 0;
}

bool DataBase_mt::ContainsImsi(const IMSI& a_imsi) const
{
	return this->m_subscribers.count(a_imsi) > 0;
}

bool DataBase_mt::ContainsLink(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther) const
{
	return this->m_neighbors.at(a_msisdn).count(a_msisdnOther) > 0;
}

void DataBase_mt::AddNeighbors(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther, const UsageType a_usageType, DATA a_data)
{
	if(this->m_neighbors.count(a_msisdn) == 0 || this->m_neighbors.at(a_msisdn).count(a_msisdnOther) == 0)
	{
		this->m_neighbors[a_msisdn][a_msisdnOther] = {0, 0};
	}
	if(a_usageType == MOC || a_usageType == MTC)
	{
		this->m_neighbors.at(a_msisdn).at(a_msisdnOther).first += a_data;
	}
	else if(a_usageType == SMS_MO || a_usageType == SMS_MT)
	{
		this->m_neighbors.at(a_msisdn).at(a_msisdnOther).second += a_data;
	}
}

void DataBase_mt::InitialNewKey(const IMSI& a_imsi, const MSISDN& a_msisdn)
{
	if(this->m_subscribers.count(a_imsi) == 0)
	{
		this->m_subscribers.insert({a_imsi , {}});
	}
	if(this->m_db.count(a_msisdn) == 0)
	{
		this->m_db.insert({a_msisdn, ContactDetails{a_msisdn}});
	}
	if(std::count(this->m_subscribers[a_imsi].begin(), this->m_subscribers[a_imsi].end(), a_msisdn) == 0)
	{
		this->m_subscribers[a_imsi].push_back(a_msisdn);
	}
}

std::list<MSISDN> Path(const MSISDN& a_msisdnOther, std::unordered_map<MSISDN, std::optional<MSISDN>>& conectes)
{
	if(conectes.count(a_msisdnOther) == 0)
	{
		return {};
	}
	std::list<MSISDN> path;
	MSISDN current = a_msisdnOther;
	while(conectes.count(current) > 0 && conectes.at(current).has_value())
	{
		path.push_front(current);
		current = conectes.at(current).value();
	}
	return path;
}

std::list<MSISDN> DataBase_mt::BFS(const MSISDN& a_msisdn, const MSISDN& a_msisdnOther) const
{
	std::unordered_map<MSISDN, std::optional<MSISDN>> conectes;
	conectes[a_msisdn] = {};
	std::queue<MSISDN> q;
	q.push(a_msisdn);
	while (!q.empty())
	{
		MSISDN closer = q.front();
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
	return Path(a_msisdnOther, conectes);
}

} // experis