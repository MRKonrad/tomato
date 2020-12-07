from conans import ConanFile, CMake, tools


class TomatoConan(ConanFile):
    name = "tomato"
    version = "0.6.4"
    default_user = "user"
    default_channel = "testing"
    license = "MIT"
    author = "Konrad Werys, konradwerys2@gmail.com"
    url = "https://github.com/MRKonrad/tomato"
    description = "Cardiac MRI map calculation library"
    topics = ("Cardiac MRI", "MRI", "CMR", "paramteric mapping", "T1")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "use_itk": [True, False],
        "use_vnl": [True, False],
        "use_lmfit": [True, False],
        "use_yaml": [True, False],
        "build_app": [True, False],
        "build_testing": [True, False],
    }
    default_options = {
        "shared": False,
        "use_itk": False,
        "use_vnl": True,
        "use_lmfit": False,
        "use_yaml": True,
        "build_app": False,
        "build_testing": True,
    }
    generators = "cmake_find_package"

    def requirements(self):
        if self.options.use_itk:
            self.requires("itk/4.13.2@%s/%s" % (self.user, self.channel))
            self.options.use_vnl = True
        elif self.options.use_vnl:
            self.requires("vxl/1.18.0@%s/%s" % (self.user, self.channel))
        if self.options.use_lmfit:
            self.requires("lmfit/8.3.0@%s/%s" % (self.user, self.channel))
        if self.options.use_yaml:
            self.requires("libyaml/0.2.5@%s/%s" % (self.user, self.channel))
        if self.options.build_testing:
            self.requires("gtest/1.6.0@%s/%s" % (self.user, self.channel))

    def source(self):
        self.run("git clone https://github.com/MRKonrad/tomato.git --branch conan")

    def _configure_cmake(self):
        cmake = CMake(self)

        if (tools.os_info.is_macos):
            cmake.definitions["CMAKE_MACOSX_RPATH"] = "ON"

        if (tools.os_info.is_linux):
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"

        # cmake.definitions["CMAKE_CXX_STANDARD"] = "11"
        cmake.definitions["USE_ITK"] = self.options.use_itk
        cmake.definitions["USE_VNL"] = self.options.use_vnl
        cmake.definitions["USE_PRIVATE_NR2"] = "ON"
        cmake.definitions["USE_LMFIT"] = self.options.use_lmfit
        cmake.definitions["USE_TOMATOFIT"] = "ON"
        cmake.definitions["USE_YAML"] = self.options.use_yaml
        cmake.definitions["BUILD_APP"] = self.options.build_app
        cmake.definitions["BUILD_TESTING"] = self.options.build_testing

        cmake.configure(source_folder="tomato")
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
