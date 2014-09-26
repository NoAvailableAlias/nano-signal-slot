// async_slot.hpp
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
#include <set>
#include <deque>
#include <functional>
#include "io_thread.hpp"
#include "timer.hpp"
#include "locking_policy.hpp"
#include "slot_interface.hpp"
#include "signal_interface.hpp"

namespace neolib
{
	template <typename LockingPolicy = locking_policy_shared_mutex>
	class has_async_slots : public async_slot_interface, private LockingPolicy, private neolib::timer
	{
		template <typename Function, std::size_t ParameterCount, typename LockingPolicy2>
		friend class signal_base;
		template <typename Key, typename Function, std::size_t ParameterCount, typename LockingPolicy2>
		friend class signal_with_key_base;
		// types
	private:
		typedef std::set<const signal_interface*> signal_list;
		typedef std::deque<std::function<void()> > event_list;
		// construction
	public:
		has_async_slots(neolib::io_thread& aThread) : 
			neolib::timer(aThread)
		{
		}
		virtual ~has_async_slots()
		{
			typename LockingPolicy::scope_lock sl(*this);
			for (signal_list::iterator i = iSignals.begin(); i != iSignals.end(); ++i)
				(**i).slot_destroyed(*this);
		}
	private:
		has_async_slots();
		has_async_slots(const has_async_slots&);
		// implementation
	private:
		// from async_slot_interface
		virtual void signal_created(signal_interface& aSignal) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			iSignals.insert(&aSignal);
		}
		virtual void signal_destroyed(signal_interface& aSignal) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			signal_list::iterator existingSignal = iSignals.find(&aSignal);
			if (existingSignal != iSignals.end())
				iSignals.erase(existingSignal);
		}
		virtual void add_event(const std::function<void()>& aEvent) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			iEvents.push_back(aEvent);
		}
		// from neolib::timer
		virtual void ready()
		{
			typename LockingPolicy::scope_lock sl(*this);
			while(!iEvents.empty())
			{
				iEvents.front()();
				iEvents.pop_front();
			}
			again();
		}
		// attributes
	private:
		mutable signal_list iSignals;
		mutable event_list iEvents;
	};
}