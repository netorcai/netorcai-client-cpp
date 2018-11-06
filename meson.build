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

rapidjson = dependency('RapidJSON', version:'>=1.1.0', required: true)

lib = library('netorcai-client',
    src,
    include_directories: [incdir],
    install: true, install_dir: 'lib',
    dependencies: [rapidjson]
)

install_headers(
    'src/netorcai-client-cpp/client.hpp',
    'src/netorcai-client-cpp/error.hpp',
    'src/netorcai-client-cpp/message.hpp',
    subdir: 'netorcai-client-cpp'
)

pkgc = import('pkgconfig')
pkgc.generate(name: 'netorcai-client-cpp',
    libraries: lib,
    version: meson.project_version(),
    requires: [rapidjson],
    description: 'A netorcai client library.'
)

# Unit tests.
gtest = dependency('gtest_main', version: '>=1.8.0', required: false)

test_incdir = include_directories('test', 'src')
test_src = [
    'test/error.cpp',
    'test/message.cpp'
]

if gtest.found()
    unittest_debug = executable('unittest',
        test_src,
        override_options: ['b_ndebug=false'],
        dependencies: [gtest],
        include_directories: [test_incdir],
        link_with: lib
    )
    test('unittest', unittest_debug)

    unittest_ndebug = executable('unittest-NDEBUG',
        test_src,
        override_options: ['b_ndebug=true'],
        dependencies: [gtest],
        include_directories: [test_incdir],
        link_with: lib
    )
    test('unittest (NDEBUG)', unittest_ndebug)
endif
