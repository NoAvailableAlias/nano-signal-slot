// thread.hpp v3.0
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

#pragma once

#include "neolib.hpp"
#include <stdexcept>
#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include "cstdint.hpp"
#include "lockable.hpp"

namespace neolib
{
	class thread : public lockable, private boost::noncopyable
	{
		// types
	public:
		typedef boost::thread::id id_type;
		typedef boost::thread thread_object_type;
		// exceptions
	public:
		struct thread_not_started : public std::logic_error { thread_not_started() : std::logic_error("thread::thread_not_started") {} };
		struct thread_already_started : public std::logic_error { thread_already_started() : std::logic_error("thread::thread_already_started") {} };
		struct cannot_wait_on_self : public std::logic_error { cannot_wait_on_self() : std::logic_error("thread::cannot_wait_on_self") {} };
		struct no_thread_object : public std::logic_error { no_thread_object() : std::logic_error("thread::no_thread_object") {} };
	private:
		typedef std::auto_ptr<thread_object_type> thread_object_pointer;
		enum state_e { ReadyToStart, Starting, Started, Finished, Aborted, Cancelled, Error };
		struct cancellation {};
		// construction
	public:
		thread(const std::string& aName = "", bool aAttachToCurrentThread = false);
		virtual ~thread();
		// operations
	public:
		const std::string& name() const;
		bool using_existing_thread() const;
		void start();
		void cancel();
		void abort(bool aWait = true);
		void wait() const;
		void block();
		void unblock();
		bool started() const;
		virtual bool finished() const;
		bool aborted() const;
		bool cancelled() const;
		bool error() const;
		id_type id() const;
		bool in() const;
		bool blocked() const;
		bool has_thread_object() const;
		thread_object_type& thread_object() const;
		static void sleep(uint32_t aDelayInMilleseconds);
		// implementation
	private:
		void entry_point();
		virtual void task() = 0;
		// attributes
	private:
		const std::string iName;
		bool iUsingExistingThread;
		volatile state_e iState;
		thread_object_pointer iThreadObject;
		id_type iId;
		volatile std::size_t iBlockedCount;
	};
}
