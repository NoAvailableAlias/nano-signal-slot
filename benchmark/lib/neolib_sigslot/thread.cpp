// thread.cpp - v3.0
/*
 *  Copyright (c) 2012 Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "neolib.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include "thread.hpp"

namespace neolib
{
	thread::thread(const std::string& aName, bool aAttachToCurrentThread) : 
		iName(aName), 
		iUsingExistingThread(aAttachToCurrentThread), 
		iState(ReadyToStart), 
		iBlockedCount(0)
	{
	}

	thread::~thread()
	{
		if (!finished())
			abort();
	}

	const std::string& thread::name() const
	{
		return iName;
	}

	bool thread::using_existing_thread() const
	{
		return iUsingExistingThread;
	}

	void thread::start()
	{
		lock();
		if (started())
		{
			unlock();
			throw thread_already_started();
		}
		try
		{
			iState = Starting;
			if (!iUsingExistingThread)
			{
				thread_object_pointer newThread(new boost::thread(boost::bind(&thread::entry_point, this)));
				iThreadObject = newThread;
				unlock();
			}
			else
			{
				unlock();
				entry_point();
			}
		}
		catch(const std::exception& aException)
		{
			iState = Error;
			unlock();
			std::cerr << std::string("Error starting thread due to exception being thrown (") + aException.what() + ")." << std::endl;
			throw;
		}
		catch(...)
		{
			iState = Error;
			unlock();
			std::cerr << std::string("Error starting thread due to exception of unknown type being thrown.") << std::endl;
			throw;
		}
	}

	void thread::cancel()
	{
		lock();
		if (finished())
		{
			unlock();
			return;
		}
		unlock();
		if (!in())
		{
			abort();
			return;
		}
		bool canCancel = false;
		lock();
		if (started())
		{
			iState = Cancelled;
			canCancel = true;
		}
		unlock();
		if (canCancel)
			throw cancellation();
	}

	void thread::abort(bool aWait)
	{
		lock();
		if (finished())
		{
			unlock();
			return;
		}
		unlock();
		if (in())
		{
			cancel();
			return;
		}
		lock();
		if (started())
		{
			iState = Aborted;
		}
		unlock();
		if (aWait)
			wait();
	}

	void thread::wait() const
	{
		if (!started())
			throw thread_not_started();
		if (in())
			throw cannot_wait_on_self();
		thread_object().join();
	}

	void thread::block()
	{
		assert(in());
		lock();
		++iBlockedCount;
		bool shouldCancel = aborted();
		unlock();
		if (shouldCancel)
			throw cancellation();
	}

	void thread::unblock()
	{
		assert(in());
		lock();
		--iBlockedCount;
		bool shouldCancel = aborted();
		unlock();
		if (shouldCancel)
			throw cancellation();
	}

	bool thread::started() const 
	{ 
		return iState != ReadyToStart; 
	}
	
	bool thread::finished() const 
	{ 
		return iState != ReadyToStart && iState != Starting && iState != Started; 
	}
	
	bool thread::aborted() const 
	{ 
		return iState == Aborted; 
	}
	
	bool thread::cancelled() const 
	{ 
		return iState == Cancelled; 
	}
	
	bool thread::error() const 
	{ 
		return iState == Error; 
	}
	
	thread::id_type thread::id() const 
	{ 
		return iId; 
	}

	bool thread::in() const 
	{
		if (!started())
			throw thread_not_started();
		return boost::this_thread::get_id() == iId;
	}

	bool thread::blocked() const 
	{ 
		return iBlockedCount != 0; 
	}

	bool thread::has_thread_object() const
	{
		return iThreadObject.get() != 0;
	}

	thread::thread_object_type& thread::thread_object() const
	{ 
		if (!has_thread_object()) 
			throw no_thread_object(); 
		return *iThreadObject; 
	}

	void thread::sleep(uint32_t aDelayInMilleseconds)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(aDelayInMilleseconds));
	}

	void thread::entry_point()
	{
		lock();
		iState = Started;
		iId = boost::this_thread::get_id();
		unlock();
		try
		{
			task();
			lock();
			if (iState == Started)
				iState = Finished;
			unlock();
		}
		catch(const std::exception& aException)
		{
			std::cerr << std::string("Thread terminating due to an uncaught exception was being thrown (") + aException.what() + ")." << std::endl;
			throw;
		}
		catch(...)
		{
			std::cerr << "Thread terminating due to an uncaught exception of unknown type being thrown." << std::endl;
			throw;
		}
	}
} // namespace neolib
