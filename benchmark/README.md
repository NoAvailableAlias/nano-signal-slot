Performance
-----------

**_Higher score is better._** _N / milliseconds per sample._

```
+ -------------------------------------------------------------------------------- +
| Library              |  construct |  destruct |  connect |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| jeffomatic jl_signal |  123557    |  9645     |  44346   |  39051    |  6352     |
| Yassi                |  149941    |  2250     |  1610    |  37176    |  845      |
| amc522 Signal11      |  125020    |  5661     |  3730    |  32403    |  2092     |
| mwthinker Signal     |  109900    |  5181     |  1857    |  38771    |  1306     |
| pbhogan Signals      |  107762    |  5195     |  4859    |  31997    |  2275     |
| * fr00b0 nod         |  95490     |  4150     |  2642    |  30786    |  1493     |
| EvilTwin Fork        |  105054    |  3801     |  2112    |  18928    |  1252     |
| EvilTwin Observer    |  96725     |  2517     |  1210    |  19295    |  813      |
| joanrieu signal11    |  90288     |  6119     |  4336    |  7796     |  1744     |
| supergrover sigslot  |  11086     |  1395     |  2244    |  38451    |  759      |
| Nano-signal-slot     |  12449     |  4060     |  3761    |  29266    |  1655     |<-
| * winglot Signals    |  5780      |  2041     |  2379    |  31247    |  900      |
| * neosigslot         |  13778     |  2541     |  2343    |  6345     |  928      |
| Boost Signals        |  7844      |  1653     |  571     |  4483     |  354      |
| * Boost Signals2     |  6367      |  1821     |  865     |  3118     |  459      |
+ -------------------------------------------------------------------------------- +
```
_* Library is designed to be thread safe._

Size Metrics
------------

_Results are from Visual Studio 2013 using release build settings._

| Library | Object File Size ^ | Header Only |
| ------- | ------------------ |:-----------:|
| [jeffomatic jl_signal](https://github.com/jeffomatic/jl_signal) | 846 kb | - |
| [winglot Signals](https://github.com/winglot/Signals) | 912 kb | - |
| [Nano-signal-slot](https://github.com/NoAvailableAlias/nano-signal-slot/tree/FT) | 915 kb | X |
| [pbhogan Signals](https://github.com/pbhogan/Signals) | 944kb | X |
| [mwthinker Signal](https://github.com/mwthinker/Signal) | 951 kb | - |
| [EvilTwin Fork](https://github.com/NoAvailableAlias/nano-signal-slot/blob/master/benchmark/lib/eviltwin/observer_fork.hpp) | 956 kb | X |
| [supergrover sigslot](https://github.com/supergrover/sigslot) | 966 kb | - |
| [fr00b0 nod](https://github.com/fr00b0/nod) | 969 kb | X |
| [EvilTwin Observer](http://eviltwingames.com/blog/the-observer-pattern-revisited/) | 973 kb | X |
| [amc522 Signal11](https://github.com/amc522/Signal11) | 981 kb | X |
| [joanrieu signal11](https://github.com/joanrieu/signal11) | 990 kb | X |
| [Yassi](http://www.codeproject.com/Articles/867044/Yassi-Yet-Another-Signal-Slot-Implementation) | 992 kb | X |
| [Boost Signals](http://www.boost.org/doc/libs/1_56_0/doc/html/signals.html) | 1,375 kb | - |
| [neosigslot](http://www.i42.co.uk/stuff/neosigslot.htm) | 1,943 kb | - |
| [Boost Signals2](http://www.boost.org/doc/libs/1_56_0/doc/html/signals2.html) | 2,350 kb | ? |

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
