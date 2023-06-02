#pragma once
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace experis
{

template <typename T>
class TaskQueue_mt
{
public:
	TaskQueue_mt();
	void Push(T a_task);
	void Pop(T& a_item);

private:
	std::queue<T> m_handledQueue;
	mutable std::mutex m_queueMutex;
	mutable std::condition_variable m_queuePossiblyNotEmpty;
};

template <typename T>
TaskQueue_mt<T>::TaskQueue_mt()
	:m_handledQueue{} , m_queueMutex{} , m_queuePossiblyNotEmpty{}
{
}

template <typename T>
void TaskQueue_mt<T>::Push(T a_task)
{
	std::unique_lock<std::mutex> queueMutexLocker(m_queueMutex);
	m_handledQueue.push(a_task);
	m_queuePossiblyNotEmpty.notify_all();
}

template <typename T>
void TaskQueue_mt<T>::Pop(T& a_item)
{
	std::unique_lock<std::mutex> queueMutexLocker(m_queueMutex);
	while(this->m_handledQueue.empty())
	{
		m_queuePossiblyNotEmpty.wait(queueMutexLocker);
	}
	a_item = m_handledQueue.front();
	this->m_handledQueue.pop();
}

} // experis