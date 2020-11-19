from conans import ConanFile, CMake, tools


class VxlConan(ConanFile):
    name = "vxl"
    version = "v2.0.2"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        url = "https://github.com/vxl/vxl.git"
        gittool = tools.Git(folder=self.name)
        gittool.clone(url)
        gittool.checkout(self.version)

    def _configure_cmake(self):
        cmake = CMake(self)

        cmake.definitions["VXL_BUILD_CORE_NUMERICS_ONLY"] = "ON"

        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["netlib", "v3p_netlib", "vcl", "vnl", "vnl_algo"]
