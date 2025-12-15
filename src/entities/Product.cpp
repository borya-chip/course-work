#include "entities/Product.h"
#include "exceptions/Exceptions.h"
#include <QString>
#include <QTextStream>
#include <iomanip>
#include <sstream>


int Product::nextId = 1;

void Product::setNextId(int id) { nextId = id + 1; }

Product::Product(const std::string &name, const std::string &category,
                 int quantity, double unitPrice)
    : AbstractProduct(name, category, quantity, unitPrice), id(0) {}

double Product::calculateTotalValue() const {
  return getQuantity() * getUnitPrice();
}

std::string Product::getProductType() const { return "Standard Product"; }

Product &Product::operator+=(int qty) {
  setQuantity(getQuantity() + qty);
  return *this;
}

Product &Product::operator-=(int qty) {
  if (qty < 0) {
    throw NegativeQuantityException("Cannot subtract negative quantity");
  }
  setQuantity(getQuantity() - qty);
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Product &product) {
  os << "Product{"
     << "id=" << product.id << ", "
     << "name=" << product.getName() << ", "
     << "category=" << product.getCategory() << ", "
     << "quantity=" << product.getQuantity() << ", "
     << "unitPrice=" << std::fixed << std::setprecision(2) << product.getUnitPrice()
     << ", "
     << "totalValue=" << product.calculateTotalValue() << "}";
  return os;
}

std::istream &operator>>(std::istream &is, Product &product) {
  std::string line;
  std::getline(is, line);

  std::istringstream iss(line);
  std::string token;

  if (std::getline(iss, token, '|')) {
    product.setName(token);
  }
  if (std::getline(iss, token, '|')) {
    product.setCategory(token);
  }
  if (std::getline(iss, token, '|')) {
    product.setQuantity(std::stoi(token));
  }
  if (std::getline(iss, token, '|')) {
    product.setUnitPrice(std::stod(token));
  }

  return is;
}

QTextStream &operator<<(QTextStream &stream, const Product &product) {
  stream << "ID: " << product.id << "\n"
         << "Name: " << QString::fromStdString(product.getName()) << "\n"
         << "Category: " << QString::fromStdString(product.getCategory()) << "\n"
         << "Quantity: " << product.getQuantity() << "\n"
         << "Unit Price: $" << QString::number(product.getUnitPrice(), 'f', 2)
         << "\n"
         << "Total Value: $"
         << QString::number(product.calculateTotalValue(), 'f', 2) << "\n";
  return stream;
}
