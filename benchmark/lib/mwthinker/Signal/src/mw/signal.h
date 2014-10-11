#ifndef MW_SIGNAL_H
#define MW_SIGNAL_H

#include "signals/connection.h"

#include <functional>
#include <list>
#include <memory>

namespace mw {

	// Is a class that holds functions. A slot/callbacks class.
	template <class... A>
	class Signal : public signals::SignalInterface {
	public:
		typedef std::function<void(A...)> Callback;

		inline Signal() {
			id_ = 0;
		}

		~Signal() {
			clear();
		}

		signals::Connection connect(Callback callback) {
			ConnectionInfoPtr c = std::make_shared<signals::ConnectionInfo>(++id_, this);
			functions_.push_back(Pair(callback, c));
			return signals::Connection(c);
		}

		inline void operator()(A... a) {
			for (Pair& pair : functions_) {
				pair.callback_(a...);
			}
		}

		inline void clear() {
			for (Pair& pair : functions_) {
				pair.connectionInfo_->signal_ = nullptr;
			}
		}

		inline int size() const {
			functions_.size();
		}

	private:
		typedef std::shared_ptr<signals::ConnectionInfo> ConnectionInfoPtr;

		void disconnect(int id) override {
			functions_.remove_if([&](Pair& pair) {
				if (pair.connectionInfo_->id_ == id) {
					pair.connectionInfo_->signal_ = nullptr;
					return true;
				}
				return false;
			});
		}

		struct Pair {
			Pair(const Callback& callback, const ConnectionInfoPtr& connectionInfo) : callback_(callback), connectionInfo_(connectionInfo) {
			}

			ConnectionInfoPtr connectionInfo_;
			Callback callback_;
		};

		int id_; // The id mapped to last added function.
		std::list<Pair> functions_; // All mapped callbacks.
	};

} // Namespace mw.

#endif // MW_SIGNAL_H
