project('netorcai-client', 'cpp',
    version: '1.0.0-dev',
    default_options: ['cpp_std=c++11'])

src = [
    'src/netorcai-client-cpp/client.hpp',
    'src/netorcai-client-cpp/client.cpp',
    'src/netorcai-client-cpp/error.hpp',
    'src/netorcai-client-cpp/message.hpp',
    'src/netorcai-client-cpp/message.cpp',
]

incdir = include_directories('src/netorcai-client-cpp')

# Search for nlohmann_json in the system.
# Use the system version if posible, or get/use the latest release otherwise.
nlohmann_json_dep = dependency('nlohmann_json', required: false)

if nlohmann_json_dep.found()
    pkgconfig_dependencies = [nlohmann_json_dep]
else
    nlohmann_json_proj = subproject('nlohmann_json')
    nlohmann_json_dep = nlohmann_json_proj.get_variable('nlohmann_json_dep')
    pkgconfig_dependencies = []
    warning('If installing netorcai-client-cpp from this build, the installed netorcai-client-cpp.pc will NOT require nlohmann_json. This means netorcai-client-cpp.pc will be invalid if you explicitly uninstall nlohmann_json afterwards. If you prefer to generate a fully valid netorcai-client-cpp.pc, make sure that nlohmann_json is installed in your system (and that nlohmann_json.pc is reachable by pkg-config) before calling meson')
endif

netorcai_client_cpp = library('netorcai-client',
    src,
    include_directories: [incdir],
    install: true, install_dir: 'lib',
    dependencies: [nlohmann_json_dep]
)

netorcai_client_cpp_dep = declare_dependency(
    include_directories: [incdir],
    link_with: netorcai_client_cpp,
    sources: src,
    dependencies: [nlohmann_json_dep]
)

install_headers(
    'src/netorcai-client-cpp/client.hpp',
    'src/netorcai-client-cpp/error.hpp',
    'src/netorcai-client-cpp/message.hpp',
    subdir: 'netorcai-client-cpp'
)

# Generate a pkg-config file
pkgc = import('pkgconfig')
pkgc.generate(name: 'netorcai-client-cpp',
    libraries: netorcai_client_cpp,
    version: meson.project_version(),
    requires: pkgconfig_dependencies,
    description: 'A netorcai client library.'
)

# Unit tests.
gtest_dep = dependency('gtest_main', version: '>=1.8.0', required: false)

test_incdir = include_directories('test', 'src')
test_src = [
    'test/error.cpp',
    'test/message.cpp'
]

if gtest_dep.found()
    unittest_debug = executable('unittest',
        test_src,
        override_options: ['b_ndebug=false'],
        dependencies: [gtest_dep, netorcai_client_cpp_dep],
        include_directories: [test_incdir]
    )
    test('unittest', unittest_debug)

    unittest_ndebug = executable('unittest-NDEBUG',
        test_src,
        override_options: ['b_ndebug=true'],
        dependencies: [gtest_dep, netorcai_client_cpp_dep],
        include_directories: [test_incdir]
    )
    test('unittest (NDEBUG)', unittest_ndebug)
endif
