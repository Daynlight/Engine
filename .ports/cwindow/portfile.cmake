vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Daynlight/CWindow

  REF 5711afac494b3daf75e6fd7a95ff4bd3de04b467
  SHA512 "e5a312b074d3e6be1753e42d13e747e1d27c4b84747cda098eee7db01ff5bc966f58ee8847fe4d8649e53618241faf163fea352c864946c0ed5e74c4e7facc13"
  HEAD_REF engine
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DCWINDOW_BUILD_EXAMPLE=OFF
    -DCWINDOW_BUILD_TESTS=OFF
)

vcpkg_cmake_install()
