#pragma once

#include <string>

struct WriteOffRecord {
  int id = 0;
  int productId = 0;
  std::string productName;
  int quantity = 0;
  double value = 0.0;

  WriteOffRecord() = default;
};

