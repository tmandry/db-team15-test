#include "data_loader.h"

#include <tuple>
#include <vector>
#include <array>

typedef array<tuple<string, Table::TYPE>> TableDescriptor;

static const TableDescriptor MY_TABLE = {
  make_tuple("name", Table::STRING),
  make_tuple("age", Table::INT)
};
