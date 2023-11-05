# Contributing

Welcome stranger!

If you have come here to learn how to contribute to rtype, we have some tips for you!

First of all, don't hesitate to ask questions!
Use the [issue tracker](https://github.com/Tipbs/rtype/issues), no question is too simple.

### Issue assignment

**:warning: Important :warning:**

Before working on pull request, please ping us on the corresponding issue.
If there isn't already an open issue for what you want to work on, please open one first to see if it is something we would be available to review.

### Quickstart

rtype builds with CMake, [Conan](https://docs.conan.io/2/installation.html) and C++20
```sh
conan install . --build=missing --profile=default
cd build && cmake .. --preset=conan-release
cd .. && cmake --build --preset conan-release
```

### Documentation

- Networking : [RFC](https://github.com/Tipbs/rtype/blob/main/RFC.md)
- ECS : [Framework documentation](https://github.com/Tipbs/rtype/blob/main/docs/ecs/src/SUMMARY.md)

### Code Formatting

The project use clang format and github actions for code quality,
use the script to format your code before pushing
```sh
./tests/format-code.sh -f
```

### Making a pull-request

When you feel comfortable that your changes could be integrated into rtype, you can create a pull-request on GitHub.
One of the core maintainers will then approve the changes or request some changes before it gets merged.

That's it, happy contributions! :tada: :tada: :tada: