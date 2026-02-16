dir = include_directories('.')
cc = meson.get_compiler('c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'cpp')

fossil_game_lib = library('fossil_game',
    files(
        'player.c',
        'score.c',
        'quizzed.c'
    ),
    install: true,
    dependencies: [cc.find_library('m', required: false)],
    include_directories: dir)

fossil_game_dep = declare_dependency(
    link_with: [fossil_game_lib],
    include_directories: dir)

meson.override_dependency('fossil-game', fossil_game_dep)