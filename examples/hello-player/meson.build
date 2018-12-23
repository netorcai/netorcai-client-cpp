project('hello-player', 'cpp',
    default_options: ['cpp_std=c++17'],
    version: '1.0.0',
    meson_version: '>=0.46.0'
)

netorcai_client_cpp_dep = dependency('netorcai-client-cpp')
threads_dep = dependency('threads')

player = executable('hello-player', ['hello-player.cpp'],
    dependencies: [netorcai_client_cpp_dep, threads_dep]
)
