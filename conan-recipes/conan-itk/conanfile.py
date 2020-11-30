from conans import ConanFile, CMake, tools


class itkConan(ConanFile):
    name = "itk"
    upstream_version = "4.13"
    version = "4.13.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "https://github.com/InsightSoftwareConsortium/ITK.git"
    description = "<Description of itk here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone --branch v{} {} {}".format(self.version, self.url, self.name))

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_EXAMPLES"] = "OFF"
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.definitions["INSTALL_GTEST"] = "OFF"
        cmake.definitions["ITK_BUILD_DEFAULT_MODULES"] = "ON"
        cmake.definitions["ITK_BUILD_DOCUMENTATION"] = "OFF"
        cmake.definitions["INSTALL_GTEST"] = "OFF"
        cmake.definitions["ITK_DOXYGEN_HTML"] = "OFF"
        cmake.definitions["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.includedirs = ['include/ITK-{0}'.format(self.upstream_version)]