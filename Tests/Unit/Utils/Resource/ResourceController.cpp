// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <string>
#include <vector>

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
TEST(ResourceControllerDefaultConstructor, HandlesInitialization){
};

TEST(ResourceControllerCopyConstructor, HandlesInitialization){
};

TEST(ResourceControllerCopyAssignConstructor, HandlesInitialization){
};

TEST(ResourceControllerMoveConstructor, HandlesInitialization){
};

TEST(ResourceControllerMoveAssignConstructor, HandlesInitialization){
};



// ================== //
// == Data Control == //
// ================== //
TEST(ResourceControllerEmplaceViaMoveEraseClear, HandlesInitialization){
  TestingRes test_res = TestingRes();
  test_res.str = "Hello";
  std::string test_res_name = "Hello";
  
  TestingRes test_res2 = TestingRes();
  test_res2.str = "Hello World";
  std::string test_res_name2 = "Hello World";

  TestingRes test_res3 = TestingRes();
  test_res3.str = "Hello Big World";
  std::string test_res_name3 = "Hello Big World";
  
  Engine::Utils::ResourceController<TestingRes> controller;
  controller.emplace_back(test_res_name, std::move(test_res));
  controller.emplace_back(test_res_name2, std::move(test_res2));
  controller.emplace_back(test_res_name3, std::move(test_res3));

  EXPECT_EQ(controller.size(), 3);
  EXPECT_TRUE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));
  
  controller.erase(test_res_name);
  EXPECT_EQ(controller.size(), 2);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.clear();
  EXPECT_EQ(controller.size(), 0);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_FALSE(controller.exists(test_res_name3));
};

TEST(ResourceControllerEmplaceViaCopyEraseClear, HandlesInitialization){
  TestingRes test_res = TestingRes();
  test_res.str = "Hello";
  std::string test_res_name = "Hello";
  
  TestingRes test_res2 = TestingRes();
  test_res2.str = "Hello World";
  std::string test_res_name2 = "Hello World";

  TestingRes test_res3 = TestingRes();
  test_res3.str = "Hello Big World";
  std::string test_res_name3 = "Hello Big World";
  
  Engine::Utils::ResourceController<TestingRes> controller;
  controller.emplace_back(test_res_name, test_res);
  controller.emplace_back(test_res_name2, test_res2);
  controller.emplace_back(test_res_name3, test_res3);

  EXPECT_EQ(controller.size(), 3);
  EXPECT_TRUE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));
  
  controller.erase(test_res_name);
  EXPECT_EQ(controller.size(), 2);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.clear();
  EXPECT_EQ(controller.size(), 0);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_FALSE(controller.exists(test_res_name3));
};

TEST(ResourceControllerEmplaceGetResource, HandlesInitialization){
  TestingRes test_res = TestingRes();
  test_res.str = "Hello";
  std::string test_res_name = "Hello";
  
  TestingRes test_res2 = TestingRes();
  test_res2.str = "Hello World";
  std::string test_res_name2 = "Hello World";

  TestingRes test_res3 = TestingRes();
  test_res3.str = "Hello Big World";
  std::string test_res_name3 = "Hello Big World";
  
  Engine::Utils::ResourceController<TestingRes> controller;
  controller.emplace_back(test_res_name, test_res);
  controller.emplace_back(test_res_name2, test_res2);
  controller.emplace_back(test_res_name3, test_res3);

  EXPECT_EQ(controller.size(), 3);
  EXPECT_TRUE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));
  
  TestingRes test_res_return = controller.getResource(controller.getID(test_res_name));
  EXPECT_TRUE(test_res_return == test_res);
  
  TestingRes test_res_return2 = controller.getResource(controller.getID(test_res_name2));
  EXPECT_TRUE(test_res_return2 == test_res2);

  TestingRes test_res_return3 = controller.getResource(controller.getID(test_res_name3));
  EXPECT_TRUE(test_res_return3 == test_res3);
};

// emplace two the same
// erase twice the same
// erase not existing one
// random amount elements clear
// getResource + getID with erasing element before
// getResource out of bound
// integration with Resource

// getID not existing name