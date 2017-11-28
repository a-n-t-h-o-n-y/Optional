## Overview
A header only implementation of the Boost::Optional Library.

The Optional template class provides an interface for any type to allow its
objects to optionally contain a value. This is useful for a variety of reasons:
- Templated type does not need a default constructor.
- It expresses its intent and makes code more readable than pointers or other
work arounds such as `std::pair<T, bool>`.
- Expensive to construct objects are not created if not needed.
- Allocates memory on the stack.

Optional objects can be queried for validity by an explicit conversion to bool.
To access the underlying object, the dereference `operator*` can be used, or
`operator->` provides access to the underlying object’s members.

## Code Example
```cpp
opt::Optional<int> opt_int;
if (opt_int) {
    std::cout << "This is never reached.\n";
} else {
    *opt_int += 7;
}

opt::Optional<std::string> opt_str{"Hello, World!"};
if (opt_str) {
    std::cout << *opt_str << " is " << opt_str->size() << " bytes long.\n";
} else {
    std::cout << "This is never reached.\n";
}

opt::Optional<char> opt_char{opt::none};
if (!opt_char) {
    std::cout << "opt_char was initialized empty with opt::none.\n";
}
```

## Motivation
This implementation was created for for use in
[CPPurses](https://github.com/a-n-t-h-o-n-y/CPPurses).

## Installation
CMakeLists.txt included, it will generate files to install and build tests with:
```
mkdir build && cd build
cmake ..            # generate make files
make                # build tests
ctest               # run tests
sudo make install   # install header files to system include directory
```

## Documentation
Doxygen documentation can be found [here](
https://a-n-t-h-o-n-y.github.io/Optional/).

## Tests
Optional uses the google testing framework.

## License
This software is distributed under the [MIT License](LICENSE.txt).
