### Physics Simulations
This repository contains projects completed during the 2026/27 _Physical Simulations in
Virtual Environment_ course at _Warsaw University of Technology_.

The repository holds the following projects:

- **Spring** &mdash; spring simulation

The project uses C++23 with Qt 6 for windowing and OpenGL for
rendering where needed.

### Dependencies
The following libraries are utilized:

- Qt 6 &mdash; windowing
- Catch2 &mdash; testing
- cad_math (see [the repository](https://github.com/1180779/cad_math) for more details) &mdash; math
- cad_common (see [the repository](https://github.com/1180779/cad_common) for more details) &mdash; Qt helpers
- cad_graphics (see [the repository](https://github.com/1180779/cad_graphics) &mdash; graphics/OpenGL infrastructure
- oneTBB &mdash; dependency for `std::execution`
- OpenGL &mdash; rendering

### Setup
CMake 4.3 or newer is required to set up the project.

#### Hooks
The repository uses Git hooks where the owner finds them useful.
The following hooks are in place:

- Commit message hook: checks that each commit message line is no longer than
  72 characters and that there is an empty line between the title and description
