project('hello-gl', 'cpp',
    default_options: ['cpp_std=c++17'],
    version: '1.0.0',
    meson_version: '>=0.46.0'
)

netorcai_client_cpp_dep = dependency('netorcai-client-cpp')
threads_dep = dependency('threads')

gl = executable('hello-gl', ['hello-gl.cpp'],
    dependencies: [netorcai_client_cpp_dep, threads_dep]
)
