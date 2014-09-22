/**
 * @file signal.h
 * @brief c++11 simple signal/slot implementation
 *
 * @author Leon Colijn
 */


/*
 * signal.h
 * 
 * Copyright (c) 2014, Leon Colijn. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <memory>
#include <functional>
#include <algorithm>

#include <vector>
#include <list>

class Binding;
class SigSlotBase;


/*
 * @brief Base class for objects wishing to receive signals (i.e. have slots)
 */
class SigSlotBase 
{
    public:
        virtual ~SigSlotBase();

        void add_binding(const std::shared_ptr<Binding>& b);
        virtual void erase_binding(const std::shared_ptr<Binding>& b);
    private:
        std::list<std::shared_ptr<Binding>> _bindings;
};


class Binding: public std::enable_shared_from_this<Binding>
{
    public:
        Binding() = delete;
        Binding(const Binding& o) = delete;
        virtual ~Binding();

        Binding& operator=(const Binding& other) = delete;

        static std::shared_ptr<Binding> create(SigSlotBase* em, SigSlotBase* recv);

        void unbind();

    private:
        Binding(SigSlotBase* emitter, SigSlotBase* receiver);

        SigSlotBase* _emitter;
        SigSlotBase* _receiver;
};

template <typename... _ArgTypes>
class Signal: public SigSlotBase
{
    typedef std::function<void(_ArgTypes...)> _Fun;

    typedef std::pair<std::shared_ptr<Binding>, _Fun> _BindingRef;

    public:
        /*
         * @brief bind a new slot to this signal
         *
         * @param slot the method to bind to
         * @param inst the instance of the class to bind to
         *
         * @return void
         */
        template <typename _Class>
        void bind(void(_Class::* slot)(_ArgTypes...), _Class* inst)
        {
            std::shared_ptr<Binding> binding = Binding::create(this, inst);

            _slots.push_back(_BindingRef(
                       binding, [=](_ArgTypes... args){return (inst->*slot)(args...);}));

            inst->add_binding(binding);
            add_binding(binding);
        }

        /*
         * @brief Emit the signal
         *
         * @param args Arguments to the slots
         *
         * @return void
         *
         * @see operator()
         */
        void emit(_ArgTypes... args)
        {
            for(auto& slot: _slots) {
                std::get<1>(slot)(args...);
            }
        }


        /*
         * @brief Emit the signal
         *
         * @param args Arguments to the slots
         *
         * @return void
         *
         * @see emit 
         */
        void operator()(_ArgTypes... args)
        {
            emit(args...);
        }

    protected:
        void erase_binding(const std::shared_ptr<Binding>& b)
        {
            SigSlotBase::erase_binding(b);

            auto it = std::find_if(_slots.begin(), _slots.end(), [&b](_BindingRef r) -> bool {
                    return std::get<0>(r) == b;});
            _slots.erase(it);
        }

    private:
        std::list<std::pair<std::shared_ptr<Binding>, _Fun>> _slots;
};



#endif /* signal.h */
