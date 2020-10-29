from conans import ConanFile, CMake, tools


class LmfitConan(ConanFile):
    name = "lmfit"
    version = "v8.3.0"
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
        self.run("git clone --branch {} {} {}".format(self.version, self.url, self.name))
        ## this does not work on linux
        ## TODO: try https://askubuntu.com/questions/186847/error-gnutls-handshake-failed-when-connecting-to-https-servers
        # gittool = tools.Git(folder=self.name, verify_ssl=False)
        # gittool.clone(self.url)
        # gittool.checkout(self.version)

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package(self):
        self.copy("*.h", dst="include", src="lmfit/lib")
        self.copy("*lmfit.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["lmfit"]

