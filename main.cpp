#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

struct Entry {
  uint32_t id;
  std::string name;
  std::string category;
  std::string type;
  uint32_t price;
  uint32_t calorie;
  double salt;
};

Entry load_entry(const std::string& line) {
  std::stringstream ss;
  ss << line;
  std::vector<std::string> cols;
  for (std::string str; std::getline(ss, str, ','); ) {
    cols.push_back(str);
  }
  Entry e = {
    std::stoul(cols[0]),
    cols[1],
    cols[2],
    cols[3],
    std::stoul(cols[4]),
    std::stoul(cols[5]),
    std::stod(cols[6])
  };
  return e;
}

std::vector<Entry> load_menu() {
  std::ifstream ifs("menu.csv");
  std::string ignore;
  std::getline(ifs, ignore);
  std::vector<Entry> res;
  for (std::string line; std::getline(ifs, line); ) {
    res.push_back(load_entry(line));
  }
  return res;
}

std::tuple<uint32_t, std::vector<Entry>> solve_impl(const std::vector<Entry>& menu, const uint32_t max_yen, const uint32_t index) {
  using std::size;
  if (index >= size(menu)) {
    return std::tuple<uint32_t, std::vector<Entry>>();
  }
  const auto not_used = solve_impl(menu, max_yen, index+1);
  const auto price = menu[index].price;
  if (price <= max_yen) {
    auto used = solve_impl(menu, max_yen - price, index+1);
    const auto used_calorie = std::get<0>(used) + menu[index].calorie;
    if (used_calorie > std::get<0>(not_used)) {
      std::get<0>(used) = used_calorie;
      std::get<1>(used).push_back(menu[index]);
      return used;
    } else {
      return not_used;
    }
  } else {
    return not_used;
  }
}

std::vector<Entry> solve(const std::vector<Entry>& menu, const uint32_t max_yen) {
  return std::get<1>(solve_impl(menu, max_yen, 0));
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " MAX_YEN" << std::endl;
    return EXIT_FAILURE;
  }

  std::size_t pos = 0;
  const uint32_t max_yen = std::stoul(argv[1], &pos);
  if (pos != std::strlen(argv[1])) {
    std::cerr << "Failed to parse argument" << std::endl;
    return EXIT_FAILURE;
  }

  std::cerr << "Loading menu..." << std::endl;
  const auto menu = load_menu();

  std::cerr << "Solving..." << std::endl;
  const auto result = solve(menu, max_yen);

  std::cerr << "Solved!" << std::endl;
  uint32_t price = 0;
  uint32_t calorie = 0;
  for (const auto& entry : result) {
    price += entry.price;
    calorie += entry.calorie;
    std::cout << entry.name << ": " << entry.price << " yen, " << entry.calorie << " kcal" << std::endl;
  }
  std::cout << "Price: " << price << " yen" << std::endl;
  std::cout << "Calorie: " << calorie << " kcal" << std::endl;

  return 0;
}
