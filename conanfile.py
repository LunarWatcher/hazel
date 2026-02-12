from conan import ConanFile
import os
from conan.tools.files import copy
from pathlib import Path

class Hazel(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"


    def package_info(self):
        self.cpp_info.libs = ["libuuid"]

    # Currently synced with magpie's requirements {{{
    def requirements(self):
        self.requires("libnghttp2/1.66.0")
        self.requires("openssl/3.6.0")
        self.requires("asio/1.36.0")
        self.requires("zlib-ng/2.3.3")

        self.requires(
            "libcurl/8.17.0"
        )
        self.requires(
            "cpr/1.14.1"
        )

    def configure(self):
        # Conan's version of libcurl default-disables like all the protocols,
        # including http2. For obvious reasons, this is a tiny bit of a problem
        # when trying to use a libcurl-extension to test an HTTP/2 server
        self.options["libcurl"].with_nghttp2 = True
    # }}}
