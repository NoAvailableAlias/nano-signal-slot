// locking_policy.hpp
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
#include <boost/thread/recursive_mutex.hpp>

namespace neolib
{
	class locking_policy_none
	{
	public:
		class scope_lock
		{
		public:
			scope_lock(const locking_policy_none& aParent)
			{
			};
		};
	public:
		void lock() {}
		void unlock() {}
	};

	class locking_policy_mutex
	{
	public:
		locking_policy_mutex() {}
	private:
		locking_policy_mutex(const locking_policy_mutex&);
	public:
		class scope_lock
		{
		public:
			scope_lock(const locking_policy_mutex& aParent) :
				iParent(aParent)
			{
				iParent.lock();
			};
			~scope_lock()
			{
				iParent.unlock();
			}
		private:
			const locking_policy_mutex& iParent;
		};
	public:
		void lock() const
		{
			iMutex.lock();
		}
		void unlock() const
		{
			iMutex.unlock();
		}
	private:
		mutable boost::recursive_mutex iMutex;
	};

	namespace detail
	{
		template <typename T>
		struct shared_mutex
		{
			static boost::recursive_mutex sMutex;
		};

		template <typename T>
		boost::recursive_mutex shared_mutex<T>::sMutex;
	}

	class locking_policy_shared_mutex
	{
	public:
		class scope_lock
		{
		public:
			scope_lock(const locking_policy_shared_mutex& aParent) :
				iParent(aParent)
			{
				iParent.lock();
			};
			~scope_lock()
			{
				iParent.unlock();
			}
		private:
			const locking_policy_shared_mutex& iParent;
		};
	public:
		void lock() const
		{
			detail::shared_mutex<locking_policy_shared_mutex>::sMutex.lock();
		}
		void unlock() const
		{
			detail::shared_mutex<locking_policy_shared_mutex>::sMutex.unlock();
		}
	};
}