from conans import ConanFile, CMake, tools


class LmfitConan(ConanFile):
    name = "lmfit"
    version = "8.3.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "https://jugit.fz-juelich.de/mlz/lmfit.git"
    description = "<Description of Lmfit here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone --branch v{} {} {}".format(self.version, self.url, self.name))

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["LIB_MAN"] = "OFF"
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
