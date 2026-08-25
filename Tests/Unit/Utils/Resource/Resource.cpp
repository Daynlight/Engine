// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>

#define private public
#define protected public

#include "Utils/Resource/Resource.h"
#include "Utils/Resource/ResourceController.h"

#undef private
#undef protected



class TestingRes{
public:
  std::string str = "";
  int val_i = 0;
  float val_f = 0.0f;

public:
  TestingRes() = default;
  TestingRes(const TestingRes& second) noexcept
    : str(second.str),
      val_i(second.val_i),
      val_f(second.val_f) {};

  TestingRes& operator=(const TestingRes& second) noexcept {
    if(this == &second) return *this;
    
    str = second.str;
    val_i = second.val_i;
    val_f = second.val_f;

    return *this;
  };

  TestingRes(TestingRes&& second) noexcept
    : str(std::move(second.str)),
      val_i(std::move(second.val_i)),
      val_f(std::move(second.val_f)) {};

  TestingRes& operator=(TestingRes&& second) noexcept {
    if(this == &second) return *this;
    
    str = std::move(second.str);
    val_i = std::move(second.val_i);
    val_f = std::move(second.val_f);

    return *this;
  };

  bool operator==(const TestingRes& second) const {
    if(str != second.str) return false;
    if(val_i != second.val_i) return false;
    if(val_f != second.val_f) return false;
    return true;
  };
};



// ================== //
// == Constructors == //
// ================== //
TEST(ResourceDefaultConstructor, HandlesInitialization){
  Engine::Utils::Resource<TestingRes> resource;

  EXPECT_EQ(resource.controller, nullptr);
  EXPECT_EQ(resource.version, -1);
  EXPECT_EQ(resource.id, -1);
  EXPECT_EQ(resource.name, "");

  TestingRes* testing_record = resource.getResource();
  
  EXPECT_EQ(resource.controller, nullptr);
  EXPECT_EQ(resource.version, -1);
  EXPECT_EQ(resource.id, -1);
  EXPECT_EQ(resource.name, "");
  EXPECT_EQ(testing_record, nullptr);

  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record = TestingRes();
  std::string testing_str = "Hello";
  std::string testing_name = "Hello";
  record.str = testing_str;
  controller.emplace_back(testing_name, record);
  resource.setController(&controller);
  
  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, -1);
  EXPECT_EQ(resource.id, -1);
  EXPECT_EQ(resource.name, "");

  resource.setName(testing_name);

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion() - 1);
  EXPECT_EQ(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());

  testing_record = resource.getResource();

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion());
  EXPECT_NE(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());
  EXPECT_TRUE(*testing_record == record);
};

TEST(ResourceParamConstructor, HandlesInitialization){
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record = TestingRes();
  std::string testing_str = "Hello";
  std::string testing_name = "Hello";
  record.str = testing_str;
  controller.emplace_back(testing_name, record);

  Engine::Utils::Resource<TestingRes> resource(testing_name, &controller);

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion() - 1);
  EXPECT_EQ(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());

  TestingRes* testing_record = resource.getResource();

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion());
  EXPECT_NE(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());
  EXPECT_TRUE(*testing_record == record);
};

TEST(ResourceCopyConstructor, HandlesInitialization){
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record = TestingRes();
  std::string testing_str = "Hello";
  std::string testing_name = "Hello";
  record.str = testing_str;
  controller.emplace_back(testing_name, record);

  Engine::Utils::Resource<TestingRes> resource(testing_name, &controller);
  resource.getResource();

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion());
  EXPECT_NE(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());

  Engine::Utils::Resource<TestingRes> resource2(resource);
  
  EXPECT_EQ(resource.controller, resource2.controller);
  EXPECT_EQ(resource.version, resource2.version);
  EXPECT_EQ(resource.id, resource2.id);
  EXPECT_EQ(resource.name, resource2.name);
  EXPECT_TRUE(resource2.nameIsValid());

  Engine::Utils::Resource<TestingRes> resource3 = resource;
  
  EXPECT_EQ(resource.controller, resource3.controller);
  EXPECT_EQ(resource.version, resource3.version);
  EXPECT_EQ(resource.id, resource3.id);
  EXPECT_EQ(resource.name, resource3.name);
  EXPECT_TRUE(resource3.nameIsValid());

  Engine::Utils::Resource<TestingRes> resource4 = resource;
  Engine::Utils::Resource<TestingRes>* resource4_ptr = &resource4;
  resource4 = resource4;
  Engine::Utils::Resource<TestingRes>* resource4_ptr2 = &resource4;
  
  EXPECT_EQ(resource4_ptr, resource4_ptr2);
  EXPECT_EQ(resource.controller, resource4.controller);
  EXPECT_EQ(resource.version, resource4.version);
  EXPECT_EQ(resource.id, resource4.id);
  EXPECT_EQ(resource.name, resource4.name);
  EXPECT_TRUE(resource4.nameIsValid());
};

