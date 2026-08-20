// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define PRODUCTION
#include <gtest/gtest.h>
#include <gmock/gmock.h>


#define private public
#define protected public

#include "Renderer.h"
#include "Utils/utils.h"

#undef private
#undef protected



CMRC_DECLARE(test_mesh_resources);

TEST(UtilsscanCmrcDirectory, RecursivelyFindsMatchingMeshFiles) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::string> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets", pattern, discovered_meshes);

  EXPECT_EQ(discovered_meshes.size(), 3);
  EXPECT_THAT(discovered_meshes, ::testing::UnorderedElementsAre(
    "test_assets/test_data/box.obj",
    "test_assets/test_data/character.fbx",
    "test_assets/test_data/nested/house.obj"
  ));
};

TEST(ScanCmrcDirectoryTest, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::string> discovered_meshes;

  Engine::Utils::scanCmrcDirectory(fs, "test_assets", R"(\.gltf$)", discovered_meshes);

  EXPECT_TRUE(discovered_meshes.empty());
};