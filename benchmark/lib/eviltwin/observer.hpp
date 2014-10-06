/** A flexible implementation of the observer pattern with automatic lifetime
 *  management.
 */

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

namespace obs
{

// Registration is just a scoped type, once it goes out of scope,
// your registration has expired. The idea is that you store this
// registration as a member of the actual observer, tying it to
// the same lifetime.
using Registration = std::shared_ptr<void>;


// Nothing to see here, the detail namespace is used for just that:
// implementation details
namespace detail
{
  // We want to create a base class for handling all the observer
  // boilerplate, because of that we need to store 'untyped' pointers.
  using UniversalPtr = std::shared_ptr<void>;

  // In this implementation we often need to know if an object is alive or not.
  // The way we do that is by using shared_ptr and weak_ptr. Often we do not
  // even care to store anything at all, we're just interested in using the
  // deterministic destructor (RAII). This function creates a 'dangerous'
  // shared_ptr because it points to 'DEADC0DE', this avoids and extra 'new',
  // we could have used make_shared but that does not allow a customer deleter.
  // Note that the deleter should not free any memory since we do not actually
  // allocate anything.
  template <typename Deleter>
  UniversalPtr createEmptyPtr(Deleter deleter)
  {
    return UniversalPtr((void*) 0xDEADC0DE, deleter);
  }

  // Slap the baby's bottom. This functions creates a complete no-op shared_ptr
  // which we use as the 'heartbeat' for the Subject
  /*UniversalPtr createHeartBeat()
  {
    return createEmptyPtr([] (void*) {});
  }*/

  // A base class for handling common code in all Subjects. Normally using
  // inheritance for code reuse is bad but in this case it is a common idiom
  // to avoid 'code bloat' due to the templating. I.e., if we would leave
  // this code in the actualy template class it would needlessly get replicated
  // for each instantiation with a different type.
  class SubjectBase
  {
  protected:
    //SubjectBase(): heartBeat_(createHeartBeat()) {}
    SubjectBase(): heartBeat_(createEmptyPtr([](void*){})) {}

    Registration registerObserver(UniversalPtr fptr)
    {
      observers_.push_back(fptr);
      std::weak_ptr<UniversalPtr::element_type> weakHeartBeat(heartBeat_);

      // we pass the function pointer and a weak_ptr into this lambda by
      // value. This is important because it lets the observer know if the
      // subject is still alive at the moment of unregistering.
      // This is basically the Registration, it's a placeholder that removes
      // the observer from the list when the registration goes out of scope.
      return createEmptyPtr([fptr, weakHeartBeat, this] (void*)
      {
        if (auto isBeating = weakHeartBeat.lock())
        {
          observers_.erase(std::remove(begin(observers_), end(observers_), fptr),
                           end(observers_));
        }
      });
    }

    std::vector<UniversalPtr> observers_;

  private:
    UniversalPtr heartBeat_;
  };

} // eons detail

// We need this otherwise we cannot explicitly specialise for the funtion
// signature later
template <typename Signature>
struct Subject;

// The Subject class is specialised on the type of 'notification' you
// want to send to the observers. Like Registration the Subject objects
// should be tied to the lifetime of what is the real subject, preferably as
// a member variable.
template <typename Return, typename... Params>
struct Subject<Return (Params...)> : detail::SubjectBase
{
  using F = std::function<Return (Params...)>;
  using FPtr = std::shared_ptr<F>;

  // instead of the Java-esque 'notify' we just use operator()
  void operator()(Params... params)
  {
    for (const auto& observer : observers_)
    {
      const FPtr fptr = std::static_pointer_cast<F>(observer);
      (*fptr)(params...);
    }
  }

  Registration registerObserver(F f)
  {
    FPtr fptr(new F(std::move(f)));
    return SubjectBase::registerObserver(fptr);
  }

};

} // eons obs
