#ifndef IG_NOD_INCLUDE_NOD_HPP
#define IG_NOD_INCLUDE_NOD_HPP

#include <vector>       // std::vector
#include <functional>   // std::function
#include <mutex>        // std::mutex, std::lock_guard
#include <memory>       // std::shared_ptr, std::weak_ptr
#include <algorithm>    // std::find_if()
#include <cassert>      // assert()

namespace nod {
	// implementational details
	namespace detail {
		/// Interface for type erasure when disconnecting slots
		struct disconnector {
			virtual void operator()( std::size_t index ) const = 0;
		};		
		/// Deleter that doesn't delete
		inline void no_delete(disconnector*){			
		};
	} // namespace detail

	/// Base template for the signal class
	template< class T >
	class signal;


	/// Connection class.
	///
	/// This is used to be able to disconnect slots after they have been connected.
	/// Used as return type for the connect method of the signals.
	///
	/// Connections are not default constructible.
	/// Connections are not copy constructible or copy assignable.
	/// Connections are move constructible and move assignable.
	///
	class connection {
		public:
			// Connection are not default constructible, copy constructible or copy assignable
			connection() = delete;
			connection( connection const& ) = delete;
			connection& operator=( connection const& ) = delete;

			/// Move constructor
			/// @param other   The instance to move from.
			connection( connection&& other ) :
				_weak_disconnector( std::move(other._weak_disconnector) ),
				_index( other._index )
			{}

			/// Move assign operator.
			/// @note If this connection is connected, it will be disconnected when moving
			///       another instance into it.
			/// @param other   The instance to move from.
			connection& operator=( connection&& other ) {
				disconnect();
				_weak_disconnector = std::move( other._weak_disconnector );
				_index = other._index;
				return *this;
			}

			/// @returns `true` if the connection is connected to a signal object,
			///          and `false` otherwise.
			bool connected() const {
				return !_weak_disconnector.expired();
			}

			/// Disconnect the slot from the connection.
			///
			/// If the connection represents a slot that is connected to a signal object, calling
			/// this method will disconnect the slot from that object. The result of this operation
			/// is that the slot will stop recieving calls when the signal is invoked.
			void disconnect();

		private:
			/// The signal template is a friend of the connection, since it is the
			/// only one allowed to create instances using the meaningful constructor.
			template<class T> friend class signal;	

			/// Create a connection.
			/// @param shared_disconnector   Disconnector instance that will be used to disconnect
			///                              the connection when the time comes. A weak pointer 
			///                              to the disconnector will be held within the connection
			///                              object.
			/// @param index                 The slot index of the connection.
			connection( std::shared_ptr<detail::disconnector> const& shared_disconnector, std::size_t index ) :
				_weak_disconnector( shared_disconnector ),
				_index( index )
			{}

			/// Weak pointer to the current disconnector functor.
			std::weak_ptr<detail::disconnector> _weak_disconnector;
			/// Slot index of the connected slot.
			std::size_t _index;
	};

	/// Scoped connection class.
	/// 
	/// This type of connection is automatically disconnected when
	/// the connection object is destructed.
	///
	class scoped_connection
	{
		public:
			/// Scoped connections are not default constructible
			scoped_connection() = delete;
			/// Scoped connections are not copy constructible
			scoped_connection( scoped_connection const& ) = delete;
			/// Scoped connections are not copy assingable
			scoped_connection& operator=( scoped_connection const& ) = delete;

			/// Construct a scoped connection from a connection object
			/// @param connection   The connection object to manage
			scoped_connection( connection&& c ) :
				_connection( std::forward<connection>(c) )
			{}

			/// destructor
			~scoped_connection() {
				disconnect();
			}

			///
			/// @returns `true` if the connection is connected to a signal object,
			///          and `false` otherwise.
			bool connected() const {
				return _connection.connected();
			}

			/// Disconnect the slot from the connection.
			///
			/// If the connection represents a slot that is connected to a signal object, calling
			/// this method will disconnect the slot from that object. The result of this operation
			/// is that the slot will stop recieving calls when the signal is invoked.
			void disconnect() {
				_connection.disconnect();
			}

		private:
			/// Underlying connection object
			connection _connection;
	};

	/// Signal template specialization.
	///
	/// This is the main signal implementation, and it is used to
	/// implement the observer pattern whithout the overhead 
	/// boilerplate code that typically comes with it.
	///
	/// Any function or function object is considered a slot, and 
	/// can be connected to a signal instance, as long as the signature
	/// of the slot matches the signature of the signal.
	///
	///
	///
	///
	///
	///
	template <class R, class... A >
	class signal<R(A...)>
	{
		public:
			/// signals are not copy constructible
			signal( signal const& ) = delete;
			/// signals are not copy assignable
			signal& operator=( signal const& ) = delete;

