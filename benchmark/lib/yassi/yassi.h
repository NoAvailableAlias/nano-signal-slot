#include <vector>
#include <functional>
#include <algorithm>

namespace {
    template <size_t I>
    struct PlaceHolder {};
}

namespace std {
    template <size_t I>
    struct is_placeholder<PlaceHolder<I>>: public integral_constant<size_t, I>
    {};
};

namespace yassi {

    template <typename To, typename From>
    To union_cast(From&& from)
    {
        union U
        {
            typename std::remove_reference<From>::type _from;
            To _to;
            ~U() {}

        } caster {from};
	
        return caster._to;
    }

// -------------------------------- FUNCTION -----------------------------------------

    template <typename FunType>
    class Function
    {
        typedef std::function<FunType> StdFunc;

        StdFunc d_func;
        void const *d_rawfcn;
        void const *d_thisptr;
        size_t d_id;

    public:
        template <typename T>
        Function(StdFunc func, void const *raw, T const *obj):
            d_func(func),
            d_rawfcn(raw),
            d_thisptr(static_cast<void const*>(obj)),
            d_id(-1)
        {}

        Function(StdFunc func, void const *raw):
            d_func(func),
            d_rawfcn(raw),
            d_thisptr(nullptr),
            d_id(-1)
        {}

        Function(StdFunc func, size_t id):
            d_func(func),
            d_rawfcn(nullptr),
            d_thisptr(nullptr),
            d_id(id)
        {}

        template <typename ... Args>
        typename StdFunc::result_type operator()(Args&& ... args)
        {
            return d_func(std::forward<Args>(args) ...);
        }

        template <typename ... Args>
        typename StdFunc::result_type operator()(Args&& ... args) const
        {
            return d_func(std::forward<Args>(args) ...);
        }

        operator std::function<FunType>()
        {
            return d_func;
        }

        void const *objPtr() const
        {
            return d_thisptr;
        }

        void const *funPtr() const
        {
            return d_rawfcn;
        }

        size_t id() const
        {
            return d_id;
        }

    };

// ----------------- MEMBERFCN --------------------------------------------

    template <typename T, typename S>
    struct MemberFcn;

    template <typename T, typename R, typename ... Args>
    struct MemberFcn<T, R(Args ...)>
    {
        using Type = R (T::*)(Args ...);
        using ConstType = R (T::*)(Args ...) const;
    };

// -------------------- ARGCOUNT -----------------------------------------

    template <typename Signature>
    struct ArgCount;

    template <typename R, typename ... Args>
    struct ArgCount<R (Args ...)>
    {
        enum { value = sizeof ... (Args) };
    };

// ------------------------- FUNCTORCHECK -------------------------------------------

    template <typename Functor, typename Signature>
    struct FunctorCheck
    {
        typedef typename MemberFcn<Functor, Signature>::Type CallbackType_;
    
        typedef char (&yes)[1];
        typedef char (&no)[2];

        template <typename T, T>
        struct IsOfType;

        template <typename T>
        static yes isValid(IsOfType<CallbackType_, &T::operator()>*);

        template <typename>
        static no isValid(...);

        enum
        {
            // non-const will match both const and non-const function operator
            nonconst_signature_matched = sizeof(isValid<Functor>(0)) == sizeof(yes),
            const_signature_matched = FunctorCheck<Functor const, Signature>::signature_matched,
            signature_matched = const_signature_matched || nonconst_signature_matched,
            argcount = ArgCount<Signature>::value
        };

        typedef typename std::conditional<nonconst_signature_matched, 
                                          CallbackType_, 
                                          typename FunctorCheck<Functor const, Signature>::CallbackType
                                          >::type CallbackType;
    };

    template <typename Functor, typename Signature>
    struct FunctorCheck<Functor const, Signature>
    {
        typedef typename MemberFcn<Functor, Signature>::ConstType CallbackType_;

        typedef char (&yes)[1];
        typedef char (&no)[2];

        template <typename T, T>
        struct IsOfType;

        template <typename T>
        static yes isValid(IsOfType<CallbackType_, &T::operator()>*);

        template <typename>
        static no isValid(...);

        enum
        {
            // const only matches const function operator
            signature_matched = sizeof(isValid<Functor>(0)) == sizeof(yes),
            argcount = ArgCount<Signature>::value
        };

        typedef typename std::conditional<signature_matched, 
                                          CallbackType_, 
                                          void
                                          >::type CallbackType;
    };

// --------------------- SIGNAL ------------------------------------

    template <typename Signature_, int I = 0>
    struct Signal
    {
        template <typename T>
        struct GetReturnType;

        template <typename R, typename ... Args>
        struct GetReturnType<R (Args ...)>
        {
            typedef R Type;
        };

