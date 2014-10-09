Performance
-----------

**_Higher score is better._** _N / milliseconds per sample._

```
+ ------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect |  emission |  combined |
+ ------------------------------------------------------------------------------- +
| Jl_signal           |  143691    |  9269     |  47657   |  40178    |  6099     |
| amc522 Signal11     |  143131    |  5630     |  3650    |  32626    |  2057     |
| pbhogan Signals     |  130056    |  4052     |  3359    |  31002    |  1784     |
| EvilTwin Obs Fork   |  132496    |  3228     |  1737    |  18787    |  1068     |
| EvilTwin Observer   |  122390    |  2154     |  1083    |  19062    |  701      |
| supergrover sigslot |  12805     |  1159     |  2000    |  39298    |  638      |
| Nano-signal-slot    |  13744     |  4086     |  3932    |  28625    |  1711     |
| winglot Signals     |  5736      |  1960     |  2463    |  32491    |  895      |
| neosigslot          |  12740     |  2296     |  2101    |  6521     |  863      |
| Boost Signals       |  8726      |  1679     |  555     |  4436     |  364      |
| Boost Signals2      |  6668      |  1778     |  825     |  3212     |  442      |
+ ------------------------------------------------------------------------------- +
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
+ ------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect |  emission |  combined |
+ ------------------------------------------------------------------------------- +
| Jl_signal           |  144298    |  9223     |  48068   |  39582    |  6094     |
| amc522 Signal11     |  136080    |  5689     |  3627    |  33038    |  2056     |
| pbhogan Signals     |  130015    |  4077     |  3467    |  28973    |  1770     |
| EvilTwin Obs Fork   |  132444    |  3254     |  1691    |  19774    |  1054     |
| EvilTwin Observer   |  124332    |  2153     |  1092    |  19145    |  704      |
| Nano-signal-slot    |  69685     |  7862     |  8428    |  28609    |  3633     |
| supergrover sigslot |  12816     |  1162     |  2002    |  39690    |  665      |
| winglot Signals     |  5771      |  1954     |  2438    |  32407    |  893      |
| neosigslot          |  12236     |  2482     |  2234    |  6889     |  922      |
| Boost Signals       |  8674      |  1691     |  551     |  4258     |  363      |
| Boost Signals2      |  6998      |  1898     |  829     |  3179     |  449      |
+ ------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

Release Build Size
------------------

| Library | Object File Size |
| ------- | ---------------- |
| Jl_signal | 846 kb |
| winglot Signals | 904 kb |
| Nano-signal-slot | 908 kb |
| EvilTwin Obs Fork | 955 kb |
| supergrover sigslot | 966 kb |
| pbhogan Signals | 967 kb |
| EvilTwin Observer | 973 kb |
| amc522 Signal11 | 981 kb |
| Boost Signals | 1,375 kb |
| neosigslot | 1,940 kb |
| Boost Signals2 | 2,350 kb |
