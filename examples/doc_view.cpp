// Document/View sample from Boost.Signals
//
// modified by Claus Klein
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include "nano_observer.hpp"     // Nano::Observer
#include "nano_signal_slot.hpp"  // Nano::Signal

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

namespace {

// Creating aliases when using nano-signal-slot will increase the maintainability of your code
// especially if you are choosing to use the alternative policies.
using NanoPolicy = Nano::TS_Policy_Safe<>;

template <typename Signature>
using Signal = Nano::Signal<Signature, NanoPolicy>;

using Observer = Nano::Observer<NanoPolicy>;

/// document_def_code_snippet
class Document {
 public:
  using signal_t = Signal<void()>;

  Document() = default;

  signal_t& getSignal() { return m_sig; }

  void append(const char* s) {
    m_text += s;

    m_sig.fire();
  }

  const std::string& getText() const { return m_text; }

 private:
  signal_t m_sig;
  std::string m_text;
};

/// text_view_def_code_snippet
class TextView : public Observer {
 public:
  explicit TextView(Document& doc) : m_document(doc) { m_document.getSignal().connect<&TextView::refresh>(this); }

  void refresh() const { std::cout << "TextView: " << m_document.getText() << std::endl; }

 private:
  Document& m_document;
};

/// hex_view_def_code_snippet
class HexView : public Observer {
 public:
  explicit HexView(Document& doc) : m_document(doc) { m_document.getSignal().connect<&HexView::refresh>(this); }

  void refresh() const {
    const std::string& s = m_document.getText();

    std::cout << "HexView:";

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
      std::cout << ' ' << std::hex << static_cast<int>(*it);

    std::cout << std::endl;
  }

 private:
  Document& m_document;
};

}  // namespace

/// document_view_main_code_snippet
int main(int argc, char* argv[]) {
  using namespace std::chrono_literals;

  try {
    Document doc;
    TextView v1(doc);  // NOTE: The Document must exists as long as the view! CK
    {
      HexView v2(doc);
      doc.append(argc == 2 ? argv[1] : " Hello world!");
    }

    std::this_thread::sleep_for(123us);
    doc.append(" ... Again, but without HexView!");
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