        typedef Signature_ Signature;
        typedef typename GetReturnType<Signature>::Type ReturnType;
        typedef Function<Signature> FunctionType;
    };

// -------------------------- VEC_REMOVE ----------------------------


    // remove function matching a free function
    template <typename FunctionType, typename R, typename ... Args>
    static bool vecRemove(std::vector<FunctionType> &vec, R (*func)(Args ...))
    {
        for (auto it = vec.begin(); it != vec.end(); ++it)
            if (it->funPtr() == reinterpret_cast<void const*>(func))
            {
                vec.erase(it);
                return true;
            }

        return false;
    }

    // remove all matching objects/memberfunctions
    template <typename FunctionType, typename T>
    static bool vecRemove(std::vector<FunctionType> &vec, T const &obj)
    {
        enum
        {
            is_memfunc = std::is_member_function_pointer<T>::value
        };

        void const *(FunctionType::* const getComparePtr)() const = is_memfunc ?
            &FunctionType::funPtr : &FunctionType::objPtr;

        void const * const comparePtr = is_memfunc ?
            union_cast<void const*>(obj) : &obj;

        size_t oldSize = vec.size();
        vec.erase(
            std::remove_if(vec.begin(), vec.end(), [&](FunctionType const &func)
                           {
                               return (func.*getComparePtr)() == comparePtr;
                           }),
            vec.end());

        return vec.size() != oldSize;
    }

    // remove object/member-pair
    template <typename FunctionType, typename T, typename Mem>
    static bool vecRemove(std::vector<FunctionType> &vec, T const &obj, Mem const &func)
    {
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            if (it->objPtr() == &obj && it->funPtr() == union_cast<void const*>(func))
            {
                vec.erase(it);
                return true;
            }
        }

