#pragma once

#include <string>

struct WriteOffRecord {
  int id;
  int productId;
  std::string productName;
  int quantity;
  double value;

  WriteOffRecord() : id(0), productId(0), quantity(0), value(0.0) {}
};

