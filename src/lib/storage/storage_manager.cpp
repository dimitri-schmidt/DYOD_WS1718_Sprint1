#include "storage_manager.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "utils/assert.hpp"

namespace opossum {

StorageManager& StorageManager::get() {
  static StorageManager storage_manager;
  return storage_manager;
}

void StorageManager::add_table(const std::string& name, std::shared_ptr<Table> table) {
  _tables.insert(std::make_pair(name, table));
}

void StorageManager::drop_table(const std::string& name) {
  auto count = _tables.erase(name);
  if (count <= 0) {
    throw std::invalid_argument("table with name " + name + " not found");
  }
}

std::shared_ptr<Table> StorageManager::get_table(const std::string& name) const {
  auto table = _tables.find(name);
  if (table != _tables.end()) {
    return table->second;
  }
  throw std::invalid_argument("table with name " + name + " not found");
}

bool StorageManager::has_table(const std::string& name) const {
  return (_tables.count(name) > 0);
}

std::vector<std::string> StorageManager::table_names() const {
  std::vector<std::string> names;
  for (auto table : _tables) {
    names.push_back(table.first);
  }
  return names;
}

void StorageManager::print(std::ostream& out) const {
  for (auto table : _tables) {
    out << table.first << std::endl;
  }
}

void StorageManager::reset() {
  get() = StorageManager();
}

}  // namespace opossum