        return false;
    }

    template <size_t ID, typename FunctionType>
    static bool vecRemove(std::vector<FunctionType> &vec)
    {
        size_t oldSize = vec.size();
        vec.erase(
            std::remove_if(vec.begin(), vec.end(), [&](FunctionType const &func)
                           {
                               return func.id() == ID;
                           }),
            vec.end());

        return vec.size() != oldSize;
    }


    template <typename ... Signals>
    class Emitter
    {
// -------------------------- DATA -----------------------------------

        template <typename First, typename ... Rest>
        struct Data: public Data<Rest ...>
        {
            typedef std::vector<typename First::FunctionType> MemberType;
    
            MemberType vec;

            template <typename T, typename First_, typename ... Rest_>
            struct Get
            {
                typedef typename Get<T, Rest_ ...>::Type Type;
            };

            template <typename T, typename ... Rest_>
            struct Get<T, T, Rest_ ...>
            {
                typedef Data<T, Rest_ ...> Type;
            };

            template <typename T>
            typename Get<T, First, Rest ...>::Type::MemberType const &get() const
            {
                return Get<T, First, Rest ...>::Type::vec;
            }
    
            template <typename T>
            typename Get<T, First, Rest ...>::Type::MemberType &get()
            {
                return Get<T, First, Rest ...>::Type::vec;
            }
            
            template <typename T>
            bool disconnectAll(T const &obj)
            {
                return vecRemove(vec, obj) | 
                    Data<Rest...>::disconnectAll(obj);
            }

            template <typename Return, typename ... Args>
            bool disconnectAll(Return (*func)(Args ...))
            {
                return vecRemove(vec, func) | 
                    Data<Rest...>::disconnectAll(func);
            }

            template <typename Object, typename Func>
            bool disconnectAll(Object const &obj, Func const &func) 
            {
                return vecRemove(vec, obj, func) | 
                    Data<Rest ...>::disconnectAll(obj, func);
            }

            template <size_t ID>
            bool disconnectAll()
            {
                return vecRemove<ID>(vec) | 
                    Data<Rest...>::template disconnectAll<ID>();
            }

            bool disconnectAll()
            {
                bool ret = vec.size() != 0;
                vec.clear();

                return ret | Data<Rest...>::disconnectAll();
            }

        };

        template <typename Last>
        struct Data<Last>
        {
            typedef std::vector<typename Last::FunctionType> MemberType;

            MemberType vec;
    
            template <typename T1, typename T2, typename Dummy = void>
            struct Get;

            template <typename T, typename Dummy>
            struct Get<T, Last, Dummy>
            {
                static_assert(std::is_same<T, Last>::value,
                              "The emitter object can not connect/disconnect/emit this signal.");
            };
            
            template <typename Dummy>
            struct Get<Last, Last, Dummy>
            {
                typedef Data<Last> Type;
            };

            template <typename T>
            typename Get<T, Last>::Type::MemberType const &get() const
            {
                return Get<T, Last>::Type::vec;
            }

            template <typename T>
            typename Get<T, Last>::Type::MemberType &get()
            {
                return Get<T, Last>::Type::vec;
            }

            template <typename T>
            bool disconnectAll(T const &obj)
            {
                return vecRemove(vec, obj);
            }

            template <typename Return, typename ... Args>
            bool disconnectAll(Return (*func)(Args ...))
            {
                return vecRemove(vec, func);
            }

            template <typename Object, typename Func>
            bool disconnectAll(Object const &obj, Func const &func) 
            {
                return vecRemove(vec, obj, func);
            }

            template <size_t ID>
            bool disconnectAll()
            {
                return vecRemove<ID>(vec);
            }

            bool disconnectAll()
            {
                bool ret = vec.size() != 0;
                vec.clear();
                return ret;
            }
        };

        Data<Signals ...> d_signals;
    
// -------------------------- BIND -----------------------------------
        template <typename Signature>
        struct Bind
        {
            template <size_t CountDown, typename Dummy, size_t ... Indices>
            struct BindHelper
            {
                template <typename Func, typename T>
                static std::function<Signature> bind(Func&& memFunc, T *obj)
                {
                    return BindHelper<CountDown - 1, Dummy, CountDown, Indices ...>::
                        bind(std::forward<Func>(memFunc), obj);
                }
            };

            template <typename Dummy, size_t ... Indices>
            struct BindHelper<0, Dummy, Indices ...>
            {
                template <typename Func, typename T>
                static std::function<Signature> bind(Func&& memFunc, T *obj)
                {
                    return std::bind(std::forward<Func>(memFunc), obj, PlaceHolder<Indices>{} ...);
                }
            };

            template <typename Func, typename T>
            static std::function<Signature> bind(Func&& memFunc, T *obj)
            {
                return BindHelper<ArgCount<Signature>::value, void>
                    ::bind(std::forward<Func>(memFunc), obj);
            }
        };

// ----------------- EmitHelper ---------------------------------------

        template <typename SignalReturn, typename Dummy = void>
        struct EmitHelper
        {
            typedef std::vector<
                typename std::remove_const<
                    typename std::remove_reference<SignalReturn>::type
                    >::type
                > EmitReturn;

            template <typename Signal, typename ... Args>
            static EmitReturn emit(Emitter<Signals...> const &emitter, Args&& ... args)
            {
                auto const &vec = emitter.d_signals.template get<Signal>();
                EmitReturn ret;
                ret.reserve(vec.size());
            
                for (auto &fun: vec)
                    ret.push_back(fun(std::forward<Args>(args) ...));

                return ret;
            }
        };

        template <typename Dummy>
        struct EmitHelper<void, Dummy>
        {
            typedef void EmitReturn;

            template <typename Signal, typename ... Args>
            static EmitReturn emit(Emitter<Signals...> const &emitter, Args&& ... args)
            {
                auto const &vec = emitter.d_signals.template get<Signal>();
                for (auto &fun: vec)
                    fun(std::forward<Args>(args) ...);
            }
        };
    
// ----------------- User interface ------------------------------

    public:

// ----------------- Connect ------------------------------------

        // connect free function
        template <typename Signal, typename Return, typename ... Args>
        void connect(Return (*func)(Args ...))
        {
            typedef typename Signal::Signature Signature;
            d_signals.template get<Signal>().emplace_back(
                std::function<Signature>(func),
                reinterpret_cast<void const *>(func));
        }

        // Non-temporary functor/lambda
        template <typename Signal, typename Functor>
        void connect(Functor&& functor)
        {
            // without a second id-argument, it may not be a temporary
            static_assert(std::is_reference<Functor>::value, "A temporary needs an ID.");

	    typedef typename std::remove_reference<Functor>::type PlainFunctor;
            typedef typename Signal::Signature Signature;
            typedef typename FunctorCheck<PlainFunctor, Signature>::CallbackType CallbackType;

            CallbackType callback = static_cast<CallbackType>(&PlainFunctor::operator());
            d_signals.template get<Signal>().emplace_back(
                Bind<Signature>::bind(callback, &functor),
                union_cast<void const *>(callback),
                &functor
                );
        }

        // Temporary functor/lambda
        template <typename Signal, size_t ID, typename Functor>
        void connect(Functor&& functor)
        {
            // with an id-argument, it must be a temporary
            static_assert(!std::is_reference<Functor>::value, "Only a temporary needs an ID");

            typedef typename Signal::Signature Signature;
            d_signals.template get<Signal>().emplace_back(
                std::function<Signature>(std::forward<Functor>(functor)), 
                ID);
        }

        // Object + Member
        template <typename Signal, typename Object, typename Func>
        void connect(Object&& obj, Func const &func) 
        {
            // Check that it's not a temporary
            static_assert(std::is_reference<Object>::value, 
                          "connect(): cannot connect to a temporary and its member-function.");

            typedef typename Signal::Signature Signature;
            d_signals.template get<Signal>().emplace_back(
                Bind<Signature>::bind(func, &obj),
                union_cast<void const *>(func),
                &obj);
        }


// ----------------- Disconnect ------------------------------------

        // free function
        template <typename Signal, typename Return, typename ... Args>
        bool disconnect(Return (*func)(Args ...))
        {
            auto &vec = d_signals.template get<Signal>();
            return vecRemove(vec, func);
        }

        // Everything associated with an object/function of type T
        template <typename Signal, typename T>
        bool disconnect(T const &obj)
        {
            auto &vec = d_signals.template get<Signal>();
            return vecRemove(vec, obj);
        }

        // disconnect object + member
        template <typename Signal, typename Object, typename Func>
        bool disconnect(Object const &obj, Func const &func) 
        {
            auto &vec = d_signals.template get<Signal>();
            return vecRemove(vec, obj, func);
        }

        // Disconnect ID-marked entries from a signal
        template <typename Signal, size_t ID>
        bool disconnect()
        {
            auto &vec = d_signals.template get<Signal>();
            return vecRemove<ID>(vec);
        }

        // Disconnect everything from a signal
        template <typename Signal>
        bool disconnect()
        {
            auto &vec = d_signals.template get<Signal>();
            bool ret = vec.size() != 0;
            vec.clear();
            return ret;
        }

        // Disconnect everything from an object or member-function-pointer
        template <typename T>
        bool disconnect(T const &obj)
        {
            return d_signals.disconnectAll(obj);
        }

        // Disconnect every signal to func
        template <typename R, typename ... Args>
        bool disconnect(R (*func)(Args ...))
        {
            return d_signals.disconnectAll(func);
        }

        // disconnect object + member
        template <typename Object, typename Func>
        bool disconnect(Object const &obj, Func const &func) 
        {
            return d_signals.disconnectAll(obj, func);
        }

        // Disconnect from ID
        template <size_t ID>
        bool disconnect()
        {
            return d_signals.template disconnectAll<ID>();
        }

        // Disconnect everything
        bool disconnect()
        {
            return d_signals.disconnectAll();
        }

// -------------------------- Emit!  --------------------------

        template <typename Signal, typename ... Args>
        typename EmitHelper<typename Signal::ReturnType>::EmitReturn emit(Args&& ... args) const
        {
            return EmitHelper<typename Signal::ReturnType>::template emit<Signal>(*this, std::forward<Args>(args)...);
        }

        template <typename Signal, typename ReturnType, typename ... Args>
        typename EmitHelper<ReturnType>::EmitReturn emit(Args&& ... args) const
        {
            static_assert(std::is_same<ReturnType, void>::value,
                          "Second template argument to emit() may only be void.");
            
            return EmitHelper<ReturnType>::template emit<Signal>(*this, std::forward<Args>(args)...);
        }
    }; // class Emitter


