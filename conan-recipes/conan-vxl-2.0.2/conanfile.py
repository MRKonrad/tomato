from conans import ConanFile, CMake, tools

class VxlConan(ConanFile):
    name = "vxl"
    version = "2.0.2"
    license = "https://github.com/vxl/vxl/blob/master/core/vxl_copyright.h"
    url = "https://github.com/vxl/vxl"
    description = "VXL (the Vision-something-Libraries) is a collection of C++ libraries designed for computer vision research and implementation."
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports = "*.patch"

    def source(self):
        gittool = tools.Git(folder=self.name)
        gittool.clone(self.url)
        gittool.checkout("v" + self.version)

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["VXL_BUILD_CORE_NUMERICS_ONLY"] = "ON"
        cmake.definitions["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        tools.patch(base_path=self.name, patch_file="win_headers_install_path_fix.patch")
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["netlib", "v3p_netlib", "vcl", "vnl", "vnl_algo"]
        self.cpp_info.includedirs = [
            'include/vxl/core',
            'include/vxl/vcl',
            'include/vxl/v3p/netlib',
            'include/vxl/v3p']
