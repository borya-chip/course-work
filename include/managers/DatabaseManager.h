#pragma once

#include "entities/Order.h"
#include "entities/Product.h"
#include <QDataStream>
#include <QDate>
#include <QFile>
#include <QString>
#include <QStringList>
#include <string>
#include <vector>

#include "managers/WriteOffRecord.h"
#include "managers/OrderDataStore.h"
#include "managers/ProductDataStore.h"
#include "managers/WriteOffDataStore.h"

class DatabaseManager {
private:
  static DatabaseManager *instance;
  QString dataFilePath;
  QString writeOffFilePath;
  QString ordersFilePath;

  bool connected = false;

  ProductDataStore *productStore;
  OrderDataStore *orderStore;
  WriteOffDataStore *writeOffStore;

  DatabaseManager();
  ~DatabaseManager();

public:
  static DatabaseManager *getInstance();
  static void destroyInstance();

  bool initializeDatabase();
  bool connect();
  void disconnect();
  bool isConnected() const;

  ProductDataStore &products();
  OrderDataStore &orders();
  WriteOffDataStore &writeOffs();
};

