# hazel

Internal metaserver for my homelab. It's going to be a while before it's generally usable by anyone else, so the docs are going to be thin for the near foreseeable future, and stuff will be ugly, broken, or both.

The entire theme is scope creep, so Hazel is designed for a lot of random shit I need, but don't feel like making an entire dedicated project for. Some planned ideas include:

* The obligatory link dashboard
* Status monitoring
    * The obligatory real-time (ish) overview
        * Practical monitoring of stuff like miniflux unreads
        * System information
        * Service information
    * Long-running job tracking (with NTFY integration for failure or completion)
* Automation with scripting for easier expansion

The overall goal of the project is primarily to join together other self-hosted services, and to implement a few things from scratch. But for the most part, where possible, main functionality is deferred to other (primarily self-hosted) services, with Hazel serving as an aggregator.

## Requirements

* A Linux-based server; no other operating systems are or will be supported
* C++20 compiler
* CMake 3.28 (Pro tip: [CMake is available via pip](https://pypi.org/project/cmake/))
* Conan, or:
    * nghttp2
    * openssl
    * zlib-ng
    * asio (NOT the boost version!)
    * libcurl
    * libcpr
    * Ubuntu does not provide libcpr, nor a new enough version of nghttp2. Using conan is strongly recommended.
* Required with or without conan: libuuid-dev
