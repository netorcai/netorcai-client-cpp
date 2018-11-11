project('hello-player', 'cpp',
    default_options: ['cpp_std=c++17'],
    version: '1.0.0'
)

netorcai_client_cpp_dep = dependency('netorcai-client-cpp',
    fallback: ['netorcai-client-cpp', 'netorcai_client_cpp_dep']
)

player = executable('hello-player', ['hello-player.cpp'],
    dependencies: [netorcai_client_cpp_dep]
)
