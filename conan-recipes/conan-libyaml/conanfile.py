from conans import ConanFile, CMake, tools
import os


class LibYAMLConan(ConanFile):
    name = "libyaml"
    version = "0.2.5"
    description = "LibYAML is a YAML parser and emitter library."
    topics = ("conan", "libyaml", "yaml", "parser", "emitter")
    url = "https://github.com/bincrafters/conan-libyaml"
    homepage = "https://github.com/yaml/libyaml"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = "cmake"
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def configure(self):
        del self.settings.compiler.libcxx
        del self.settings.compiler.cppstd

    def source(self):
        url = "https://github.com/yaml/libyaml"
        gittool = tools.Git(folder=self.name)
        gittool.clone(url)
        gittool.checkout(self.version)

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = False
        cmake.definitions["YAML_STATIC_LIB_NAME"] = "yaml"
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
        if self.settings.compiler == "Visual Studio":
            self.cpp_info.defines = ["YAML_DECLARE_EXPORT"] if self.options.shared else ["YAML_DECLARE_STATIC"]