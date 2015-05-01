Performance
-----------

**_Higher score is better._** _N / milliseconds per sample._

```
+ -------------------------------------------------------------------------------- +
| Library       [MSVC] |  construct |  destruct |  connect |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| jeffomatic jl_signal |  120169    |  9683     |  46925   |  38034    |  6248     |
| Yassi                |  146357    |  2249     |  1639    |  39144    |  810      |
| amc522 Signal11      |  121921    |  5776     |  3574    |  34371    |  1985     |
| pbhogan Signals      |  109879    |  5345     |  4787    |  31980    |  2277     |
| mwthinker Signal     |  106472    |  5291     |  1848    |  39052    |  1291     |
| EvilTwin Fork        |  108401    |  3986     |  2131    |  20262    |  1281     |
| EvilTwin Observer    |  103376    |  2490     |  1304    |  19282    |  798      |
| joanrieu signal11    |  87940     |  5994     |  4279    |  7839     |  1757     |
| Nano-signal-slot     |  13564     |  4277     |  4572    |  27476    |  1818     | <-
| supergrover sigslot  |  10558     |  1387     |  2126    |  36789    |  745      |
| * winglot Signals    |  5272      |  1942     |  2424    |  31097    |  893      |
| * fr00b0 nod         |  3213      |  1101     |  311     |  32641    |  211      |
| * neosigslot         |  13585     |  2500     |  2193    |  6059     |  909      |
| Boost Signals        |  8043      |  1635     |  572     |  4149     |  363      |
| * Boost Signals2     |  6457      |  1903     |  845     |  3315     |  450      |
+ -------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

Size Metrics
------------

_Results using release build settings._

| Library | Object File Size ^ | Lines of Code | Header Only |
| ------- | ------------------ | ------------- |:-----------:|
| [jeffomatic jl_signal](https://github.com/jeffomatic/jl_signal) | 846 kb | 6060 ~ | - |
| [winglot Signals](https://github.com/winglot/Signals) | 912 kb | 427 | - |
| [Nano-signal-slot](https://github.com/NoAvailableAlias/nano-signal-slot/tree/FT) | 915 kb | 251 | X |
| [pbhogan Signals](https://github.com/pbhogan/Signals) | 944kb | 2595 ~ | X |
| [mwthinker Signal](https://github.com/mwthinker/Signal) | 951 kb | 171 | - |
| [EvilTwin Fork](https://github.com/NoAvailableAlias/nano-signal-slot/blob/master/benchmark/lib/eviltwin/observer_fork.hpp) | 956 kb | 65 | X |
| [supergrover sigslot](https://github.com/supergrover/sigslot) | 966 kb | 185 | - |
| [fr00b0 nod](https://github.com/fr00b0/nod) | 969 kb | < 300 | X |
| [EvilTwin Observer](http://eviltwingames.com/blog/the-observer-pattern-revisited/) | 973 kb | 117 | X |
| [amc522 Signal11](https://github.com/amc522/Signal11) | 981 kb | 730 | X |
| [joanrieu signal11](https://github.com/joanrieu/signal11) | 990 kb | 47 | X |
| [Yassi](http://www.codeproject.com/Articles/867044/Yassi-Yet-Another-Signal-Slot-Implementation) | 992 kb | ~700 | X |
| [Boost Signals](http://www.boost.org/doc/libs/1_56_0/doc/html/signals.html) | 1,375 kb | N/A | - |
| [neosigslot](http://www.i42.co.uk/stuff/neosigslot.htm) | 1,943 kb | 2496 ~ | - |
| [Boost Signals2](http://www.boost.org/doc/libs/1_56_0/doc/html/signals2.html) | 2,350 kb | N/A | ? |

Benchmark Algorithms
--------------------

_The test size N is doubled for every iteration._

| Algorithm | Description |
| --------- | ----------- |
| [validation_assert](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L21) | Assert correct implementation operation. |
| [construction](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L46) | Time the construction of a Signal and N number of Foo instances. |
| [destruction](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L67) | Time the destruction of a Signal and associated Connections to N number of Foo instances. |
| [connection](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L97) | Time Signal connections to a randomized N number of Foo instances. |
| [emission](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L125) | Time the duration of an N slot emission. |
| [combined](https://github.com/NoAvailableAlias/nano-signal-slot/blob/FT/benchmark/benchmark.hpp#L155) | Time construction, destruction, connection, and emission together. |
