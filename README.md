# hazel


Internal metaserver for my homelab. It's going to be a while before it's generally usable by anyone else, so the docs are going to be thin for the near foreseeable future.

## Requirements

* A Linux-based server
* C++20 compiler
* CMake 3.28 (Pro tip: [CMake is available via pip](https://pypi.org/project/cmake/))
* libpq-dev, uuid-dev, libasio-dev (Debian-based names; look up your distro's package manager for the applicable packages). Additional dependencies are sourced automagically during the build process
* Postgresql