TEST(ResourceMoveConstructor, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record = TestingRes();
  std::string testing_str = "Hello";
  std::string testing_name = "Hello";
  record.str = testing_str;
  controller.emplace_back(testing_name, record);

  Engine::Utils::Resource<TestingRes> resource(testing_name, &controller);
  resource.getResource();

  EXPECT_NE(resource.controller, nullptr);
  EXPECT_EQ(resource.version, controller.getLatestsVersion());
  EXPECT_NE(resource.id, -1);
  EXPECT_EQ(resource.name, testing_name);
  EXPECT_TRUE(resource.nameIsValid());

  auto expected_controller = resource.controller;
  auto expected_version = resource.version;
  auto expected_id = resource.id;
  auto expected_name = resource.name;

  Engine::Utils::Resource<TestingRes> resource2(std::move(resource));
  
  EXPECT_EQ(expected_controller, resource2.controller);
  EXPECT_EQ(expected_version, resource2.version);
  EXPECT_EQ(expected_id, resource2.id);
  EXPECT_EQ(expected_name, resource2.name);
  EXPECT_TRUE(resource2.nameIsValid());
  EXPECT_EQ(resource.controller, nullptr);

  Engine::Utils::Resource<TestingRes> resource_source(testing_name, &controller);
  resource_source.getResource();

  Engine::Utils::Resource<TestingRes> resource3;
  resource3 = std::move(resource_source);
  
  EXPECT_EQ(expected_controller, resource3.controller);
  EXPECT_EQ(expected_version, resource3.version);
  EXPECT_EQ(expected_id, resource3.id);
  EXPECT_EQ(expected_name, resource3.name);
  EXPECT_TRUE(resource3.nameIsValid());
  EXPECT_EQ(resource_source.controller, nullptr);

  Engine::Utils::Resource<TestingRes> resource4(testing_name, &controller);
  resource4.getResource();
  Engine::Utils::Resource<TestingRes>* resource4_ptr = &resource4;
  resource4 = std::move(resource4);
  Engine::Utils::Resource<TestingRes>* resource4_ptr2 = &resource4;
  
  EXPECT_EQ(resource4_ptr, resource4_ptr2);
  EXPECT_EQ(expected_controller, resource4.controller);
  EXPECT_EQ(expected_version, resource4.version);
  EXPECT_EQ(expected_id, resource4.id);
  EXPECT_EQ(expected_name, resource4.name);
  EXPECT_TRUE(resource4.nameIsValid());
};



// ================== //
// == Data Control == //
// ================== //
TEST(ResourceGet, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record;
  record.str = "ValidResource";
  record.val_i = 42;
  std::string resource_name = "Res1";
  controller.emplace_back(resource_name, record);

  Engine::Utils::Resource<TestingRes> resource(resource_name, &controller);

  TestingRes* ptr = resource.getResource();
  ASSERT_NE(ptr, nullptr);
  EXPECT_EQ(ptr->str, "ValidResource");
  EXPECT_EQ(ptr->val_i, 42);
};

