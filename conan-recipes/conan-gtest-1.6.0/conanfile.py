from conans import ConanFile, CMake, tools


class GTestConan(ConanFile):
    name = "gtest"
    version = "1.6.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "https://github.com/google/googletest.git"
    description = "<Description of GTest here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone --branch release-{} {} {}".format(self.version, self.url, self.name))

    def _configure_cmake(self):
        cmake = CMake(self)
        # TODO: change it in the code as patch
        cmake.definitions["CMAKE_CXX_FLAGS"] = "-DGTEST_HAS_TR1_TUPLE=0"
        if self.settings.os == "Windows" and self.settings.get_safe("compiler.runtime"):
            cmake.definitions["gtest_force_shared_crt"] = "MD" in str(self.settings.compiler.runtime)
        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="gtest/include")
        self.copy("*gtest.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.defines.append("GTEST_HAS_TR1_TUPLE=0")
        if self.options.shared:
            self.cpp_info.defines.append("GTEST_LINKED_AS_SHARED_LIBRARY=1")
        if self.settings.os == "Linux":
            self.cpp_info.libs.append("pthread")