// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#define PRODUCTION
#include <gtest/gtest.h>
#include <gmock/gmock.h>


#define private public
#define protected public

#include "Utils/utils.h"

#undef private
#undef protected



CMRC_DECLARE(test_mesh_resources);

TEST(UtilsScanCmrcDirectoryProperFolders, RecursivelyFindsMatchingMeshFiles) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets", pattern, discovered_meshes);

  EXPECT_EQ(discovered_meshes.size(), 3);
  EXPECT_THAT(discovered_meshes, ::testing::UnorderedElementsAre(
    std::filesystem::path("test_assets/test_data/box.obj"),
    std::filesystem::path("test_assets/test_data/character.fbx"),
    std::filesystem::path("test_assets/test_data/nested/house.obj")
  ));
};

TEST(ScanCmrcDirectoryTestEmpty, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  Engine::Utils::scanCmrcDirectory(fs, "test_assets", R"(\.gltf$)", discovered_meshes);

  EXPECT_TRUE(discovered_meshes.empty());
};

TEST(ScanCmrcDirectoryTestNoPattern, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = "";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets", pattern, discovered_meshes);

  EXPECT_TRUE(discovered_meshes.empty());
};

TEST(ScanCmrcDirectoryTestFile, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets/test_data/box.obj", pattern, discovered_meshes);
  
  EXPECT_EQ(discovered_meshes.size(), 1);
  EXPECT_THAT(discovered_meshes, ::testing::UnorderedElementsAre(
    std::filesystem::path("test_assets/test_data/box.obj")
  ));
};

TEST(ScanCmrcDirectoryTestFileInvalidRegex, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets/test_data/box.gls", pattern, discovered_meshes);
  
  EXPECT_EQ(discovered_meshes.size(), 0);
};

TEST(ScanCmrcDirectoryTestFileNotExists, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets/test_data/boxe.obj", pattern, discovered_meshes);
  
  EXPECT_EQ(discovered_meshes.size(), 0);
};

TEST(ScanCmrcDirectoryTestNoPath, ReturnsEmptyWhenNoMatchesFound) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "", pattern, discovered_meshes);

  EXPECT_TRUE(discovered_meshes.empty());
};

TEST(UtilsScanCmrcDirectoryNotEmpty, RecursivelyFindsMatchingMeshFiles) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;
  discovered_meshes.emplace_back(std::filesystem::path("Err"));

  std::string pattern = R"(\.(obj|fbx)$)";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets", pattern, discovered_meshes);

  EXPECT_EQ(discovered_meshes.size(), 3);
  EXPECT_THAT(discovered_meshes, ::testing::UnorderedElementsAre(
    std::filesystem::path("test_assets/test_data/box.obj"),
    std::filesystem::path("test_assets/test_data/character.fbx"),
    std::filesystem::path("test_assets/test_data/nested/house.obj")
  ));
};

TEST(UtilsScanCmrcDirectoryNotEmptyEarlyExit, RecursivelyFindsMatchingMeshFiles) {
  auto fs = cmrc::test_mesh_resources::get_filesystem();
  std::vector<std::filesystem::path> discovered_meshes;
  discovered_meshes.emplace_back(std::filesystem::path("Err"));

  std::string pattern = "";
  Engine::Utils::scanCmrcDirectory(fs, "test_assets", pattern, discovered_meshes);

  EXPECT_EQ(discovered_meshes.size(), 0);
};