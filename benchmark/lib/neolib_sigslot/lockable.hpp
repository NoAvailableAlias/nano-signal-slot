// lockable.hpp v2.0
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

#ifdef _WIN32
#include <intrin.h>
#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)
#define MEMORY_BARRIER_ACQUIRE _ReadBarrier();
#define MEMORY_BARRIER_ACQUIRE_DEPENDANT _ReadBarrier();
#define MEMORY_BARRIER_RELEASE _WriteBarrier();
#define MEMORY_BARRIER _ReadWriteBarrier();
#else
#define MEMORY_BARRIER_ACQUIRE
#define MEMORY_BARRIER_ACQUIRE_DEPENDANT
#define MEMORY_BARRIER_RELEASE
#define MEMORY_BARRIER
#endif

namespace neolib
{
	class lockable
	{
		// construction
	public:
		lockable() {}
		lockable(const lockable& aOther) {}
		virtual ~lockable() {}
		lockable& operator=(const lockable& aOther) { return *this; }
		// operations
	public:
		void lock() const { iMutex.lock(); }
		void unlock() const { iMutex.unlock(); }
		// attributes
	private:
		mutable boost::recursive_mutex iMutex;
	};

	class lock
	{
		// construction
	public:
		lock(const lockable& aLockable) : iLockable(aLockable) { iLockable.lock(); }
		~lock() { iLockable.unlock(); }
	private:
		lock& operator=(const lock&);
		// attributes
	private:
		const lockable& iLockable;
	};

	inline void memory_barrier_acquire()
	{
		MEMORY_BARRIER_ACQUIRE
	}

	inline void memory_barrier_acquire_dependant()
	{
		MEMORY_BARRIER_ACQUIRE
	}

	inline void memory_barrier_release()
	{
		MEMORY_BARRIER_RELEASE
	}

	inline void memory_barrier()
	{
		MEMORY_BARRIER
	}
}
