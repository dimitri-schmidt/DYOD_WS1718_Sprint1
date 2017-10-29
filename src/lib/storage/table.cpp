#include "table.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "value_column.hpp"

#include "resolve_type.hpp"
#include "types.hpp"
#include "utils/assert.hpp"

namespace opossum {

Table::Table(const uint32_t chunk_size)
: _chunk_size(chunk_size) {
  create_new_chunk();
}

void Table::add_column_definition(const std::string& name, const std::string& type) {
  _column_names.push_back(name);
  _column_types.push_back(type);
}

void Table::add_column(const std::string& name, const std::string& type) {
  add_column_definition(name, type);
  for (auto chunk : _chunks) {
    auto column = make_shared_by_column_type<BaseColumn, ValueColumn>(type);
    chunk->add_column(column);
  }
}

void Table::append(std::vector<AllTypeVariant> values) {
  if (_chunk_size != 0 && _chunks.back()->size() == _chunk_size) {
    create_new_chunk();
  }
  _chunks.back()->append(values);
}

void Table::create_new_chunk() {
  auto new_chunk = std::make_shared<Chunk>(Chunk());

  for (auto type : _column_types) {
    new_chunk->add_column(make_shared_by_column_type<BaseColumn, ValueColumn>(type));
  }

  _chunks.push_back(new_chunk);
}

uint16_t Table::col_count() const {
  return _chunks[0]->col_count();
}

uint64_t Table::row_count() const {
  // all chunks except the last one got exactly _chunk_size many elements
  return (chunk_count() - 1) * _chunk_size + _chunks.back()->size();
}

ChunkID Table::chunk_count() const {
  return ChunkID{_chunks.size()};
}

ColumnID Table::column_id_by_name(const std::string& column_name) const {
  uint16_t index = 0;
  for (auto& name : _column_names) {
    if (name == column_name) {
      return ColumnID{index};
    }
    ++index;
  }
  throw std::invalid_argument("column with name " + column_name + " not found");
}

uint32_t Table::chunk_size() const {
  return _chunk_size;
}

const std::vector<std::string>& Table::column_names() const {
  return _column_names;
}

const std::string& Table::column_name(ColumnID column_id) const {
  DebugAssert(column_id < _column_names.size(),
    "Table::column_name: column_id out of range: " + std::to_string(column_id));
  return _column_names[column_id];
}

const std::string& Table::column_type(ColumnID column_id) const {
DebugAssert(column_id < _column_types.size(),
    "Table::column_type: column_id out of range: " + std::to_string(column_id));
  return _column_types[column_id];
}

Chunk& Table::get_chunk(ChunkID chunk_id) {
DebugAssert(chunk_id < _chunks.size(),
    "Table::get_chunk: chunk_id out of range: " + std::to_string(chunk_id));
  return (*_chunks[chunk_id]);
}

const Chunk& Table::get_chunk(ChunkID chunk_id) const {
DebugAssert(chunk_id < _chunks.size(),
    "Table::get_chunk: chunk_id out of range: " + std::to_string(chunk_id));
  return (*_chunks[chunk_id]);
}

}  // namespace opossum
