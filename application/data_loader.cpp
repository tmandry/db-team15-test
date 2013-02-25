#include "data_loader.h"

#include <tuple>
#include <vector>
#include <array>

#include "Table.h"
#include "Database.h"

template<int Num>
struct TableDescriptor {
  typedef array<tuple<string, Table::TYPE>, Num> type;
};

static const TableDescriptor<2>::type MY_TABLE = {
  make_tuple("name", Table::STRING),
  make_tuple("age", Table::INT)
};
