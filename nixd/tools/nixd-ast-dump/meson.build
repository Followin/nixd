nixd_ast_dump = executable('nixd-ast-dump', ['nixd-ast-dump.cpp']
, dependencies: [ boost
                , nixdDeps
                , llvm
                ] + nix_all
)

regression_env = environment()

regression_env.append('PATH', meson.current_build_dir())
regression_env.set('MESON_BUILD_ROOT', meson.current_build_dir())

if lit.found()
    test('regression/nixd-ast-dump'
        , lit
        , timeout: 120
        , env: regression_env
        , args: [ '-vv', meson.current_source_dir() + '/test' ]
        , depends: [ nixd_ast_dump ]
        )
endif
