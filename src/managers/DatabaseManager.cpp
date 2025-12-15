#include "managers/DatabaseManager.h"
#include "entities/Product.h"
#include "managers/OrderDataStore.h"
#include "managers/ProductDataStore.h"
#include "managers/WriteOffDataStore.h"
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QStandardPaths>
#include <algorithm>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <vector>


DatabaseManager *DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {

  QString dataPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir;
  if (!dir.exists(dataPath)) {
    dir.mkpath(dataPath);
  }
  dataFilePath = dataPath + "/products.dat";
  writeOffFilePath = dataPath + "/writeoff.dat";
  ordersFilePath = dataPath + "/orders.dat";

  productStore = new ProductDataStore(dataFilePath);
  orderStore = new OrderDataStore(ordersFilePath);
  writeOffStore = new WriteOffDataStore(writeOffFilePath);
}

DatabaseManager::~DatabaseManager() {
  delete productStore;
  delete orderStore;
  delete writeOffStore;
}

DatabaseManager *DatabaseManager::getInstance() {
  if (!instance) {
    instance = new DatabaseManager();
  }
  return instance;
}

void DatabaseManager::destroyInstance() {
  if (instance) {
    delete instance;
    instance = nullptr;
  }
}

bool DatabaseManager::initializeDatabase() { return connect(); }

bool DatabaseManager::connect() {

  QIODevice::OpenMode appendMode = QIODevice::WriteOnly | QIODevice::Append;

  {
    QFile file(dataFilePath);
    if (!file.open(appendMode)) {
      qDebug() << "Cannot create/open products file:" << dataFilePath;
      return false;
    }
  }

  {
    QFile writeOffFile(writeOffFilePath);
    if (!writeOffFile.open(appendMode)) {
      qDebug() << "Cannot create/open write-off file:" << writeOffFilePath;
      return false;
    }
  }

  {
    QFile ordersFile(ordersFilePath);
    if (!ordersFile.open(appendMode)) {
      qDebug() << "Cannot create/open orders file:" << ordersFilePath;
      return false;
    }
  }

  return true;
}

void DatabaseManager::disconnect() {}

bool DatabaseManager::isConnected() const {

  QFileInfo info(dataFilePath);
  return info.exists() || info.dir().exists();
}

ProductDataStore &DatabaseManager::products() { return *productStore; }

OrderDataStore &DatabaseManager::orders() { return *orderStore; }

WriteOffDataStore &DatabaseManager::writeOffs() { return *writeOffStore; }
