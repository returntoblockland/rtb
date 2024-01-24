// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/TaskQueue.h"

#include <memory>
#include <thread>

#include <taskflow/taskflow.hpp>

namespace RTB {

class Scheduler
{
public:
	Scheduler(size_t numUnmanagedThreads, size_t minExecutorThreads);
	~Scheduler();

	TaskQueue& MainQueue() { return m_mainQueue; }
	TaskQueue& UIQueue() { return m_uiQueue; }

	template <typename T>
	tf::Future<void> Run(tf::Taskflow&& taskflow, T&& doneCallback);
	tf::Future<void> Run(tf::Taskflow&& taskflow);

private:
	std::unique_ptr<tf::Executor> m_executor;
	TaskQueue m_mainQueue;
	TaskQueue m_uiQueue;
};

template <typename T>
inline tf::Future<void> Scheduler::Run(tf::Taskflow&& taskflow, T&& doneCallback)
{
	return m_executor->run(std::move(taskflow), std::move(doneCallback));
}

inline tf::Future<void> Scheduler::Run(tf::Taskflow&& taskflow)
{
	return m_executor->run(std::move(taskflow));
}

} // namespace RTB
