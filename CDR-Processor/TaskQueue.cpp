//#include "TaskQueue.h"
//
//namespace experis
//{
//
//TaskQueue_mt::TaskQueue_mt()
//	:m_handledQueue{} , m_queueMutex{} , m_queuePossiblyNotEmpty{}
//{
//}
//
//void TaskQueue_mt::Push(std::vector<std::string> a_task)
//{
//	std::unique_lock<std::mutex> queueMutexLocker(m_queueMutex);
//	m_handledQueue.push(a_task);
//	m_queuePossiblyNotEmpty.notify_all();
//}
//
//void TaskQueue_mt::Pop(std::vector<std::string>& a_item)
//{
//	std::unique_lock<std::mutex> queueMutexLocker(m_queueMutex);
//	while(this->m_handledQueue.empty())
//	{
//		m_queuePossiblyNotEmpty.wait(queueMutexLocker);
//	}
//	a_item = m_handledQueue.front();
//	this->m_handledQueue.pop();
//}
//
//} // experis