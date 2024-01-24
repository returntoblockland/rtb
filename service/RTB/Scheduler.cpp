// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Scheduler.h"

namespace RTB {

Scheduler::Scheduler(size_t numUnmanagedThreads, size_t minExecutorThreads)
{
	size_t numExecutorThreads = std::thread::hardware_concurrency();
	if (numExecutorThreads > numUnmanagedThreads)
		numExecutorThreads -= numUnmanagedThreads;
	numExecutorThreads = std::max(numExecutorThreads, minExecutorThreads);

	m_executor = std::make_unique<tf::Executor>(numExecutorThreads);
}

Scheduler::~Scheduler()
{
	while (m_executor->num_topologies())
		m_executor->wait_for_all();
}

} // namespace RTB
