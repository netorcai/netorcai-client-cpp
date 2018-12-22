project('netorcai-client', 'cpp',
    version: '1.0.0-dev',
    default_options: ['cpp_std=c++17'])

src = [
    'src/netorcai-client-cpp/client.hpp',
    'src/netorcai-client-cpp/client.cpp',
    'src/netorcai-client-cpp/error.hpp',
    'src/netorcai-client-cpp/message.hpp',
    'src/netorcai-client-cpp/message.cpp',
]

incdir = include_directories('src')

sfml_network_dep = dependency('sfml-network', required: true)
nlohmann_json_dep = dependency('nlohmann_json', required: true)

netorcai_client_cpp = library('netorcai-client',
    src,
    include_directories: [incdir],
    install: true, install_dir: 'lib',
    dependencies: [nlohmann_json_dep, sfml_network_dep]
)

netorcai_client_cpp_dep = declare_dependency(
    include_directories: [incdir],
    link_with: netorcai_client_cpp,
    sources: src,
    dependencies: [nlohmann_json_dep, sfml_network_dep]
)

install_headers(
    'src/netorcai-client-cpp/client.hpp',
    'src/netorcai-client-cpp/error.hpp',
    'src/netorcai-client-cpp/json.hpp',
    'src/netorcai-client-cpp/message.hpp',
    subdir: 'netorcai-client-cpp'
)

# Generate a pkg-config file
pkgc = import('pkgconfig')
pkgc.generate(name: 'netorcai-client-cpp',
    libraries: netorcai_client_cpp,
    version: meson.project_version(),
    requires: [sfml_network_dep, nlohmann_json_dep],
    description: 'A netorcai client library.'
)

# Unit tests.
gtest_dep = dependency('gtest_main', version: '>=1.8.0', required: false)

test_incdir = include_directories('test', 'src')
test_src = [
    'test/client.cpp',
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

    unittest_ndebug = executable('unittest-NDEBUG',
        test_src,
        override_options: ['b_ndebug=true'],
        dependencies: [gtest_dep, netorcai_client_cpp_dep],
        include_directories: [test_incdir]
    )

    # not parallel because of netorcai's shared TCP port
    test('unittest', unittest_debug, is_parallel : false)
    test('unittest (NDEBUG)', unittest_ndebug, is_parallel : false)
endif

# Doxygen documentation.
doxygen = find_program('doxygen', required: false)
if doxygen.found()
    run_target('doxygen-doc', command: 'doxygen/generate-doxydoc-meson.sh')
endif
