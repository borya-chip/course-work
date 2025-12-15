#include "services/WriteOffService.h"
#include "services/WriteOffCalculator.h"

WriteOffService::Result WriteOffService::writeOffProduct(
    InventoryService &inventory, DatabaseManager *dbManager, int productId,
    int quantity, const QString &productName) {

  inventory.writeOffProduct(productId, quantity);

  auto productPtr = inventory.getProduct(productId);
  WriteOffService::Result result{0.0, false};

  if (productPtr) {

    result.writeOffValue =
        WriteOffCalculator::calculateWriteOffValue(*productPtr, quantity);
  }

  if (dbManager && result.writeOffValue >= 0.0) {
    try {
      result.dbRecordSaved = dbManager->writeOffs().addWriteOffRecord(
          productId, quantity, result.writeOffValue,
          productName.isEmpty() ? QString("Unknown Product") : productName);
    } catch (...) {
      result.dbRecordSaved = false;
    }
  }

  return result;
}
