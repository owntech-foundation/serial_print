# SerialPrintChannels

SerialPrintChannels allows to register live float variables and stream either
their header row or their current values over the serial console.

## Load library

Place the library in your OwnTech `lib` folder or add it as a dependency in
your project layout.

## Include the library

```cpp
#include "SerialPrintChannels.h"
```

## SerialPrintChannels Usage

### Creating the printer

Create your print object:

```cpp
SerialPrintChannels print;
```

By default, the active capacity is `10` channels.

You can also choose a different default capacity:

```cpp
SerialPrintChannels print(16U);
```

### Configuring the printer

The active number of printable channels can be changed with:

```cpp
print.setCapacity(16U);
```

The separator used between fields can be changed with:

```cpp
print.setSeparator(":");
```

To remove all registered channels:

```cpp
print.clearChannels();
```

### Connecting channels

Channels are connected directly from float variables using:

```cpp
print.connectChannel(variable, "channel_name");
```

For example:

```cpp
print.connectChannel(app.variable.V_high, "V_high");
print.connectChannel(app.variable.speed_ref, "speed_ref");
```

Channels are printed in the same order as they are connected.

### Printing the header

To print the registered names:

```cpp
print.printHeader();
```

### Printing the values

To print the current values:

```cpp
print.printValues();
```

Each call prints one full row terminated by `\n`.
