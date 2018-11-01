project('netorcai-client', 'cpp', version: '1.0.0-dev')

src = [
    'src/client.hpp',
    'src/client.cpp',
    'src/message.hpp',
    'src/message.cpp',
]

incdir = include_directories('src')

lib = library('netorcai-client',
    src,
    include_directories: [incdir],
    install: true, install_dir: 'lib'
)

install_headers(
    'src/client.hpp',
    'src/message.hpp'
)

pkgc = import('pkgconfig')
pkgc.generate(name: 'netorcai-client-cpp',
    libraries: lib,
    version: meson.project_version(),
    description: 'A netorcai client library.'
)
