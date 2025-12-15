#pragma once

#include "entities/Order.h"
#include <QDataStream>
#include <QDate>
#include <QString>
#include <vector>

class OrderDataStore {
private:
  QString filePath;

public:
  explicit OrderDataStore(const QString &filePath);

  bool addOrder(const Order &order);
  bool updateOrder(const Order &order);
  bool deleteOrder(int id);
  Order getOrder(int id);
  std::vector<Order> getAllOrders();
  std::vector<Order> getOrdersByCompany(const QString &companyName);
  std::vector<Order> getOrdersByType(OrderType type);
  std::vector<Order> getOrdersByDateRange(const QDate &startDate,
                                          const QDate &endDate);

private:
  bool loadOrders(std::vector<Order> &orders);
  bool saveOrders(const std::vector<Order> &orders);
  static void writeOrderToFile(QDataStream &stream, const Order &order);
  static bool readOrderFromFile(QDataStream &stream, Order &order);
};

