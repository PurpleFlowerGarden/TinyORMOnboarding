if (${env:GITHUB_WORKSPACE} -eq ${null}) {
  ${env:GITHUB_WORKSPACE}=${pwd}
}

if (${env:BUILD_TYPE} -eq ${null}) {
  ${env:BUILD_TYPE}="Debug"
}

if (${env:VCPKG_TARGET_TRIPLET} -eq ${null}) {
  ${env:VCPKG_TARGET_TRIPLET}="x64-windows"
}

if ("x64-windows-static" -eq "${env:VCPKG_TARGET_TRIPLET}") {
  $msvc_runtime_dynamic="Off"
} else {
  $msvc_runtime_dynamic="On"
}

if ("x64-windows", "x64-windows-static" -contains "${env:VCPKG_TARGET_TRIPLET}") {
  $cmake_build_args="-v:n"
}

cmake `
	-B ${env:GITHUB_WORKSPACE}/build `
	-DVCPKG_VERBOSE=ON `
	-DVCPKG_INSTALL_OPTIONS="--debug" `
 	-DVCPKG_TARGET_TRIPLET="${env:VCPKG_TARGET_TRIPLET}" `
	-DCMAKE_TOOLCHAIN_FILE="${env:GITHUB_WORKSPACE}/vcpkg/scripts/buildsystems/vcpkg.cmake" `
	-DMSVC_RUNTIME_DYNAMIC="${msvc_runtime_dynamic}" `
	${env:GITHUB_WORKSPACE}

cmake --build ${env:GITHUB_WORKSPACE}/build --config ${env:BUILD_TYPE} --verbose -- ${cmake_build_args}
