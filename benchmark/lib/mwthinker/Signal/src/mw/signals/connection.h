#ifndef MW_SIGNALS_CONNECTION_H
#define MW_SIGNALS_CONNECTION_H

#include <memory>

namespace mw {

	template<class... A>
	class Signal;

	namespace signals {

		class Connection;

		// Is used by mw::signal. Is not to be used elsewhere.
		class SignalInterface {
		protected:
			friend class Connection;

			inline SignalInterface() {
			}

			inline ~SignalInterface() {
			}

		private:
			SignalInterface(const SignalInterface&) = delete;
			SignalInterface& operator=(const SignalInterface& SignalInterface) = delete;

			virtual void disconnect(int id) = 0;
		};

		// Used internally in mw::Signal to remember a connection made.
		struct ConnectionInfo {
			inline ConnectionInfo(int id, SignalInterface* signal) : signal_(signal), id_(id) {
			}

			SignalInterface* signal_;
			const int id_;
		};

		// A connection Object remembers a connection and gives infomation
		// if the connection is active or not.		
		class Connection {
		public:
			template<class... A> friend class mw::Signal;

			// Creates a empty connection. By default the connection is not active.
			inline Connection() {
			}

			// Disconnect the active connection. The callback associated to this connection
			// will disconnect from the corresponding slot.
			void disconnect();

			// Returns true if the connection is still active else false.
			bool connected() const;

		private:
			typedef std::shared_ptr<signals::ConnectionInfo> ConnectionInfoPtr;

			// Is called from mw::Signal to bind a connection.
			Connection(const ConnectionInfoPtr& c);
			
			ConnectionInfoPtr connectionInfo_;
		};

	} // Namespace signals.

} // Namespace mw.

#endif // MW_SIGNALS_CONNECTION_H
