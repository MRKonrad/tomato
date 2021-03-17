from conans import ConanFile, CMake, tools, RunEnvironment
import os

class TomatoConan(ConanFile):
    name = "tomato"
    version = "0.6.5"
    default_user = "user"
    default_channel = "testing"
    license = "MIT <https://github.com/MRKonrad/tomato/blob/master/LICENSE>"
    author = "<Konrad Werys>, <konradwerys2@gmail.com>"
    url = "https://github.com/MRKonrad/tomato"
    description = "Cardiac MRI map calculation library"
    topics = ("Cardiac MRI", "MRI", "CMR", "paramteric mapping", "T1")
    settings = "os", "compiler", "build_type", "arch"
    short_paths = True
    options = {
        "shared": [True, False],
        "use_itk": [True, False],
        "use_vnl": [True, False],
        "use_lmfit": [True, False],
        "use_yaml": [True, False],
        "build_app": [True, False],
        "build_testing": [True, False],
        "use_tomato_private": [True, False],
    }
    default_options = {
        "shared": False,
        "use_itk": False,
        "use_vnl": True,
        "use_lmfit": False,
        "use_yaml": False,
        "build_app": False,
        "build_testing": True,
        "use_tomato_private": False,
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

    def build_requirements(self):
        if self.options.build_testing:
            self.build_requires("gtest/1.6.0@%s/%s" % (self.user, self.channel))

    def source(self):
        self.run("git clone https://github.com/MRKonrad/tomato.git --branch T2")
        if self.options.use_tomato_private:
            token = os.getenv("GH_PERSONAL_ACCESS_TOKEN")
            if not token:
                self.options.use_tomato_private=False
                return
            username = "MRKonrad:"+token+"@"
            self.run("git clone https://"+username+"github.com/MRKonrad/tomato_private --branch T2")

    def _configure_cmake(self):
        cmake = CMake(self)

        cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        cmake.definitions["BUILD_SHARED_LIBS"] = self.options.shared

        if (tools.os_info.is_macos):
            cmake.definitions["CMAKE_MACOSX_RPATH"] = "ON"

        if (tools.os_info.is_linux):
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"

        #cmake.definitions["CMAKE_CXX_STANDARD"] = "11"
        cmake.definitions["USE_ITK"] = self.options.use_itk
        cmake.definitions["USE_VNL"] = self.options.use_vnl
        cmake.definitions["USE_PRIVATE_NR2"] = self.options.use_tomato_private
        cmake.definitions["USE_LMFIT"] = self.options.use_lmfit
        cmake.definitions["USE_TOMATOFIT"] = True
        cmake.definitions["USE_YAML"] = self.options.use_yaml
        cmake.definitions["BUILD_APP"] = self.options.build_app
        cmake.definitions["BUILD_TESTING"] = self.options.build_testing

        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        # run tests
        if self.options.build_testing:
            # tell test executable where to look for the shared libs generated during build
            if self.settings.os == "Windows" and self.options.shared:
                os.environ['PATH'] += ";%s\%s"%(self.build_folder, self.settings.build_type)
            self.run("ctest -V -C %s" % (self.settings.build_type), cwd="tests", run_environment=True)

    def package(self):
        cmake = self._configure_cmake()
        cmake.definitions["BUILD_TESTING"] = False
        cmake.configure(source_folder=self.name)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["TomatoLib"]
