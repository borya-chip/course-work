#include "managers/WriteOffDataStore.h"
#include <QDataStream>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <algorithm>

WriteOffDataStore::WriteOffDataStore(const QString &filePath)
    : filePath(filePath) {}

bool WriteOffDataStore::loadWriteOffRecords(std::vector<WriteOffRecord> &records) {
  records.clear();
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

  WriteOffRecord record;
  while (!in.atEnd()) {
    if (readWriteOffRecordFromFile(in, record)) {
      records.push_back(record);
    } else {
      break;
    }
  }

  file.close();
  return true;
}

bool WriteOffDataStore::saveWriteOffRecords(
    const std::vector<WriteOffRecord> &records) {
  QFile file(filePath);
  QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Truncate;
  if (!file.open(mode)) {
    qDebug() << "Error opening write-off file for writing:" << filePath;
    return false;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_6_0);

  for (const auto &record : records) {
    writeWriteOffRecordToFile(out, record);
  }

  file.close();
  return true;
}

void WriteOffDataStore::writeWriteOffRecordToFile(QDataStream &stream,
                                                  const WriteOffRecord &record) {
  stream << static_cast<qint32>(record.id);
  stream << static_cast<qint32>(record.productId);

  QString name = QString::fromStdString(record.productName);
  stream << name;

  stream << static_cast<qint32>(record.quantity);
  stream << record.value;
}

bool WriteOffDataStore::readWriteOffRecordFromFile(QDataStream &stream,
                                                   WriteOffRecord &record) {
  if (stream.atEnd()) {
    return false;
  }

  qint32 id;
  stream >> id;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }
  record.id = id;

  qint32 productId;
  stream >> productId;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }
  record.productId = productId;

  QString name;
  stream >> name;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }
  record.productName = name.toStdString();

  qint32 quantity;
  stream >> quantity;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }
  record.quantity = quantity;

  double value;
  stream >> value;
  if (stream.status() != QDataStream::Ok) {
    return false;
  }
  record.value = value;

  if (!stream.atEnd()) {
    QDate dummyDate;
    stream >> dummyDate;
  }

  return true;
}

bool WriteOffDataStore::addWriteOffRecord(int productId, int quantity,
                                         double value,
                                         const QString &productName) {
  try {
    std::vector<WriteOffRecord> records;
    if (!loadWriteOffRecords(records)) {
      records.clear();
    }

    WriteOffRecord record;

    if (records.empty()) {
      record.id = 1;
    } else {
      int maxId = 0;
      for (const auto &r : records) {
        if (r.id > maxId) {
          maxId = r.id;
        }
      }
      record.id = maxId + 1;
    }

    record.productId = productId;
    record.productName = productName.toStdString();
    record.quantity = quantity;
    record.value = value;

    records.push_back(record);
    return saveWriteOffRecords(records);
  } catch (const std::exception &e) {
    qDebug() << "Exception in addWriteOffRecord:" << e.what();
    return false;
  } catch (...) {
    qDebug() << "Unknown exception in addWriteOffRecord";
    return false;
  }
}

std::vector<QStringList> WriteOffDataStore::getWriteOffHistory() {
  std::vector<WriteOffRecord> records;
  if (!loadWriteOffRecords(records)) {
    return std::vector<QStringList>();
  }

  std::sort(records.begin(), records.end(),
            [](const WriteOffRecord &a, const WriteOffRecord &b) {
              return a.id > b.id;
            });

  std::vector<QStringList> history;
  for (const auto &record : records) {
    QStringList list;
    list << QString::number(record.id);
    list << QString::fromStdString(record.productName);
    list << QString::number(record.quantity);
    list << QString::number(record.value, 'f', 2);
    history.push_back(list);
  }

  return history;
}
