#pragma once

#include "managers/WriteOffRecord.h"
#include <QDataStream>
#include <QString>
#include <QStringList>
#include <vector>

class WriteOffDataStore {
private:
  QString filePath;

public:
  explicit WriteOffDataStore(const QString &filePath);

  bool addWriteOffRecord(int productId, int quantity, double value,
                         const QString &productName);
  std::vector<QStringList> getWriteOffHistory();

private:
  bool loadWriteOffRecords(std::vector<WriteOffRecord> &records);
  bool saveWriteOffRecords(const std::vector<WriteOffRecord> &records);
  static void writeWriteOffRecordToFile(QDataStream &stream,
                                        const WriteOffRecord &record);
  static bool readWriteOffRecordFromFile(QDataStream &stream,
                                         WriteOffRecord &record);
};

