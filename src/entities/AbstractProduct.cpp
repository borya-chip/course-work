#include "entities/AbstractProduct.h"
#include "exceptions/Exceptions.h"

AbstractProduct::AbstractProduct(const std::string& name, const std::string& category,
                               int quantity, double unitPrice)
    : name(name), category(category), quantity(quantity), 
      unitPrice(unitPrice) {
    if (quantity < 0) {
        throw NegativeQuantityException("Quantity cannot be negative");
    }
    if (unitPrice < 0) {
        throw InvalidPriceException("Unit price cannot be negative");
    }
}

void AbstractProduct::setQuantity(int newQuantity) {
    if (newQuantity < 0) {
        throw NegativeQuantityException("Quantity cannot be negative");
    }
    quantity = newQuantity;
}

void AbstractProduct::setUnitPrice(double newUnitPrice) {
    if (newUnitPrice < 0) {
        throw InvalidPriceException("Unit price cannot be negative");
    }
    unitPrice = newUnitPrice;
}

double calculateDiscountedPrice(const AbstractProduct& product, double discount) {
    if (discount < 0.0 || discount > 1.0) {
        throw InvalidPriceException("Discount must be between 0 and 1");
    }
    return product.getUnitPrice() * (1.0 - discount);
}
