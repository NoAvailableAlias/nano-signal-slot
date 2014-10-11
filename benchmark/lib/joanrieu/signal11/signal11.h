/*
  The MIT License (MIT)

  Copyright (c) 2013 Joan Rieu

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef SIGNAL11_H
#define SIGNAL11_H

#include <functional>
#include <list>

template<class... Args>
class signal11 {

  public:

  typedef std::function<void(Args...)> slot;
  typedef typename std::list<slot>::iterator connection; // std::list::iterator is never invalidated

  connection connect(slot slot) {
    m_slots.push_back(std::move(slot));
    return --m_slots.end();
  }

  connection connect_oneshot(slot slot) {
    auto connection = connect(slot);
    m_oneshot.push_back(connection);
    return connection;
  }

  void disconnect(connection connection) {
    m_slots.erase(connection);
  }

  void operator()(Args... args) {
    for (auto slot: m_slots)
      slot(args...);
    for (auto connection: m_oneshot)
      disconnect(connection);
    m_oneshot.clear();
  }

  private:

  std::list<slot> m_slots;
  std::list<connection> m_oneshot;

};

#endif
