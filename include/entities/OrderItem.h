#pragma once

#include "entities/Product.h"
#include <QString>

struct OrderItem {
    int productId = 0;
    QString productName;
    QString category;
    int quantity = 0;
    double unitPrice = 0.0;
    double discountPercent = 0.0;
    double totalPrice = 0.0;
    
    OrderItem() = default;
    
    OrderItem(int id, const QString& name, const QString& cat, int qty, double price)
        : productId(id), productName(name), category(cat), quantity(qty), 
          unitPrice(price) {}
    
    void calculateTotal(double discount) {
        discountPercent = discount;
        double priceAfterDiscount = unitPrice * (1.0 - discount / 100.0);
        totalPrice = priceAfterDiscount * quantity;
    }
};








