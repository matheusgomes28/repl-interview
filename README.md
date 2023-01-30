# CommandInterpreter - Repl in C++

This project contains my implementation of a general command interpreter for an interview coding challenge. It was all written in moden C++, where I decided to use the `std::variant` as the type-puning method for it's advantages in catching bugs at compilation time.

## Remarks

I cam accross some very interesting problems and decisions through the development of the interpreter. The list below summarises some of them

- Architecture of a future-proof API, allowing users to add commands with welll-supported argument types.
- Dynamically checking runtime command types.
- Type-puning techniques.
- String manipulation with modern C++ struct `std::string_view`.
- Non-blocking development of the command handlers.
- Deciding which structures to base command queues and maps.

In addition, I've credited a couple of posts about certain patterns in C++ that I've used in this project (small snippets). However, for clarificationa and disclaimers, the following techniques were used:

- [Visiting `std::variant` with the overload pattern in C++](https://www.modernescpp.com/index.php/visiting-a-std-variant-with-the-overload-pattern)
- [Speeding up `std::string_view` split implementation](https://www.cppstories.com/2018/07/string-view-perf-followup/)

# Build

To build the project, I'm assuming you have a modern-enough version of CMake (> 3.21) that supports presets.

To list all configuration presets, use the folowing command from the project's root

```sh
cmake . --list-presets
```

This will give you a list of the provided presets I've made available:

```sh
"ninja-unix-rel"    - Ninja Unix Release
"ninja-unix-deb"    - Ninja Unix Debug
"ninja-windows-rel" - Ninja Windows Release
"ninja-windows-deb" - Ninja Windows Debug
"unix-rel"          - Unix Make Release
"unix-deb"          - Unix Make Debug
"vs2022-rel"        - Visual Studio 2022 Release
"vs2022-deb"        - Visual Studio 2022 Debug
"vs2019-rel"        - Visual Studio 2019 Release
"vs2019-deb"        - Visual Studio 2019 Debug
```

Similarly, use `cmake --build --list-presets` to list the available build presets I've provided

```sh
"vs2022-rel"        - Visual Studio 2022 Release Build
"vs2022-deb"        - Visual Studio 2022 Debug Build
"vs2019-rel"        - Visual Studio 2019 Release Build
"vs2019-deb"        - Visual Studio 2019 Debug Build
"unix-deb"          - Unix Make Debug Build
"unix-rel"          - Unix Make Release Build
"ninja-unix-rel"    - Ninja Unix Release Build
"ninja-unix-deb"    - Ninja Unix Debug Build
"ninja-windows-rel" - Ninja Windows Release Build
"ninja-windows-deb" - Ninja Windows Debug Build
```

To configure using a preset, use `cmake . --preset "<PRESET_NAME>"`, choosing the preset from the list above. Similarly, to build use `cmake --build --preset "<PRESET_NAME>"`, which will build all available targets.

Unfortunately, to install you will need to call `cmake install` from the corresponding build directory for a preset, as CMake doesn't yet support install presets.

**Feel free to contact me in case your setup ins't accounted for in the presets provided. I'm happy to push a few changes to include your configuration so you can build it**

## Integrating To IDE's

As far as I know, most modern IDE's with CMake support already have support for presets. These include

- Visual Studio Code
- Visual Studio 2022
- CLion

So you could just open the CMake project in those IDE's and simply select the presets for building.

## Manual Building

If you don't want to use the presets provided, take a look at the `CMakePresets.json` to figure which flags and settings a preset uses for the configure and build calls.

Bear in mind that I've added all the flags and build settings mentioned in the specification, so you would obviously need to replicate that for your build.

For more information on how to read the presets, check out the [CMake presets documentation page](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

## Build Disclaimers

I've built and tested this project for both Windows & Unix x64 builds. All the presets will put your installation files into `<PROJECT_ROOT>/install/{rel,deb}` depending on whether you've build with Debug or Release presets.

Due to some questionable behaviour of the `MSCV` compiler, I've added the `/EHsc` flag to Windows builds as the use of `<iostream>` will trigger a warning regarding exceptions being thrown from `extern` definitions.

This was tested with:
- `gcc9`
- `msvc 19.29.30147.0`

I've not ran the code through compilers or static analysers. Specially, I've not compiled with `clang` so there may be warnings that those tools would catch with the warning flags provided in the specs. I do know how to fix these warnings but I didn't have time to extensive analyse the code with these tools. I'm happy to talk about my experience with these automation tools that can help improve code quality in a chat.

# Usage

Using the library is fairly straight forward. You need to:

- Intiialise with `CmdInt_Initialize(...)`
- Register function `CmdInt_RegisterCommand(...)`
- Queue Command `CmdInt_QueueCommand(...)`
- Optionally register callback with `CmInt_RegisterCallback(...)`.
- Poll to process with `CmdInt_Poll()`
- Shutdown with `CmdInt_Shutdown()`

For more information, take a look at sanity tests added in `SanityTests`.

# Project Logic & Decisions

I've decided to implement the interpreter as a command queue, where you can register commands that are stored in a `{str, command_function}` map.

I've used `std::variant` as our "everything" value holder, for type-puning. Given that future iterations of the library don't change the template parameters, the API functions should work ABI-wise. A bonus by using `std::variant` is that it should give you compilation issues if the user tries to change the argument types.


In addition, I've defined a few more headers/source files in the `src` directory for implementation:

- `CommandInterpreterImpl.cpp` contains definitions for the `CommandInterpreter` class, which holds a comand queue, command handler maps and callback. 
- `include/CommandInterpreter/CommandInterpreterImpl.cpp` is the public, non-exported header for the class above.
- `CommandExmaples.cpp` provides factory functions for the example commands given in the specification, so we can register them in `SanityTests`. Note that we don't have to provide these definitions, as we could have done them in the tests. I provided them for usage examples.
- `include/CommandInterpreter/CommandTypesHelper.h` holding some templated functions to help with parsing arguments. This is not exported.

And I've also added another exported header:
- `include/CommandInterpreter/CommandExamples.h` are the declarations of the exported command function factory functions for usage examples.

## What I Could Improve

- The API is essentially just a singleton, so I would export the class `CommandInterpreter` instead of the free functions. This would allow for multiple interpreter with different command queues, as well as cleaner RAII.
- The example functions can probably be simpler, I didn't have time to refactor all the code.
- Regarding the `countdown` command, it's essentially a hack that outputs every second by checking the current epoch agains the epoch when it was called. Everytime over a second passes, we add another `countdown` command with one less second to count xD. I understand that maybe you were looking for worker threads or better async programming, but I didn't have time to implement everything.
- Maybe creating transient targets to better split the helper code, again I wanted to hand this in sooner so I didn't put that much thought into the project layout.
- Documentation is important, but I mostly documented public functions and really hard to read code. I would have taken more time in a real-world project.
- Way more tests nd edge cases, there's gotta be bugs somewhere as I've implemented this over three days.


Many more things I've commented on, so please check comments.

# Disclaimer

I've obviously researched solutions for this type of problem. Essentially I came accros projects like the `lua` interpreter which do this on a much larger scale and robustness, to smaller C++ libraries that were very bloated and not worth mentioning here.

Furthermore, I've taken a look at their public API's and the core ideas, and passed some of them down to this implementation.

However, it's worth noting that all algorithms, architecture and design was done by me (with exception of the mentioned techiniques mentioned).
