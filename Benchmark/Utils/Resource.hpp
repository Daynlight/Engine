// Engine
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <chrono>
#include <unordered_map>
#include <string>
#include <random>

#include "Utils/Resource/ResourceController.h"
#include "Utils/Resource/Resource.h"



// Unified Interface
template<typename T>
class BenchmarkingControllerInterface{
public:
  virtual void emplace_back(const std::string& name, const T& record) noexcept = 0;
  virtual void emplace_back(const std::string& name, T&& record) noexcept = 0;
  virtual void erase(const std::string& name) noexcept = 0;
  virtual void clear() noexcept = 0;
};

template<typename T>
class BenchmarkingResourceInterface{
public:
  virtual T* getResource() noexcept = 0;
  virtual std::string getName() const noexcept = 0;
  virtual void setName(const std::string& name) noexcept = 0;
  virtual bool nameIsValid() const noexcept = 0;
  virtual void setController(BenchmarkingControllerInterface<T>* controller) noexcept = 0;
  virtual BenchmarkingControllerInterface<T>* getController() noexcept = 0;
};



// unordered_map
template<typename T>
class BenchmarkingUnorderedMapResourceController : public BenchmarkingControllerInterface<T> {
public:
  std::unordered_map<std::string, T> data;

public:
  void emplace_back(const std::string& name, const T& record) noexcept{
    data[name] = record;
  };
  void emplace_back(const std::string& name, T&& record) noexcept{
    data[name] = std::move(record);
  };
  void erase(const std::string& name) noexcept{
    data.erase(name);
  };
  void clear() noexcept{
    data.clear();
  };
};

template<typename T>
class BenchmarkingUnorderedMapResource : public BenchmarkingResourceInterface<T> {
public:
  BenchmarkingUnorderedMapResourceController<T>* data;
  std::string active_name = "";

public:
  T* getResource() noexcept {
    auto it = data->data.find(active_name);
    if(it == data->data.end()) return nullptr;
    else return &(data->data[active_name]);
  };
  std::string getName() const noexcept{
    return active_name;
  };
  void setName(const std::string& name) noexcept{
    active_name = name;
  };
  bool nameIsValid() const noexcept{
    auto it = data->data.find(active_name);
    if(it == data->data.end()) return false;
    return true;
  };
  void setController(BenchmarkingControllerInterface<T>* controller) noexcept{
    // data = controller;
  };
  BenchmarkingControllerInterface<T>* getController() noexcept{
    return data;
  };
};

// resource and resource_controller
template<typename T>
class BenchmarkingResourceController : public BenchmarkingControllerInterface<T>{
public:
  Engine::Utils::ResourceController<T> controller;
public:
  void emplace_back(const std::string& name, const T& record) noexcept{
    controller.emplace_back(name, record);
  };
  void emplace_back(const std::string& name, T&& record) noexcept{
    controller.emplace_back(name, std::move(record));  
  };
  void erase(const std::string& name) noexcept{
    controller.erase(name);
  };
  void clear() noexcept{
    controller.clear();
  };
};

template<typename T>
class BenchmarkingResource : public BenchmarkingResourceInterface<T>{
public:
  Engine::Utils::Resource<T> resource;
  BenchmarkingControllerInterface<T>* controller = nullptr;
public:
  T* getResource() noexcept{
    return resource.getResource();
  };
  std::string getName() const noexcept{
    return resource.getName();
  };
  void setName(const std::string& name) noexcept{
    resource.setName(name);
  };
  bool nameIsValid() const noexcept{
    return resource.nameIsValid();
  };
  void setController(BenchmarkingControllerInterface<T>* controller) noexcept{
    this->controller = controller;
  };
  BenchmarkingControllerInterface<T>* getController() noexcept{
    return controller;
  };
};



class BenchmarkData{
public:
  int val = 0;

public:
  BenchmarkData() noexcept {};
  ~BenchmarkData() noexcept {};
  BenchmarkData(const BenchmarkData& second) noexcept
  : val(second.val) {};
  BenchmarkData& operator=(const BenchmarkData& second) noexcept {
    if(this == &second) return *this;
    val = second.val;
    return *this;
  };
  BenchmarkData(BenchmarkData&& second) noexcept
  : val(std::move(second.val)) {};
  BenchmarkData& operator=(BenchmarkData&& second) noexcept {
    if(this == &second) return *this;
    val = std::move(second.val);
    return *this;
  };

  bool operator==(const BenchmarkData& second) const noexcept {
    if(val != second.val) return false;
    return true;
  };
};