TEST(ResourceMassiveResourceGet, HandlesMassiveAddRemove) {
  Engine::Utils::ResourceController<TestingRes> controller;
  
  for(int i = 0; i < 500; ++i) {
    TestingRes record;
    record.str = "Data_" + std::to_string(i);
    record.val_i = i;
    controller.emplace_back("Res_" + std::to_string(i), record);
  };

  Engine::Utils::Resource<TestingRes> res_10("Res_10", &controller);
  Engine::Utils::Resource<TestingRes> res_250("Res_250", &controller);
  Engine::Utils::Resource<TestingRes> res_499("Res_499", &controller);

  ASSERT_NE(res_10.getResource(), nullptr);
  EXPECT_EQ(res_10.getResource()->val_i, 10);
  
  ASSERT_NE(res_250.getResource(), nullptr);
  EXPECT_EQ(res_250.getResource()->val_i, 250);

  for(int i = 500; i < 1000; ++i) {
    TestingRes record;
    record.str = "Data_" + std::to_string(i);
    record.val_i = i;
    controller.emplace_back("Res_" + std::to_string(i), record);
  };

  Engine::Utils::Resource<TestingRes> res_800("Res_800", &controller);

  ASSERT_NE(res_10.getResource(), nullptr);
  EXPECT_EQ(res_10.getResource()->val_i, 10);
  
  ASSERT_NE(res_250.getResource(), nullptr);
  EXPECT_EQ(res_250.getResource()->val_i, 250);
  
  ASSERT_NE(res_800.getResource(), nullptr);
  EXPECT_EQ(res_800.getResource()->val_i, 800);

  for(int i = 0; i < 200; ++i) controller.erase("Res_" + std::to_string(i)); 

  EXPECT_EQ(res_10.getResource(), nullptr);
  EXPECT_FALSE(res_10.nameIsValid());

  ASSERT_NE(res_250.getResource(), nullptr);
  EXPECT_EQ(res_250.getResource()->val_i, 250);

  ASSERT_NE(res_499.getResource(), nullptr);
  EXPECT_EQ(res_499.getResource()->val_i, 499);

  ASSERT_NE(res_800.getResource(), nullptr);
  EXPECT_EQ(res_800.getResource()->val_i, 800);

  for(int i = 400; i < 600; ++i) {
    controller.erase("Res_" + std::to_string(i)); 
  }

  EXPECT_EQ(res_499.getResource(), nullptr);
  
  ASSERT_NE(res_250.getResource(), nullptr);
  EXPECT_EQ(res_250.getResource()->val_i, 250);

  ASSERT_NE(res_800.getResource(), nullptr);
  EXPECT_EQ(res_800.getResource()->val_i, 800);
};

TEST(ResourceGetNotExisting, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  
  Engine::Utils::Resource<TestingRes> null_ctrl_res("NonExistent", nullptr);
  EXPECT_EQ(null_ctrl_res.getResource(), nullptr);

  Engine::Utils::Resource<TestingRes> missing_res("NonExistent", &controller);
  EXPECT_EQ(missing_res.getResource(), nullptr);
  EXPECT_FALSE(missing_res.nameIsValid());
};

TEST(ResourceSetGetName, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record1, record2;
  record1.str = "First";
  record2.str = "Second";

  controller.emplace_back("Res1", record1);
  controller.emplace_back("Res2", record2);

  Engine::Utils::Resource<TestingRes> resource("Res1", &controller);
  EXPECT_EQ(resource.getName(), "Res1");
  EXPECT_EQ(resource.getResource()->str, "First");

  resource.setName("Res2");
  EXPECT_EQ(resource.getName(), "Res2");
  EXPECT_EQ(resource.version, controller.getLatestsVersion() - 1);
  EXPECT_EQ(resource.getResource()->str, "Second");
};

TEST(ResourceSetGetController, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller1;
  Engine::Utils::ResourceController<TestingRes> controller2;

  TestingRes record;
  record.str = "Ctrl2Data";
  controller2.emplace_back("SharedName", record);

  Engine::Utils::Resource<TestingRes> resource;
  EXPECT_EQ(resource.getController(), nullptr);

  resource.setController(&controller1);
  EXPECT_EQ(resource.getController(), &controller1);

  resource.setController(&controller2);
  resource.setName("SharedName");
  EXPECT_EQ(resource.getController(), &controller2);
  ASSERT_NE(resource.getResource(), nullptr);
  EXPECT_EQ(resource.getResource()->str, "Ctrl2Data");
};



// ================ //
// == Validation == //
// ================ //
TEST(ResourceValidate, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  TestingRes record;
  record.str = "DynamicRes";
  controller.emplace_back("DynamicRes", record);

  Engine::Utils::Resource<TestingRes> resource("DynamicRes", &controller);

  EXPECT_TRUE(resource.validate());
  EXPECT_EQ(resource.version, controller.getLatestsVersion());
  EXPECT_NE(resource.id, -1);

  EXPECT_TRUE(resource.validate());
};

TEST(ResourceValidateNotExisting, HandlesInitialization) {
  Engine::Utils::ResourceController<TestingRes> controller;
  Engine::Utils::Resource<TestingRes> resource("Missing", &controller);

  EXPECT_FALSE(resource.validate());

  resource.setController(nullptr);
  EXPECT_FALSE(resource.validate());
};