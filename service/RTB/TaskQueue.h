// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <future>
#include <queue>
#include <mutex>

namespace RTB {

class TaskQueue
{
public:
	using func_t = std::function<void()>;
	using task_t = std::packaged_task<void()>;

	~TaskQueue();

	void PushBack(func_t&& func);
	void PushBack(task_t&& task);
	void PushFront(task_t&& task);
	void PushFront(func_t&& func);
	void Flush();

private:
	struct TaskEntry
	{
		std::unique_ptr<func_t> forgotten;
		std::unique_ptr<task_t> tracked;
	};

	std::deque<TaskEntry> m_tasks;
	std::mutex m_tasksMtx;
};

inline void TaskQueue::PushBack(func_t&& func)
{
	TaskEntry entry { std::make_unique<func_t>(std::move(func)), {} };

	m_tasksMtx.lock();
	m_tasks.push_back(std::move(entry));
	m_tasksMtx.unlock();
}

inline void TaskQueue::PushBack(task_t&& task)
{
	TaskEntry entry { {}, std::make_unique<task_t>(std::move(task)) };

	m_tasksMtx.lock();
	m_tasks.push_back(std::move(entry));
	m_tasksMtx.unlock();
}

inline void TaskQueue::PushFront(func_t&& func)
{
	TaskEntry entry { std::make_unique<func_t>(std::move(func)), {} };

	m_tasksMtx.lock();
	m_tasks.push_front(std::move(entry));
	m_tasksMtx.unlock();
}

inline void TaskQueue::PushFront(task_t&& task)
{
	TaskEntry entry { {}, std::make_unique<task_t>(std::move(task)) };

	m_tasksMtx.lock();
	m_tasks.push_front(std::move(entry));
	m_tasksMtx.unlock();
}

} // namespace RTB

