Performance
-----------

**_Higher score is better._** _N / milliseconds per sample._

```
+ -------------------------------------------------------------------------------- +
| Library       [MSVC] |  construct |  destruct |  connect |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| jeffomatic jl_signal |  120984    |  9945     |  47380   |  36026    |  6463     |
| amc522 Signal11      |  125716    |  5558     |  3554    |  33909    |  2047     |
| mwthinker Signal     |  110859    |  4716     |  1670    |  38904    |  1151     |
| pbhogan Signals      |  110994    |  4215     |  3587    |  29659    |  1818     |
| EvilTwin Fork        |  115713    |  3303     |  1864    |  19827    |  1093     |
| EvilTwin Observer    |  107610    |  2338     |  1098    |  19188    |  723      |
| joanrieu signal11    |  86338     |  4525     |  3366    |  7625     |  1533     |
| supergrover sigslot  |  11880     |  1403     |  2166    |  41723    |  766      |
| Nano-signal-slot     |  13771     |  4169     |  4335    |  30313    |  1823     |
| * winglot Signals    |  5568      |  1998     |  2548    |  33095    |  899      |
| * neosigslot         |  13616     |  2506     |  2367    |  6508     |  921      |
| Boost Signals        |  8139      |  1688     |  586     |  4262     |  366      |
| * Boost Signals2     |  6031      |  1820     |  866     |  3279     |  456      |
+ -------------------------------------------------------------------------------- +
```

#### Performance using an Allocator

To utilize allocators in Nano-signal-slot, the only change required is the following:

```
    // Before
    std::map<DelegateKey, Observer*> tracked_connections;
```
```
    // After
    using Allocator = YourAllocator<std::map<DelegateKey, Observer*>::value_type>;
    std::map<DelegateKey, Observer*, std::less<DelegateKey>, Allocator> tracked_connections;
```

_Jl_signal uses a custom static allocator to achieve high performance._

```
+ -------------------------------------------------------------------------------- +
| Library       [MSVC] |  construct |  destruct |  connect |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| jeffomatic jl_signal |  123124    |  10155    |  48483   |  35243    |  6548     |
| amc522 Signal11      |  123355    |  5709     |  3501    |  30904    |  2050     |
| mwthinker Signal     |  118211    |  4318     |  1654    |  39040    |  1158     |
| pbhogan Signals      |  114170    |  4167     |  3616    |  32659    |  1808     |
| EvilTwin Fork        |  111976    |  3226     |  1805    |  19350    |  1090     |
| EvilTwin Observer    |  100186    |  2177     |  1086    |  19505    |  729      |
| Nano-signal-slot     |  73118     |  8362     |  9273    |  28565    |  3919     |
| joanrieu signal11    |  85838     |  4720     |  3419    |  7619     |  1532     |
| supergrover sigslot  |  11943     |  1365     |  2091    |  39375    |  752      |
| * winglot Signals    |  5366      |  2035     |  2485    |  31195    |  898      |
| * neosigslot         |  12782     |  2494     |  2394    |  6849     |  930      |
| Boost Signals        |  7917      |  1630     |  586     |  4494     |  367      |
| * Boost Signals2     |  6121      |  1790     |  901     |  3162     |  461      |
+ -------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

Size Metrics
------------

| Library       [MSVC] | Object File Size ^ | Lines of Code |
| -------------------- | ------------------ | ------------- |
| [jeffomatic Jl_signal](https://github.com/jeffomatic/jl_signal) | 846 kb | ~ 6060 |
| [winglot Signals](https://github.com/winglot/Signals) | 904 kb | 427 |
| [Nano-signal-slot](https://github.com/NoAvailableAlias/nano-signal-slot) | 908 kb | 251 |
| [EvilTwin Fork](https://github.com/NoAvailableAlias/nano-signal-slot/blob/master/benchmark/lib/eviltwin/observer_fork.hpp) | 955 kb | 65 |
| [supergrover sigslot](https://github.com/supergrover/sigslot) | 966 kb | 185 |
| [pbhogan Signals](https://github.com/pbhogan/Signals) | 967 kb | ~ 2595 |
| [EvilTwin Observer](http://eviltwingames.com/blog/the-observer-pattern-revisited/) | 973 kb | 117 |
| [mwthinker Signal](https://github.com/mwthinker/Signal) | 975 kb | 171 |
| [amc522 Signal11](https://github.com/amc522/Signal11) | 981 kb | 730 |
| [joanrieu signal11](https://github.com/joanrieu/signal11) | 1,006 kb | 47 |
| [Boost Signals](http://www.boost.org/doc/libs/1_56_0/doc/html/signals.html) | 1,375 kb | N/A |
| [neosigslot](http://www.i42.co.uk/stuff/neosigslot.htm) | 1,940 kb | ~ 2496 |
| [Boost Signals2](http://www.boost.org/doc/libs/1_56_0/doc/html/signals2.html) | 2,350 kb | N/A |
