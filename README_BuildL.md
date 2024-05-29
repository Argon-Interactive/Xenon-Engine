# Building for Linux

## Required Packages (Inside the parethasis are the sugested versions)

- cmake (3.25)
- g++/clang (g++ 12.2 / clang 14.0.5 )
- ninja-build (1.11.1)
- xorg-dev

### (1)
Run: `"cmake . --preset <configuration>"`

With avalible configuraions being:
- unixlike-gcc-release
- unixlike-clang-release
- unixlike-gcc-debug
- unixlike-clang-debug

### (2)
Go into the `build/<configuraion>` folder and run: `"ninja"`

### (3)

The executable file can be found inside `build/<configuraion>/Test` directory