int benchmarkRealWorkloadWithFocusOnGetResource(BenchmarkingControllerInterface<BenchmarkData>& controller, BenchmarkingResourceInterface<BenchmarkData>& resource) {
  const unsigned int total_operations = 1000000;
  const int key_pool_size = 5000;

  const unsigned int emplace_item_prob = 10;
  const unsigned int emplace_move_item_prob = 10 + emplace_item_prob;
  const unsigned int set_name_prob = 10 + emplace_move_item_prob;
  const unsigned int get_resource_prob = 50 + set_name_prob;
  const unsigned int erase_prob = 10 + get_resource_prob;
  const unsigned int clear_prob = 10 + erase_prob;
  
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> op_dist(0, 99);
  std::uniform_int_distribution<int> key_dist(0, key_pool_size - 1);
  std::uniform_int_distribution<int> val_dist(1, 1000000);

  int checksum = 0;

  for (unsigned int i = 0; i < total_operations; ++i) {
    const int op = op_dist(rng);
    const std::string key = std::to_string(key_dist(rng));

    if (op < emplace_item_prob) {
      BenchmarkData data;
      data.val = val_dist(rng);
      controller.emplace_back(key, data);
    } 
    else if (op < emplace_move_item_prob) {
      BenchmarkData data;
      data.val = val_dist(rng);
      controller.emplace_back(key, std::move(data));
    } 
    else if(op < set_name_prob){
      resource.setName(key);
    }
    else if (op < get_resource_prob) {
      BenchmarkData* ptr = resource.getResource();
      if (ptr != nullptr) checksum = checksum + ptr->val;
    } 
    else if (op < erase_prob) {
      controller.erase(key);
    } 
    else if(op < clear_prob){
      controller.clear();
    };
  };
  
  return checksum;
};

int benchmarkEmplaceThenGet(BenchmarkingControllerInterface<BenchmarkData>& controller, BenchmarkingResourceInterface<BenchmarkData>& resource) {
  const unsigned int total_operations = 1000000;
  const int key_pool_size = 5000;
  
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> op_dist(0, 99);
  std::uniform_int_distribution<int> key_dist(0, key_pool_size - 1);
  std::uniform_int_distribution<int> val_dist(1, 1000000);

  int checksum = 0;

  for (unsigned int i = 0; i < key_pool_size; ++i) {
    const int op = op_dist(rng);
    const std::string key = std::to_string(i);

    if (op < 50) {
      BenchmarkData data;
      data.val = val_dist(rng);
      controller.emplace_back(key, data);
    } 
    else {
      BenchmarkData data;
      data.val = val_dist(rng);
      controller.emplace_back(key, std::move(data));
    };
  };

  for (unsigned int i = 0; i < total_operations; ++i) {
    const int op = op_dist(rng);
    const std::string key = std::to_string(key_dist(rng));

    if(op < 5){
      resource.setName(key);
    }
    else {
      BenchmarkData* ptr = resource.getResource();
      if (ptr != nullptr) checksum = checksum + ptr->val;
    };
  };

  return checksum;
};

int benchmarkDirectLookup(BenchmarkingControllerInterface<BenchmarkData>& controller, BenchmarkingResourceInterface<BenchmarkData>& resource) {
  const unsigned int total_operations = 1000000;
  const int key_pool_size = 5000;

  std::mt19937 rng(42);
  std::uniform_int_distribution<int> key_dist(0, key_pool_size - 1);

  int checksum = 0;

  for (int i = 0; i < key_pool_size; ++i) {
    const std::string key = std::to_string(i);

    BenchmarkData data;
    data.val = i;

    controller.emplace_back(key, std::move(data));
  };

  resource.setName("2500");

  for (unsigned int i = 0; i < total_operations; ++i) {
    BenchmarkData* ptr = resource.getResource();
    if (ptr != nullptr) checksum += ptr->val;
  };

  return checksum;
};



