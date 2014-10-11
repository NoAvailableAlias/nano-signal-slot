#include "connection.h"
#include "signal.h"

namespace mw {

	namespace signals {

		void Connection::disconnect() {
			if (connectionInfo_ && connectionInfo_->signal_ != nullptr) {
				connectionInfo_->signal_->disconnect(connectionInfo_->id_);
			}
		}

		bool Connection::connected() const {
			return connectionInfo_ && connectionInfo_->signal_ != nullptr;
		}

		Connection::Connection(const ConnectionInfoPtr& c) {
			connectionInfo_ = c;
		}

	} // Namespace signals.

} // Namespace mw.
