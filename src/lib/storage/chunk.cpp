#include <iomanip>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "base_column.hpp"
#include "chunk.hpp"

#include "utils/assert.hpp"

namespace opossum {

void Chunk::add_column(std::shared_ptr<BaseColumn> column) {
  _columns.push_back(column);
}

void Chunk::append(const std::vector<AllTypeVariant>& values) {
  // Implementation goes here
}

std::shared_ptr<BaseColumn> Chunk::get_column(ColumnID column_id) const {
  if (column_id < col_count()) {
    return _columns[column_id];
  }
  return nullptr;
}

uint16_t Chunk::col_count() const {
  return _columns.size(); 
}

uint32_t Chunk::size() const {
  if (_columns[0]) {
    return _columns[0]->size();
  }
  return 0;
}

}  // namespace opossum
