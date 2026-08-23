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

TEST(ResourceControllerTwiceEmplace, HandlesInitialization){
  TestingRes test_res = TestingRes();
  test_res.str = "Hello";
  std::string test_res_name = "Hello";
  
  TestingRes test_res2 = TestingRes();
  test_res2.str = "Hello World";

  TestingRes test_res3 = TestingRes();
  test_res3.str = "Hello Big World";
  
  Engine::Utils::ResourceController<TestingRes> controller;
  controller.emplace_back(test_res_name, test_res);
  
  EXPECT_EQ(controller.size(), 1);
  EXPECT_TRUE(controller.exists(test_res_name));
  
  TestingRes test_res_return = controller.getResource(controller.getID(test_res_name));
  EXPECT_TRUE(test_res_return == test_res);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_TRUE(controller.exists(test_res_name));
  
  controller.emplace_back(test_res_name, test_res2);
  TestingRes test_res_return2 = controller.getResource(controller.getID(test_res_name));
  EXPECT_TRUE(test_res_return2 == test_res2);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_TRUE(controller.exists(test_res_name));
  
  controller.emplace_back(test_res_name, test_res3);
  TestingRes test_res_return3 = controller.getResource(controller.getID(test_res_name));
  EXPECT_TRUE(test_res_return3 == test_res3);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_TRUE(controller.exists(test_res_name));
};

TEST(ResourceControllerEraseMultipleTimes, HandlesInitialization){
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

  controller.erase(test_res_name);
  EXPECT_EQ(controller.size(), 2);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_TRUE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.erase(test_res_name2);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.erase(test_res_name);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.erase("empty");
  EXPECT_EQ(controller.size(), 1);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));

  controller.erase(test_res_name);
  EXPECT_EQ(controller.size(), 1);
  EXPECT_FALSE(controller.exists(test_res_name));
  EXPECT_FALSE(controller.exists(test_res_name2));
  EXPECT_TRUE(controller.exists(test_res_name3));
};

TEST(ResourceControllerEraseMultipleItems, HandlesInitialization){
  const unsigned int seed = 1231231;
  const unsigned int random_tests_min = 25;
  const unsigned int random_tests_max = 100;
  const float random_element_f_min = -200.0f;
  const float random_element_f_max = 200.0f;
  const int random_element_i_min = -200;
  const int random_element_i_max = 200;

  std::mt19937 gen_f(seed);
  std::uniform_real_distribution<float> dist_f(random_element_f_min, random_element_f_max);

  std::mt19937 gen_i(seed);
  std::uniform_int_distribution<int> dist_i(random_element_i_min, random_element_i_max);

  std::mt19937 gen_tests(seed);
  std::uniform_int_distribution<unsigned int> dist_tests(random_tests_min, random_tests_max);
  
  Engine::Utils::ResourceController<TestingRes> controller;
  std::vector<TestingRes> tests = {};
  
  unsigned int random_tests = dist_tests(gen_tests);
  for(unsigned int i = 0; i < random_tests; i++){
    TestingRes test = TestingRes();
    float rand_f = dist_f(gen_f);
    int rand_i = dist_i(gen_i);
    test.val_f = rand_f;
    test.val_i = rand_i;
    tests.emplace_back(test);
    std::string name = std::to_string(i);
    controller.emplace_back(name, test);
  };

  EXPECT_EQ(controller.size(), tests.size());

  for(unsigned int i = 0; i < random_tests; i++){
    std::string name = std::to_string(i);
    TestingRes test = controller.getResource(controller.getID(name));
    EXPECT_TRUE(test == tests[i]);
  };

  std::mt19937 gen_rm_tests(seed);
  std::uniform_int_distribution<unsigned int> dist_rm_tests(0, tests.size());
  
  unsigned int random_remove = dist_rm_tests(gen_rm_tests);
  for(unsigned int i = 0; i < random_remove; i++){
    std::string name = std::to_string(i);
    controller.erase(name);
  };

  EXPECT_EQ(controller.size(), tests.size() - random_remove);

  controller.clear();

  EXPECT_EQ(controller.size(), 0);
};

TEST(ResourceControllerEmplaceGetResourceIncorrectID, HandlesInitialization){
  TestingRes test_res = TestingRes();
  test_res.str = "Hello";
  std::string test_res_name = "Hello";
  
  Engine::Utils::ResourceController<TestingRes> controller;
  controller.emplace_back(test_res_name, test_res);

  EXPECT_EQ(controller.size(), 1);
  EXPECT_TRUE(controller.exists(test_res_name));
  
  TestingRes test_res_return = controller.getResource(controller.getID(test_res_name));
  EXPECT_TRUE(test_res_return == test_res);
  
  unsigned int test_id = 1231;
  EXPECT_THROW(controller.getResource(test_id), std::runtime_error);
  EXPECT_FALSE(controller.isIDValid(test_id));
  EXPECT_FALSE(controller.isIDValid(-1));
  EXPECT_TRUE(controller.isIDValid(controller.getID(test_res_name)));
};



// ================== //
// ==== Data Info === //
// ================== //
// getID not existing name


// integration with Resource