// -------------------------- Safe Slot --------------------------------------------------

    template <typename T>
    using DisconnectMe = Signal<void(T const&)>;

    class SafeSlot: public Emitter<DisconnectMe<SafeSlot>>
    {
        typedef DisconnectMe<SafeSlot> DiscSig;
        typedef Emitter<DiscSig> Parent;

        template <typename First, typename ... Rest>
            struct Connecter
        {
            static void connectAll(SafeSlot &safeSlot, First const &first, Rest&& ... rest)
            {
                typedef typename std::remove_reference<First>::type EmitterType;
                bool (EmitterType::*ptr)(SafeSlot const &) = &EmitterType::disconnect;
                safeSlot.template connect<DiscSig>(first, ptr);
                Connecter<Rest...>::connectAll(safeSlot, rest ...);
            }
        };

        template <typename Last>
            struct Connecter<Last>
        {
            static void connectAll(SafeSlot &safeSlot, Last const &last)
            {
                typedef typename std::remove_reference<Last>::type EmitterType;
                bool (EmitterType::*ptr)(SafeSlot const &) = &EmitterType::disconnect;
                safeSlot.template connect<DiscSig>(last, ptr);
            }
        };


    public:
        SafeSlot() = default;

        template <typename FirstEmitter, typename ... EmitterList>
            SafeSlot(FirstEmitter&& first, EmitterList&& ... list)
        {
            addEmitters(std::forward<FirstEmitter>(first),
                        std::forward<EmitterList>(list)...);
        }

        template <typename ... Emitters>
        void addEmitters(Emitters&& ... list)
        {
            Connecter<Emitters ...>::connectAll(*this, std::forward<Emitters>(list)...);
        }

        ~SafeSlot()
        {
            Parent::template emit<DiscSig>(*this);
        }
    }; // class SafeSlot

} // namespace yassi
