#include "managers/OrderDataStore.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <algorithm>

OrderDataStore::OrderDataStore(const QString &filePath) : filePath(filePath) {}

bool OrderDataStore::loadOrders(std::vector<Order> &orders) {
  orders.clear();
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

  Order order;
  int maxId = 0;
  while (!in.atEnd()) {
    if (readOrderFromFile(in, order)) {
      orders.push_back(order);
      if (order.getId() > maxId) {
        maxId = order.getId();
      }
    } else {
      break;
    }
  }

  file.close();
  Order::setNextId(maxId);
  return true;
}

bool OrderDataStore::saveOrders(const std::vector<Order> &orders) {
  QFile file(filePath);
  QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Truncate;
  if (!file.open(mode)) {
    qDebug() << "Error opening orders file for writing:" << filePath;
    return false;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_6_0);

  for (const auto &order : orders) {
    writeOrderToFile(out, order);
  }

  file.close();
  return true;
}

void OrderDataStore::writeOrderToFile(QDataStream &stream, const Order &order) {
  stream << static_cast<qint32>(order.getId());

  stream << order.getCompanyName();
  stream << order.getContactPerson();
  stream << order.getPhone();

  stream << static_cast<qint32>(static_cast<int>(order.getOrderType()));

  stream << order.getOrderDate();

  const auto &items = order.getItems();
  stream << static_cast<qint32>(items.size());

  for (const auto &item : items) {
    stream << static_cast<qint32>(item.productId);
    stream << item.productName;
    stream << item.category;
    stream << static_cast<qint32>(item.quantity);
    stream << item.unitPrice;
    stream << item.discountPercent;
    stream << item.totalPrice;
  }

  stream << order.getTotalAmount();
  stream << order.getTotalDiscount();
}

bool OrderDataStore::readOrderFromFile(QDataStream &stream, Order &order) {
  if (stream.atEnd()) {
    return false;
  }

  qint32 id;
  stream >> id;
  if (stream.status() != QDataStream::Ok)
    return false;

  QString companyName, contactPerson, phone;
  stream >> companyName;
  if (stream.status() != QDataStream::Ok)
    return false;
  stream >> contactPerson;
  if (stream.status() != QDataStream::Ok)
    return false;
  stream >> phone;
  if (stream.status() != QDataStream::Ok)
    return false;

  qint32 orderTypeInt;
  stream >> orderTypeInt;
  if (stream.status() != QDataStream::Ok)
    return false;
  OrderType orderType = static_cast<OrderType>(orderTypeInt);

  QDate orderDate;
  stream >> orderDate;
  if (stream.status() != QDataStream::Ok)
    return false;

  order = Order(companyName, contactPerson, phone, orderType);
  order.setId(id);
  order.setOrderType(orderType);
  order.setOrderDate(orderDate);

  qint32 itemsCount;
  stream >> itemsCount;
  if (stream.status() != QDataStream::Ok)
    return false;

  for (int i = 0; i < itemsCount; ++i) {
    qint32 productId;
    QString productName, category;
    qint32 quantity;
    double unitPrice, discountPercent, totalPrice;

    stream >> productId;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> productName;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> category;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> quantity;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> unitPrice;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> discountPercent;
    if (stream.status() != QDataStream::Ok)
      return false;
    stream >> totalPrice;
    if (stream.status() != QDataStream::Ok)
      return false;

    OrderItem item(productId, productName, category, quantity, unitPrice);
    item.discountPercent = discountPercent;
    item.totalPrice = totalPrice;
    order.addItem(item);
  }

  double totalAmount, totalDiscount;
  stream >> totalAmount;
  if (stream.status() != QDataStream::Ok)
    return false;
  stream >> totalDiscount;
  if (stream.status() != QDataStream::Ok)
    return false;

  return true;
}

bool OrderDataStore::addOrder(const Order &order) {
  std::vector<Order> orders;
  if (!loadOrders(orders)) {
    orders.clear();
  }

  orders.push_back(order);
  return saveOrders(orders);
}

std::vector<Order> OrderDataStore::getAllOrders() {
  std::vector<Order> orders;
  loadOrders(orders);
  return orders;
}

std::vector<Order> OrderDataStore::getOrdersByCompany(const QString &companyName) {
  std::vector<Order> allOrders;
  if (!loadOrders(allOrders)) {
    return std::vector<Order>();
  }

  std::vector<Order> results;
  QString searchName = companyName.toLower();

  for (const auto &order : allOrders) {
    if (order.getCompanyName().toLower().contains(searchName)) {
      results.push_back(order);
    }
  }

  return results;
}

std::vector<Order> OrderDataStore::getOrdersByType(OrderType type) {
  std::vector<Order> allOrders;
  if (!loadOrders(allOrders)) {
    return std::vector<Order>();
  }

  std::vector<Order> results;
  for (const auto &order : allOrders) {
    if (order.getOrderType() == type) {
      results.push_back(order);
    }
  }

  return results;
}

std::vector<Order>
OrderDataStore::getOrdersByDateRange(const QDate &startDate, const QDate &endDate) {
  std::vector<Order> allOrders;
  if (!loadOrders(allOrders)) {
    return std::vector<Order>();
  }

  std::vector<Order> results;
  for (const auto &order : allOrders) {
    QDate orderDate = order.getOrderDate();
    if (orderDate >= startDate && orderDate <= endDate) {
      results.push_back(order);
    }
  }

  return results;
}

bool OrderDataStore::updateOrder(const Order &order) {
  std::vector<Order> orders;
  if (!loadOrders(orders)) {
    return false;
  }

  auto it =
      std::find_if(orders.begin(), orders.end(), [&order](const Order &o) {
        return o.getId() == order.getId();
      });

  if (it != orders.end()) {
    *it = order;
    return saveOrders(orders);
  }

  return false;
}

bool OrderDataStore::deleteOrder(int id) {
  std::vector<Order> orders;
  if (!loadOrders(orders)) {
    return false;
  }

  auto it = std::remove_if(orders.begin(), orders.end(),
                           [id](const Order &o) { return o.getId() == id; });

  if (it != orders.end()) {
    orders.erase(it, orders.end());
    return saveOrders(orders);
  }

  return false;
}

Order OrderDataStore::getOrder(int id) {
  std::vector<Order> orders;
  if (!loadOrders(orders)) {
    return Order();
  }

  auto it = std::find_if(orders.begin(), orders.end(),
                         [id](const Order &o) { return o.getId() == id; });

  if (it != orders.end()) {
    return *it;
  }

  return Order();
}
