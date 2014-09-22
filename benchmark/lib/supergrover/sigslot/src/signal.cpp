/**
 * @file signal.cpp
 * @brief c++11 simple signal/slot implementation
 *
 * @author Leon Colijn
 */

/*
 * signal.cpp
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

#include "signal.h"

#include <cassert>

SigSlotBase::~SigSlotBase()
{
    while(!_bindings.empty()) {
        _bindings.front()->unbind();
    }
}

void SigSlotBase::add_binding(const std::shared_ptr<Binding>& b)
{
    _bindings.push_back(b);
}

void SigSlotBase::erase_binding(const std::shared_ptr<Binding>& b)
{
    auto pos = std::find(_bindings.begin(), _bindings.end(), b);
    if(pos == _bindings.end()) {
        throw std::runtime_error("Specified binding was not found");
    }
    _bindings.erase(pos);
}




Binding::Binding(SigSlotBase* emitter, SigSlotBase* receiver): _emitter(emitter), _receiver(receiver)
{
    assert(_emitter != nullptr);
    assert(_receiver != nullptr);
}

Binding::~Binding()
{
    unbind();
}

std::shared_ptr<Binding> Binding::create(SigSlotBase* em, SigSlotBase* recv)
{
    return std::shared_ptr<Binding>(new Binding(em, recv));
}

void Binding::unbind()
{
    // TODO: Don't unbind on the currently unbinding object (binding -> object -> binding loop)
    if(_emitter) {
        SigSlotBase* em = _emitter;
        _emitter = nullptr;
        em->erase_binding(shared_from_this());
    }
    if(_receiver) {
        SigSlotBase* recv = _receiver;
        _receiver = nullptr;
        recv->erase_binding(shared_from_this());
    }
}