void benchmarkResourceVSUnorderedMap(){
  {
    fmt::println(fg(fmt::color::blue) | fmt::emphasis::bold, "Benchmark Real Workload With Focus On Get Resource");
    std::chrono::duration<double, std::milli> unordered_map_duration;
    std::chrono::duration<double, std::milli> resource_duration;
    int unordered_map_checksum = 0;
    int resource_checksum = 0;

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingUnorderedMapResourceController<BenchmarkData> resource_controller_unordered_map_benchmark;
      BenchmarkingUnorderedMapResource<BenchmarkData> resource_unordered_map_benchmark;
      resource_unordered_map_benchmark.data = &resource_controller_unordered_map_benchmark;
      unordered_map_checksum = benchmarkRealWorkloadWithFocusOnGetResource(resource_controller_unordered_map_benchmark, resource_unordered_map_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      unordered_map_duration = test_end - test_start;
    }

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingResourceController<BenchmarkData> resource_controller_benchmark;
      BenchmarkingResource<BenchmarkData> resource_benchmark;
      resource_benchmark.resource.setController(&resource_controller_benchmark.controller);
      resource_checksum = benchmarkRealWorkloadWithFocusOnGetResource(resource_controller_benchmark, resource_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      resource_duration = test_end - test_start;
    }

    if(unordered_map_duration.count() < resource_duration.count())
      fmt::println(fg(fmt::color::green), "Unordered: {}", unordered_map_duration);
    else
      fmt::println(fg(fmt::color::red), "Unordered: {}", unordered_map_duration);
      
    if(unordered_map_duration.count() > resource_duration.count())
      fmt::println(fg(fmt::color::green), "Resource: {}", resource_duration);
    else
      fmt::println(fg(fmt::color::red), "Resource: {}", resource_duration);

    fmt::println(fg(fmt::color::purple), "Unordered - Resource = {}", unordered_map_duration - resource_duration);
    fmt::println(fg(fmt::color::purple), "Unordered / Resource = {}", unordered_map_duration / resource_duration);

    fmt::println(fg(fmt::color::yellow), "Unordered checksum = {}", unordered_map_checksum);
    fmt::println(fg(fmt::color::yellow), "Resource checksum = {}", resource_checksum);
    
    fmt::println("");
  }

  {
    fmt::println(fg(fmt::color::blue) | fmt::emphasis::bold, "Benchmark Emplace Then Get");
    std::chrono::duration<double, std::milli> unordered_map_duration;
    std::chrono::duration<double, std::milli> resource_duration;
    int unordered_map_checksum = 0;
    int resource_checksum = 0;

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingUnorderedMapResourceController<BenchmarkData> resource_controller_unordered_map_benchmark;
      BenchmarkingUnorderedMapResource<BenchmarkData> resource_unordered_map_benchmark;
      resource_unordered_map_benchmark.data = &resource_controller_unordered_map_benchmark;
      unordered_map_checksum = benchmarkEmplaceThenGet(resource_controller_unordered_map_benchmark, resource_unordered_map_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      unordered_map_duration = test_end - test_start;
    }

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingResourceController<BenchmarkData> resource_controller_benchmark;
      BenchmarkingResource<BenchmarkData> resource_benchmark;
      resource_benchmark.resource.setController(&resource_controller_benchmark.controller);
      resource_checksum = benchmarkEmplaceThenGet(resource_controller_benchmark, resource_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      resource_duration = test_end - test_start;
    }

    if(unordered_map_duration.count() < resource_duration.count())
      fmt::println(fg(fmt::color::green), "Unordered: {}", unordered_map_duration);
    else
      fmt::println(fg(fmt::color::red), "Unordered: {}", unordered_map_duration);
      
    if(unordered_map_duration.count() > resource_duration.count())
      fmt::println(fg(fmt::color::green), "Resource: {}", resource_duration);
    else
      fmt::println(fg(fmt::color::red), "Resource: {}", resource_duration);

    fmt::println(fg(fmt::color::purple), "Unordered - Resource = {}", unordered_map_duration - resource_duration);
    fmt::println(fg(fmt::color::purple), "Unordered / Resource = {}", unordered_map_duration / resource_duration);

    fmt::println(fg(fmt::color::yellow), "Unordered checksum = {}", unordered_map_checksum);
    fmt::println(fg(fmt::color::yellow), "Resource checksum = {}", resource_checksum);
    
    fmt::println("");
  }

  {
    fmt::println(fg(fmt::color::blue) | fmt::emphasis::bold, "Benchmark Direct Lookup");
    std::chrono::duration<double, std::milli> unordered_map_duration;
    std::chrono::duration<double, std::milli> resource_duration;
    int unordered_map_checksum = 0;
    int resource_checksum = 0;

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingUnorderedMapResourceController<BenchmarkData> resource_controller_unordered_map_benchmark;
      BenchmarkingUnorderedMapResource<BenchmarkData> resource_unordered_map_benchmark;
      resource_unordered_map_benchmark.data = &resource_controller_unordered_map_benchmark;
      unordered_map_checksum = benchmarkDirectLookup(resource_controller_unordered_map_benchmark, resource_unordered_map_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      unordered_map_duration = test_end - test_start;
    }

    {
      std::chrono::time_point test_start = std::chrono::high_resolution_clock::now();
      BenchmarkingResourceController<BenchmarkData> resource_controller_benchmark;
      BenchmarkingResource<BenchmarkData> resource_benchmark;
      resource_benchmark.resource.setController(&resource_controller_benchmark.controller);
      resource_checksum = benchmarkDirectLookup(resource_controller_benchmark, resource_benchmark);
      std::chrono::time_point test_end = std::chrono::high_resolution_clock::now();
      resource_duration = test_end - test_start;
    }

    if(unordered_map_duration.count() < resource_duration.count())
      fmt::println(fg(fmt::color::green), "Unordered: {}", unordered_map_duration);
    else
      fmt::println(fg(fmt::color::red), "Unordered: {}", unordered_map_duration);
      
    if(unordered_map_duration.count() > resource_duration.count())
      fmt::println(fg(fmt::color::green), "Resource: {}", resource_duration);
    else
      fmt::println(fg(fmt::color::red), "Resource: {}", resource_duration);

    fmt::println(fg(fmt::color::purple), "Unordered - Resource = {}", unordered_map_duration - resource_duration);
    fmt::println(fg(fmt::color::purple), "Unordered / Resource = {}", unordered_map_duration / resource_duration);

    fmt::println(fg(fmt::color::yellow), "Unordered checksum = {}", unordered_map_checksum);
    fmt::println(fg(fmt::color::yellow), "Resource checksum = {}", resource_checksum);
    
    fmt::println("");
  }
};