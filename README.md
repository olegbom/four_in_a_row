# Example of a project using the can_transport_protocol library

## Build and run tests

If you want to build can_transport_protocol as DLL use for generate Makefiles:

```sh
cmake -S . -B build -D CTP_SHARED=ON
```

Otherwise:

```sh
cmake -S . -B build
```

And build

```sh
cmake --build build
```

Launch tests:

```sh
ctest --test-dir build --build-config debug --verbose
```
