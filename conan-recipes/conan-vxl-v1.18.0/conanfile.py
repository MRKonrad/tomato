from conans import ConanFile, CMake, tools


class VxlConan(ConanFile):
    name = "vxl"
    version = "v1.18.0"
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

        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.definitions["BUILD_CORE_GEOMETRY"] = "OFF"
        cmake.definitions["BUILD_CORE_IMAGING"] = "OFF"
        cmake.definitions["BUILD_CORE_NUMERICS"] = "ON"
        cmake.definitions["BUILD_CORE_SERIALISATION"] = "OFF"
        cmake.definitions["BUILD_CORE_UTILITIES"] = "OFF"
        cmake.definitions["VXL_USE_GEOTIFF"] = "OFF"
        cmake.definitions["VXL_USE_LFS"] = "OFF"

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
