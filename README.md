## Overview
A header only implementation of the Boost::Optional Library.

The Optional template class provides an interface for any type to allow its
objects to optionally contain a value. This is useful for a few reasons:
- Templated type does not need a default constructor.
- It expresses intent, making code more readable than pointers or other
workarounds such as `std::pair<T, bool>`.
- Expensive to construct objects are not created if not needed.
- No dynamic allocations.

Optional objects can be queried for validity by an explicit conversion to bool.
To access the underlying object, the dereference `operator*` can be used, or
`operator->` provides access to the object’s members.

## Code Example
```cpp
#include <ctime>
#include <iostream>
#include <optional/optional.hpp>
using opt::Optional;

Optional<std::time_t> get_time_if_even() {
    auto t = std::time(nullptr);
    if (t % 2 == 0) {
        return t;
    } else {
        return opt::none; 
    }
}

int main() {
    auto time_opt = get_time_if_even();
    if (time_opt) {
        std::cout << "The time was: " << *time_opt  << " s" << std::endl;
    } else {
        std::cout << "No time given." << std::endl;
    }
}
```

## Motivation
This implementation was created for use in
[CPPurses](https://github.com/a-n-t-h-o-n-y/CPPurses).

## Installation
CMakeLists.txt will generate files to install headers and build tests.
```
# In top level project directory
mkdir build && cd build
cmake ..            # generate make files
make testoptional   # build tests(optional)
ctest               # run tests(optional)
sudo make install   # install header files to system include directory
```

## Documentation
Doxygen documentation can be found [here](
https://a-n-t-h-o-n-y.github.io/Optional/).

## Tests
Optional depends on the google testing framework.

## License
This software is distributed under the [MIT License](LICENSE.txt).
