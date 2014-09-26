// async_signal.hpp
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
#include <map>
#include <deque>
#include <vector>
#include <functional>
#include <type_traits>
#include "locking_policy.hpp"
#include "signal_interface.hpp"
#include "slot_interface.hpp"
#include "signal.hpp"

namespace neolib
{
	template <typename Function, typename LockingPolicy = locking_policy_shared_mutex>
	class async_signal;

	template <typename LockingPolicy>
	class async_signal<void(), LockingPolicy> : public signal<void(), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(), 0, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void async_trigger() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(i->second);
		}
	};

	template <typename Arg1, typename LockingPolicy>
	class async_signal<void(Arg1), LockingPolicy> : public signal<void(Arg1), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1), 1, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1));
		}
	};

	template <typename Arg1, typename Arg2, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2), LockingPolicy> : public signal<void(Arg1, Arg2), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2), 2, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3), 3, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9));
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename LockingPolicy>
	class async_signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy> : public signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9, Arg10 aArg10) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				dynamic_cast<const async_slot_interface&>(*i->first).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9, aArg10));
		}
	};

	template <typename Key, typename Function, typename LockingPolicy = locking_policy_shared_mutex>
	class async_signal_with_key;

	template <typename Key, typename LockingPolicy>
	class async_signal_with_key<Key, void(), LockingPolicy> : public signal_with_key<Key, void(), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(), 0, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(i->second);
		}
	};

	template <typename Key, typename Arg1, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1), LockingPolicy> : public signal_with_key<Key, void(Arg1), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1), 1, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2), 2, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3), 3, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9));
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename LockingPolicy>
	class async_signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy> : public signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void async_trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9, Arg10 aArg10) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			for (typename base_type::slot_list::const_iterator i = base_type::slots().begin(); i != base_type::slots().end(); ++i)
				if (i->first.first == aKey)
					dynamic_cast<const async_slot_interface&>(*i->first.second).add_event(std::bind(i->second, aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9, aArg10));
		}
	};
}
