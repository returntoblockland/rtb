// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TaskQueue.h"

namespace RTB {

TaskQueue::~TaskQueue()
{
	Flush();
}

void TaskQueue::Flush()
{
	TaskEntry entry;

	for (;;)
	{
		m_tasksMtx.lock();

		if (!m_tasks.empty())
		{
			entry = std::move(m_tasks.front());
			m_tasks.pop_front();
		}

		m_tasksMtx.unlock();

		if (entry.forgotten)
		{
			(*entry.forgotten)();
			entry.forgotten.reset();
		}
		else if (entry.tracked)
		{
			(*entry.tracked)();
			entry.tracked.reset();
		}
		else
		{
			break;
		}
	}
}

} // namespace RTB
