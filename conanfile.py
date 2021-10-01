from conans import ConanFile, CMake, tools
import os

class TomatoConan(ConanFile):
    name = "tomato"
    version = "0.6.6"
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
        "build_testing_coverage": [True, False],
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
        "build_testing_coverage": True,
        "use_tomato_private": False,
    }
    generators = "cmake_find_package"

    def configure(self):
        self.runCoverage = self.options.build_testing_coverage \
                           and tools.os_info.is_macos \
                           and self.settings.build_type == "Debug"

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
            self.build_requires("libyaml/0.2.5@%s/%s" % (self.user, self.channel))

    def source(self):
        self.run("git clone https://github.com/MRKonrad/tomato.git")
        if self.options.use_tomato_private:
            token = os.getenv("GH_PERSONAL_ACCESS_TOKEN")
            if not token:
                self.options.use_tomato_private=False
                return
            username = "MRKonrad:"+token+"@"
            self.run("git clone https://"+username+"github.com/MRKonrad/tomato_private")

    def _configure_cmake(self):
        cmake = CMake(self)

        if (tools.os_info.is_linux):
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = True

        cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        cmake.definitions["CMAKE_CXX_STANDARD"] = "11"
        cmake.definitions["USE_ITK"] = self.options.use_itk
        cmake.definitions["USE_VNL"] = self.options.use_vnl
        cmake.definitions["USE_PRIVATE_NR2"] = self.options.use_tomato_private
        cmake.definitions["USE_LMFIT"] = self.options.use_lmfit
        cmake.definitions["USE_TOMATOFIT"] = True
        cmake.definitions["USE_YAML"] = self.options.use_yaml or self.options.build_testing
        cmake.definitions["BUILD_APP"] = self.options.build_app
        cmake.definitions["BUILD_TESTING"] = self.options.build_testing
        if self.runCoverage:
            cmake.definitions["CMAKE_CXX_FLAGS"] = "-fprofile-instr-generate -fcoverage-mapping"
            cmake.definitions["CMAKE_C_FLAGS"] = "-fprofile-instr-generate -fcoverage-mapping"

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
            testingOutputFile = "reports/unitTestsReport_%s.log" % self.settings.build_type
            self.run("ctest -V -C %s --output-log %s" % (self.settings.build_type, testingOutputFile), cwd="tests", run_environment=True)

        # coverage
        if self.runCoverage:
            # find llvm-cov
            print("Get path to clang. llvm-cov should be in the same dir")
            clangPath = os.popen("xcodebuild -find clang").read()
            llvmCovLocation = os.path.dirname(clangPath)
            print("I assume that llvm-cov is here: " + llvmCovLocation)

            # llvm-profdata
            self.run("./TomatoTests", cwd="tests", run_environment=True)
            self.run("PATH=%s:$PATH llvm-profdata merge -sparse default.profraw -o default.profdata" % (llvmCovLocation), cwd="tests")

            # llvm-cov
            filesToBeReported = "%s/lib/*.cpp" % os.path.join(self.build_folder, self.name)
            coverageOutputFile = "reports/coverageReport.log"
            self.run("PATH=%s:$PATH llvm-cov report ./TomatoTests -instr-profile=default.profdata %s > %s" % (llvmCovLocation, filesToBeReported, coverageOutputFile), cwd="tests")
            self.run("cat %s" % coverageOutputFile, cwd="tests") # just to display results

    def package(self):
        cmake = self._configure_cmake()
        cmake.definitions["BUILD_TESTING"] = False
        cmake.definitions["USE_YAML"] = self.options.use_yaml
        if self.runCoverage:
            cmake.definitions["CMAKE_CXX_FLAGS"] = ""
            cmake.definitions["CMAKE_C_FLAGS"] = ""
        cmake.configure(source_folder=self.name)
        cmake.install()

        self.copy("*", dst="reports", src="tests/reports") # copy coverage results
        for bindir in self.deps_cpp_info.bindirs:
            self.copy("*.dll", dst="bin", src=bindir)

    def package_info(self):
        self.cpp_info.libs = ["TomatoLib"]
