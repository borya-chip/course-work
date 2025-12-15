#include "managers/ProductDataStore.h"
#include <QDataStream>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <algorithm>

ProductDataStore::ProductDataStore(const QString &filePath)
    : filePath(filePath) {}

bool ProductDataStore::loadProducts(std::vector<Product> &products) {
  products.clear();
  QFile file(filePath);
  if (!file.exists()) {
    return true;
  }

  if (!file.open(QIODevice::ReadOnly)) {
    return false;
  }

  if (file.size() == 0) {
    file.close();
    return true;
  }

  QDataStream in(&file);
  in.setVersion(QDataStream::Qt_6_0);

  Product product;
  int maxId = 0;
  while (!in.atEnd()) {
    if (readProductFromFile(in, product)) {
      products.push_back(product);
      if (product.getId() > maxId) {
        maxId = product.getId();
      }
    } else {
      break;
    }
  }

  file.close();
  Product::setNextId(maxId);
  return true;
}

bool ProductDataStore::saveProducts(const std::vector<Product> &products) {
  QFile file(filePath);
  QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Truncate;
  if (!file.open(mode)) {
    qDebug() << "Error opening products file for writing:" << filePath;
    return false;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_6_0);

  for (const auto &product : products) {
    writeProductToFile(out, product);
  }

  file.close();
  return true;
}

void ProductDataStore::writeProductToFile(QDataStream &stream,
                                         const Product &product) {
  stream << static_cast<qint32>(product.getId());

  QString name = QString::fromStdString(product.getName());
  stream << name;

  QString category = QString::fromStdString(product.getCategory());
  stream << category;

  stream << static_cast<qint32>(product.getQuantity());

  stream << product.getUnitPrice();

  QString productType = QString::fromStdString(product.getProductType());
  stream << productType;
}

bool ProductDataStore::readProductFromFile(QDataStream &stream,
                                          Product &product) {
  if (stream.atEnd()) {
    return false;
  }

  qint32 id;
  stream >> id;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  QString name;
  stream >> name;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  QString category;
  stream >> category;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  qint32 quantity;
  stream >> quantity;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  double unitPrice;
  stream >> unitPrice;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  QString productType;
  stream >> productType;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }

  if (!stream.atEnd()) {
    QDate dummyDate;
    stream >> dummyDate;
  }

  product =
      Product(name.toStdString(), category.toStdString(), quantity, unitPrice);
  product.setId(id);

  return true;
}

bool ProductDataStore::addProduct(const Product &product) {
  std::vector<Product> products;
  if (!loadProducts(products)) {
    products.clear();
  }

  auto it = std::find_if(
      products.begin(), products.end(),
      [&product](const Product &p) { return p.getId() == product.getId(); });

  if (it != products.end()) {
    *it = product;
  } else {
    products.push_back(product);
  }

  return saveProducts(products);
}

bool ProductDataStore::updateProduct(const Product &product) {
  std::vector<Product> products;
  if (!loadProducts(products)) {
    qDebug() << "Failed to load products for update";
    return false;
  }

  auto it = std::find_if(
      products.begin(), products.end(),
      [&product](const Product &p) { return p.getId() == product.getId(); });

  if (it != products.end()) {
    *it = product;
    if (!saveProducts(products)) {
      qDebug() << "Failed to save products after update";
      return false;
    }
    return true;
  }

  qDebug() << "Product with ID" << product.getId()
           << "not found in database for update";
  return false;
}

bool ProductDataStore::deleteProduct(int id) {
  std::vector<Product> products;
  if (!loadProducts(products)) {
    return false;
  }

  auto it = std::remove_if(products.begin(), products.end(),
                           [id](const Product &p) { return p.getId() == id; });

  if (it != products.end()) {
    products.erase(it, products.end());
    return saveProducts(products);
  }

  return false;
}

Product ProductDataStore::getProduct(int id) {
  std::vector<Product> products;
  if (!loadProducts(products)) {
    return Product();
  }

  auto it = std::find_if(products.begin(), products.end(),
                         [id](const Product &p) { return p.getId() == id; });

  if (it != products.end()) {
    return *it;
  }

  return Product();
}

std::vector<Product> ProductDataStore::getAllProducts() {
  std::vector<Product> products;
  loadProducts(products);
  return products;
}

std::vector<Product> ProductDataStore::searchProductsByName(const QString &name) {
  std::vector<Product> allProducts;
  if (!loadProducts(allProducts)) {
    return std::vector<Product>();
  }

  std::vector<Product> results;
  QString searchName = name.toLower();

  for (const auto &product : allProducts) {
    QString productName = QString::fromStdString(product.getName()).toLower();
    if (productName.contains(searchName)) {
      results.push_back(product);
    }
  }

  return results;
}

std::vector<Product>
ProductDataStore::searchProductsByCategory(const QString &category) {
  std::vector<Product> allProducts;
  if (!loadProducts(allProducts)) {
    return std::vector<Product>();
  }

  std::vector<Product> results;
  QString searchCategory = category.toLower();

  for (const auto &product : allProducts) {
    QString productCategory =
        QString::fromStdString(product.getCategory()).toLower();
    if (productCategory == searchCategory) {
      results.push_back(product);
    }
  }

  return results;
}
