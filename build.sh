if (test -z ${GITHUB_WORKSPACE+x}) then
	GITHUB_WORKSPACE=$(pwd)
fi

if (test -z ${BUILD_TYPE+x}) then
	BUILD_TYPE="Debug"
fi

cmake \
	-B "${GITHUB_WORKSPACE}/build" \
	-DVCPKG_VERBOSE=ON \
	-DVCPKG_INSTALL_OPTIONS="--debug" \
	-DCMAKE_TOOLCHAIN_FILE="${GITHUB_WORKSPACE}/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	"${GITHUB_WORKSPACE}"

cmake --build "${GITHUB_WORKSPACE}/build" --config "${BUILD_TYPE}"
