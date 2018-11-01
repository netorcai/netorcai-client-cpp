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

# Coverage: add missing flags
# if meson.get_compiler('cpp').get_id() == 'gcc' and get_option('b_coverage')
#     coverage_extra_cpp_flags = ['-O0', '-fprofile-arcs', '-ftest-coverage']
#     coverage_extra_link_flags = ['-lgcov', '--coverage']
# else
#     coverage_extra_cpp_flags = ''
#     coverage_extra_link_flags = ''
# endif

incdir = include_directories('src/netorcai-client-cpp')

lib = library('netorcai-client',
    src,
    include_directories: [incdir],
    install: true, install_dir: 'lib'
    #cpp_args: coverage_extra_cpp_flags,
    #link_args: coverage_extra_link_flags
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
    description: 'A netorcai client library.'
)

# Unit tests.
test_incdir = include_directories('test', 'src')
test_src = [
    'test/catch.hpp',
    'test/error.cpp',
    'test/main.cpp',
    'test/message.cpp'
]

unittest_debug = executable('unittest',
    test_src,
    override_options: ['b_ndebug=false'],
    include_directories: [test_incdir],
    link_with: lib
)
test('unittest', unittest_debug)

unittest_ndebug = executable('unittest-NDEBUG',
    test_src,
    override_options: ['b_ndebug=true'],
    include_directories: [test_incdir],
    link_with: lib
)
test('unittest (NDEBUG)', unittest_ndebug)

