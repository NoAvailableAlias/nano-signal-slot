// timer.h
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
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include "cstdint.hpp"
#include "io_thread.hpp"

namespace neolib
{
	class timer : private boost::noncopyable
	{
		// types
	public:
		struct already_waiting : std::logic_error { already_waiting() : std::logic_error("neolib::timer::already_waiting") {} };
		// construction
	public:
		timer(io_thread& aOwnerThread, uint32_t aDuration_ms = 10, bool aInitialWait = true) : 
			iOwnerThread(aOwnerThread),
			iTimerObject(aOwnerThread.io_service()),
			iDuration_ms(aDuration_ms), 
			iWaiting(false), 
			iCancelling(false)
		{
			if (aInitialWait)
				again();
		}
		timer(const timer& aOther) :
			iOwnerThread(aOther.iOwnerThread),
			iTimerObject(aOther.iOwnerThread.io_service()),
			iDuration_ms(aOther.iDuration_ms), 
			iWaiting(false), 
			iCancelling(false)
		{
			if (aOther.waiting())
				again();
		}
		timer& operator=(const timer& aOther)
		{
			if (waiting())
				cancel();
			if (aOther.waiting())
				again();
			return *this;
		}
		~timer()
		{
			cancel();
		}
		// operations
	public:
		void again()
		{
			if (iWaiting)
				throw already_waiting();
			iTimerObject.expires_from_now(boost::posix_time::milliseconds(iDuration_ms));
			iTimerObject.async_wait(boost::bind(&timer::handler, this, boost::asio::placeholders::error));
			iWaiting = true;
		}
		void again_if()
		{
			if (!iWaiting)
				again();
		}
		void cancel()
		{
			if (!iWaiting)
				return;
			if (!iCancelling)
			{
				iCancelling = true;
				iTimerObject.cancel();
			}
			while(iWaiting)
			{
				iOwnerThread.do_io();
			}
			iCancelling = false;
		}
		bool waiting() const
		{
			return iWaiting;
		}
		bool cancelling() const
		{
			return iCancelling;
		}
		uint32_t duration() const
		{
			return iDuration_ms;
		}
		void set_duration(uint32_t aDuration_ms, bool aEffectiveImmediately = false)
		{
			iDuration_ms = aDuration_ms;
			if (aEffectiveImmediately && waiting())
			{
				cancel();
				again();
			}
		}
		// implementation
	private:
		void handler(const boost::system::error_code& aError)
		{
			iWaiting = false;
			if (!aError && !iCancelling)
				ready();
		}
		virtual void ready() = 0;
		// attributes
	private:
		io_thread& iOwnerThread;
		boost::asio::deadline_timer iTimerObject;
		uint32_t iDuration_ms;
		bool iWaiting;
		bool iCancelling;
	};
}