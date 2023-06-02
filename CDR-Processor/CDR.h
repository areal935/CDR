#pragma once
#include "TaskQueue.h"
#include <thread>

namespace experis
{

class CDR
{
public:
	CDR() = delete;
	CDR(TaskQueue_mt<std::vector<std::string>>& a_containerQueries, std::istream& a_whereInput);
	CDR(const CDR& a_other) = delete;
	CDR& operator=(const CDR& a_other) = delete;
	~CDR() = default;

	void GetCDRs();

private:
	TaskQueue_mt<std::vector<std::string>>& m_containerCDRs;
	std::istream& m_whereInput;
	std::thread m_producerCDRsThread;
};

}