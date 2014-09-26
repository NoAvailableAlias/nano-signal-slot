// signal.hpp
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

namespace neolib
{
	namespace detail
	{
		using namespace std::placeholders;

		template <std::size_t ParameterCount>
		struct do_bind;
		template <>
		struct do_bind<0>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink);
			}
		};
		template <>
		struct do_bind<1>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
                using std::placeholders::_1;
				aResult = std::bind(aSinkFunction, &aSink, _1);
			}
		};
		template <>
		struct do_bind<2>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2);
			}
		};
		template <>
		struct do_bind<3>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3);
			}
		};
		template <>
		struct do_bind<4>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4);
			}
		};
		template <>
		struct do_bind<5>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5);
			}
		};
		template <>
		struct do_bind<6>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5, _6);
			}
		};
		template <>
		struct do_bind<7>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5, _6, _7);
			}
		};
		template <>
		struct do_bind<8>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5, _6, _7, _8);
			}
		};
		template <>
		struct do_bind<9>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5, _6, _7, _8, _9);
			}
		};
		template <>
		struct do_bind<10>
		{
			template <typename Sink, typename SinkFunction, typename Result>
			void operator()(Sink& aSink, SinkFunction aSinkFunction, Result& aResult)
			{
				aResult = std::bind(aSinkFunction, &aSink, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10);
			}
		};
	}

	template <typename Function, std::size_t ParameterCount, typename LockingPolicy = locking_policy_shared_mutex>
	class signal_base : public signal_interface, protected LockingPolicy
	{
		// types
	public:
		struct slot_already_added : public std::logic_error { slot_already_added() : std::logic_error("signal_base::slot_already_added") {} };
	protected:
		typedef const slot_interface* slot_pointer;
		typedef std::function<Function> function_type;
		typedef std::map<slot_pointer, function_type> slot_list;
		typedef std::auto_ptr<slot_list> slot_list_pointer;
		typedef std::vector<typename slot_list::const_iterator> notification_list;
		typedef std::deque<std::pair<bool*, notification_list> > notification_list_list;
		// construction
	public:
		signal_base() :
			iScope(0)
		{
		}
		virtual ~signal_base()
		{
			typename LockingPolicy::scope_lock sl(*this);
			for (std::size_t i = 0; i != iScope; ++i)
				*(iNotificationListList[i].first) = false;
			if (!has_slots())
				return;
			for (typename slot_list::iterator i = slots().begin(); i != slots().end(); ++i)
				i->first->signal_destroyed(*this);
		}
		// operations
	public:
		// registration
		template <typename Sink, typename SinkFunction>
		void operator()(Sink& aSink, SinkFunction aSinkFunction) 
		{
			typename LockingPolicy::scope_lock sl(*this);
			add_slot(aSink, aSinkFunction);
		}
		void disconnect(slot_interface& aSlot) const
		{
			remove_slot(aSlot);
		}
		// implementation
	protected:
		// from signal_interface
		virtual void slot_destroyed(slot_interface& aSlot) const
		{
			remove_slot(aSlot);
		}
		void remove_slot(slot_interface& aSlot) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!has_slots())
				return;
			typename slot_list::iterator existingSlot = slots().find(&aSlot);
			if (existingSlot != slots().end())
			{
				for (typename notification_list_list::iterator i = iNotificationListList.begin(); i != iNotificationListList.end(); ++i)
					for (typename notification_list::iterator j = i->second.begin(); j != i->second.end();)
						if (*j == existingSlot)
							j = i->second.erase(j);
						else
							++j;
				slots().erase(existingSlot);
			}
		}
		// own
		bool has_slots() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			return iSlots.get() != 0 && !slots().empty();
		}
		slot_list& slots() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (iSlots.get() == 0)
			{
				slot_list_pointer newSlotList(new slot_list);
				iSlots = newSlotList;
			}
			return *iSlots;
		}
		template <typename Sink, typename SinkFunction>
		void add_slot(Sink& aSink, SinkFunction aSinkFunction)
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (slots().find(&aSink) != slots().end())
				throw slot_already_added();
			detail::do_bind<ParameterCount>()(aSink, aSinkFunction, slots()[&aSink]);
			aSink.signal_created(*this);
		}
		notification_list& new_notification_scope(bool& aSignalValidFlag) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (iNotificationListList.size() <= iScope)
				iNotificationListList.resize(iScope + 1);
			iNotificationListList[iScope].first = &aSignalValidFlag;
			iNotificationListList[iScope].second.erase(iNotificationListList[iScope].second.begin(), iNotificationListList[iScope].second.end());
			for (typename slot_list::const_iterator i = slots().begin(); i != slots().end(); ++i)
				iNotificationListList[iScope].second.push_back(i);
			return iNotificationListList[iScope++].second;
		}
		void exit_notification_scope() const
		{
			--iScope;
		}
		// attributes
	private:
		mutable slot_list_pointer iSlots;
		mutable std::size_t iScope;
		mutable notification_list_list iNotificationListList;
	};

	template <typename Function, typename LockingPolicy = locking_policy_shared_mutex>
	class signal;

	template <typename LockingPolicy>
	class signal<void(), LockingPolicy> : public signal_base<void(), 0, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(), 0, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void trigger() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)();
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename LockingPolicy>
	class signal<void(Arg1), LockingPolicy> : public signal_base<void(Arg1), 1, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1), 1, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename LockingPolicy>
	class signal<void(Arg1, Arg2), LockingPolicy> : public signal_base<void(Arg1, Arg2), 2, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2), 2, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3), 3, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3), 3, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy> base_type;
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename LockingPolicy>
	class signal<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy> : public signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy>
	{
		// types
	private:
		typedef signal_base<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9, Arg10 aArg10) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9, aArg10);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Function, std::size_t ParameterCount, typename LockingPolicy = locking_policy_shared_mutex>
	class signal_with_key_base : public signal_interface, protected LockingPolicy
	{
		// types
	public:
		struct slot_already_added : public std::logic_error { slot_already_added() : std::logic_error("signal_with_key_base::slot_already_added") {} };
	protected:
		typedef const slot_interface* slot_pointer;
		typedef std::function<Function> function_type;
		typedef std::map<std::pair<Key, slot_pointer>, function_type> slot_list;
		typedef std::auto_ptr<slot_list> slot_list_pointer;
		typedef std::vector<typename slot_list::const_iterator> notification_list;
		typedef std::deque<std::pair<bool*, notification_list> > notification_list_list;
		// construction
	public:
		signal_with_key_base() :
			iScope(0)
		{
		}
		virtual ~signal_with_key_base()
		{
			typename LockingPolicy::scope_lock sl(*this);
			for (std::size_t i = 0; i != iScope; ++i)
				*(iNotificationListList[i].first) = false;
			if (!has_slots())
				return;
			for (typename slot_list::iterator i = slots().begin(); i != slots().end(); ++i)
				i->first.second->signal_destroyed(*this);
	}
		// operations
	public:
		// registration
		template <typename Sink, typename SinkFunction>
		void operator()(const Key& aKey, Sink& aSink, SinkFunction aSinkFunction) 
		{
			add_slot(aKey, aSink, aSinkFunction);
		}
		void disconnect(const Key& aKey, slot_interface& aSlot) const
		{
			remove_slot(aKey, aSlot);
		}
		// implementation
	protected:
		// from signal_interface
		virtual void slot_destroyed(slot_interface& aSlot) const
		{
			remove_slot(aSlot);
		}
		void remove_slot(slot_interface& aSlot) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!has_slots())
				return;
			for (typename notification_list_list::iterator i = iNotificationListList.begin(); i != iNotificationListList.end(); ++i)
				for (typename notification_list::iterator j = i->second.begin(); j != i->second.end();)
					if ((**j).first.second == &aSlot)
						j = i->second.erase(j);
					else
						++j;
			for (typename slot_list::iterator i = slots().begin(); i != slots().end();)
				if (i->first.second == &aSlot)
					i = slots().erase(i);
				else
					++i;
		}
		void remove_slot(const Key& aKey, slot_interface& aSlot) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!has_slots())
				return;
			for (typename notification_list_list::iterator i = iNotificationListList.begin(); i != iNotificationListList.end(); ++i)
				for (typename notification_list::iterator j = i->second.begin(); j != i->second.end();)
					if ((**j).first.first == aKey && (**j).first.second == &aSlot)
						j = i->second.erase(j);
					else
						++j;
			for (typename slot_list::iterator i = slots().begin(); i != slots().end();)
				if (i->first.first == aKey && i->first.second == &aSlot)
					i = slots().erase(i);
				else
					++i;
		}
		// own
		bool has_slots() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			return iSlots.get() != 0 && !slots().empty();
		}
		slot_list& slots() const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (iSlots.get() == 0)
			{
				slot_list_pointer newSlotList(new slot_list);
				iSlots = newSlotList;
			}
			return *iSlots;
		}
		template <typename Sink, typename SinkFunction>
		void add_slot(const Key& aKey, Sink& aSink, SinkFunction aSinkFunction)
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (slots().find(std::make_pair(aKey, &aSink)) != slots().end())
				throw slot_already_added();
			detail::do_bind<ParameterCount>()(aSink, aSinkFunction, slots()[std::make_pair(aKey, &aSink)]);
			aSink.signal_created(*this);
		}
		notification_list& new_notification_scope(const Key& aKey, bool& aSignalValidFlag) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (iNotificationListList.size() <= iScope)
				iNotificationListList.resize(iScope + 1);
			iNotificationListList[iScope].first = &aSignalValidFlag;
			iNotificationListList[iScope].second.erase(iNotificationListList[iScope].second.begin(), iNotificationListList[iScope].second.end());
			for (typename slot_list::const_iterator i = slots().begin(); i != slots().end(); ++i)
				if (i->first.first == aKey)
					iNotificationListList[iScope].second.push_back(i);
			return iNotificationListList[iScope++].second;
		}
		void exit_notification_scope() const
		{
			--iScope;
		}
		// attributes
	private:
		mutable slot_list_pointer iSlots;
		mutable std::size_t iScope;
		mutable notification_list_list iNotificationListList;
	};

	template <typename Key, typename Function, typename LockingPolicy = locking_policy_shared_mutex>
	class signal_with_key;

	template <typename Key, typename LockingPolicy>
	class signal_with_key<Key, void(), LockingPolicy> : public signal_with_key_base<Key, void(), 0, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(), 0, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)();
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1), LockingPolicy> : public signal_with_key_base<Key, void(Arg1), 1, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1), 1, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2), 2, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2), 2, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3), 3, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3), 3, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4), 4, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5), 5, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 6, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 7, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 8, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 9, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};

	template <typename Key, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename LockingPolicy>
	class signal_with_key<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), LockingPolicy> : public signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy>
	{
		// types
	private:
		typedef signal_with_key_base<Key, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 10, LockingPolicy> base_type;		
		// operations
	public:
		// signaling
		void trigger(const Key& aKey, Arg1 aArg1, Arg2 aArg2, Arg3 aArg3, Arg4 aArg4, Arg5 aArg5, Arg6 aArg6, Arg7 aArg7, Arg8 aArg8, Arg9 aArg9, Arg10 aArg10) const
		{
			typename LockingPolicy::scope_lock sl(*this);
			if (!base_type::has_slots())
				return;
			bool signalValidFlag = true;
			typename base_type::notification_list& list = base_type::new_notification_scope(aKey, signalValidFlag);
			try
			{
				while(!list.empty())
				{
					(list.back()->second)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8, aArg9, aArg10);
					if (!signalValidFlag)
						return;
					list.pop_back();
				}
				base_type::exit_notification_scope();
			}
			catch(...)
			{
				if (signalValidFlag)
					base_type::exit_notification_scope();
				throw;
			}
		}
	};
}