			/// signals are default constructible
			signal() = default;

			// Destruct the signal object.
			~signal() {
				// If we are unlucky, some of the connected slots
				// might be in the process of disconnecting from other threads.
				// If this happens, we are risking to destruct the disconnector
				// object managed by our shared pointer before they are done 
				// disconnecting. This would be bad. To solve this problem, we
				// discard the shared pointer (that is pointing to the disconnector
				// object within our own instance), but keep a weak pointer to that
				// instance. We then stall the destruction until all other weak
				// pointers have released their "lock" (indicated by the fact that
				// we will get a nullptr when locking our weak pointer).
				std::weak_ptr<detail::disconnector> weak{_shared_disconnector};
				_shared_disconnector.reset();
				while( weak.lock() != nullptr )	{
					// we just stall here, waiting for all threads to release the disconnector
				}
			}

			/// Type that will be used to store the slots for this signal type.
			using slot_type = std::function<R(A...)>;

			///
			///
			///
			///
			template <class T>
			connection connect( T&& slot ) {
				std::lock_guard<std::mutex> lock{ _mutex };
				auto it = std::find_if( std::begin(_slots), std::end(_slots), 
					[]( slot_type const& slot ){
						return !slot;
					});
				std::size_t index;
				if( it == std::end(_slots) ) {
					_slots.push_back( std::forward<T>(slot) );
					index = _slots.size()-1;
				}
				else {
					index = std::distance( std::begin(_slots), it );
					_slots[index] =  std::forward<T>(slot);
				}				
				if( _shared_disconnector == nullptr ) {
					_disconnector = disconnector{ this };
					_shared_disconnector = std::shared_ptr<detail::disconnector>{&_disconnector, detail::no_delete};
				}
				return connection{ _shared_disconnector, index };
			}
	
			/// Function call operator.
			///
			/// Calling this is how the signal is triggered and the
			/// connected slots are called.
			///
			/// @note The slots will be called in the order they were 
			///       connected to the signal.
			///
			/// @param args   Arguments that will be propagated to the
			///               connected slots when they are called.
			void operator()( A&&... args ) const {
				std::lock_guard<std::mutex> lock{ _mutex };
				for( auto const& slot : _slots ) {
					if( slot ) {
						slot( args... );
					}
				}
			}

		private:

			/// Implementation of the disconnection operation.
			///
			/// This is private, and only called by the connection
			/// objects created when connecting slots to this signal.
			/// @param index   The slot index of the slot that should
			///                be disconnected.
			void disconnect( std::size_t index ) {
				std::lock_guard<std::mutex> lock( _mutex );				
				assert( _slots.size() > index );				
				_slots[ index ] = slot_type{};
			}
			
			/// Implementation of the shared disconnection state
			/// used by all connection created by signal instances.
			/// 
			/// This inherits the @ref detail::disconnector interface
			/// for type erasure.
			struct disconnector :
				detail::disconnector
			{
				/// Default constructor, resulting in a no-op disconnector.
				disconnector() :
					_ptr(nullptr)
				{}

				/// Create a disconnector that works with a given signal instance.
				/// @param ptr   Pointer to the signal instance that the disconnector
				///              should work with.
				disconnector( signal<R(A...)>* ptr ) :
					_ptr( ptr )
				{}

				/// Disconnect a given slot on the current signal instance.
				/// @note If the instance is default constructed, or created
				///       with `nullptr` as signal pointer this operation will
				///       effectively be a no-op.
				/// @param index   The index of the slot to disconnect.
				void operator()( std::size_t index ) const override {
					if( _ptr ) {
						_ptr->disconnect( index );
					}
				}

				/// Pointer to the current signal.
				signal<R(A...)>* _ptr;
			};

			/// Mutex to syncronize access to the slot vector
			mutable std::mutex _mutex;
			/// Vector of all connected slots
			std::vector<slot_type> _slots;
			/// Disconnector operation, used for executing disconnection in a
			/// type erased manner.
			disconnector _disconnector;
			/// Shared pointer to the disconnector. All connection objects has a 
			/// weak pointer to this pointer for performing disconnections.
			std::shared_ptr<detail::disconnector> _shared_disconnector;
	};

	// Implementation of the disconnect operation of the connection class
	inline void connection::disconnect() {
		auto ptr = _weak_disconnector.lock();
		if( ptr ) {
			(*ptr)( _index );
		}
		_weak_disconnector.reset();
	}
} // namespace nod

#endif // IG_NOD_INCLUDE_NOD_HPP