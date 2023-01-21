#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

const int count = 100000;

TEST_CASE("find items in snapshot", "[snapshot]") {
  std::vector<int> ids(count);
  std::unordered_map<int, std::shared_ptr<const std::string>> data;

  std::generate_n(ids.begin(), count, []() { return std::rand(); });

  BENCHMARK("fill snapshot") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      data.emplace(i,
                   std::make_shared<const std::string>(std::to_string(ids[i])));
      sum += ids[i];
    }
    return sum;
  };

  BENCHMARK("query present in snapshot") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = ids[rand() % ids.size()];
      auto pos = data.find(value);
      if (pos != data.end()) {
        sum += std::stoi(*pos->second);
      }
    }
    return sum;
  };

  BENCHMARK("query unknown in snapshot") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = rand();
      auto pos = data.find(value);
      if (pos != data.end()) {
        sum += std::stoi(*pos->second);
      }
    }
    return sum;
  };
}

TEST_CASE("find items in 10 versions", "[snapshot]") {
  const int version_count = 10;
  const int new_version = count / version_count;
  using map_type = std::unordered_map<int, std::shared_ptr<const std::string>>;
  std::vector<int> ids(count);
  std::vector<map_type> versions;

  std::generate_n(ids.begin(), count, []() { return std::rand(); });

  BENCHMARK("fill versions") {
    auto sum = 0;
    versions.clear();
    for (auto i = 0, new_version_count = 0; i < count;
         ++i, ++new_version_count) {
      if (new_version_count >= new_version) {
        new_version_count = 0;
      }
      if (new_version_count == 0) {
        versions.emplace_back(map_type());
      }
      versions.back().emplace(
          i, std::make_shared<const std::string>(std::to_string(ids[i])));
      sum += ids[i];
    }
    return sum;
  };

  auto find_value = [&](int value) {
    for (int v = size(versions) - 1; v >= 0; --v) {
      const auto &data = versions[v];
      auto pos = data.find(value);
      if (pos != data.end()) {
        return std::stoi(*pos->second);
      }
    }
    return 0;
  };

  BENCHMARK("query present in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = ids[rand() % ids.size()];
      sum += find_value(value);
    }
    return sum;
  };

  BENCHMARK("query unknown in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      sum += find_value(rand());
    }
    return sum;
  };
}

TEST_CASE("find items in 100 versions", "[snapshot]") {
  const int version_count = 100;
  const int new_version = count / version_count;
  using map_type = std::unordered_map<int, std::shared_ptr<const std::string>>;
  std::vector<int> ids(count);
  std::vector<map_type> versions;

  std::generate_n(ids.begin(), count, []() { return std::rand(); });

  BENCHMARK("fill versions") {
    auto sum = 0;
    versions.clear();
    for (auto i = 0, new_version_count = 0; i < count;
         ++i, ++new_version_count) {
      if (new_version_count >= new_version) {
        new_version_count = 0;
      }
      if (new_version_count == 0) {
        versions.emplace_back(map_type());
      }
      versions.back().emplace(
          i, std::make_shared<const std::string>(std::to_string(ids[i])));
      sum += ids[i];
    }
    return sum;
  };

  auto find_value = [&](int value) {
    for (int v = size(versions) - 1; v >= 0; --v) {
      const auto &data = versions[v];
      auto pos = data.find(value);
      if (pos != data.end()) {
        return std::stoi(*pos->second);
      }
    }
    return 0;
  };

  BENCHMARK("query present in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = ids[rand() % ids.size()];
      sum += find_value(value);
    }
    return sum;
  };

  BENCHMARK("query unknown in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      sum += find_value(rand());
    }
    return sum;
  };
}

TEST_CASE("find items in versioned map", "[snapshot]") {
  using list_type = std::vector<std::shared_ptr<const std::string>>;
  using map_type = std::unordered_map<int, list_type>;
  std::vector<int> ids(count);
  map_type versions;

  std::generate_n(ids.begin(), count, []() { return std::rand(); });

  BENCHMARK("fill versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = ids[i];
      auto pos = versions.find(value);
      if (pos == versions.end()) {
        bool inserted;
        std::tie(pos, inserted) = versions.emplace(value, list_type());
      }
      pos->second.emplace_back(
          std::make_shared<const std::string>(std::to_string(value)));
      sum += ids[i];
    }
    return sum;
  };

  BENCHMARK("query present in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = ids[rand() % ids.size()];
      auto pos = versions.find(value);
      if (pos!=versions.end()) {
        sum += std::stoi(*pos->second.back());
      }
    }
    return sum;
  };

  BENCHMARK("query unknown in versions") {
    auto sum = 0;
    for (auto i = 0; i < count; ++i) {
      auto value = rand();
      auto pos = versions.find(value);
      if (pos!=versions.end()) {
        sum += std::stoi(*pos->second.back());
      }
    }
    return sum;
  };
}