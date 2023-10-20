# TinyORM Onboarding Experiment

The scope of this project is to provision TinyORM and all dependencies using VCPKG.

Run

    cmake --list-presets
    
to list the current CI presets. If any of the CI presets is representative enough, they can be used to build and run the tests using e.g.

    cmake --preset ci-ninja-x64-freebsd-release
    cmake --build --preset ci-ninja-x64-freebsd-release
    ctest --preset ci-ninja-x64-freebsd-release
    
to build using the `ci-ninja-x64-freebsd-release` preset.

If none of the presets is eligable, the `debug` and `release` presets can be used, though they don't configure a build path nor generator, and the ctest runner can't be used as straightforwardedly:

    cmake --preset release -B mybuildpath
    cmake --build mybuildpath --preset release
    pushd mybuildpath && ctest -C Release; popd
