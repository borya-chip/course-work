#pragma once

#include "entities/Product.h"
#include <QDataStream>
#include <QString>
#include <vector>

class ProductDataStore {
private:
  QString filePath;

public:
  explicit ProductDataStore(const QString &filePath);

  bool addProduct(const Product &product);
  bool updateProduct(const Product &product);
  bool deleteProduct(int id);
  Product getProduct(int id);
  std::vector<Product> getAllProducts();

  std::vector<Product> searchProductsByName(const QString &name);
  std::vector<Product> searchProductsByCategory(const QString &category);

private:
  bool loadProducts(std::vector<Product> &products);
  bool saveProducts(const std::vector<Product> &products);
  static void writeProductToFile(QDataStream &stream, const Product &product);
  static bool readProductFromFile(QDataStream &stream, Product &product);
};

