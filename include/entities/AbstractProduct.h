#pragma once

#include <string>
#include <string_view>

class AbstractProduct {
private:
    std::string name;
    std::string category;
    int quantity;
    double unitPrice;

public:
    AbstractProduct(const std::string& name, const std::string& category, 
                   int quantity, double unitPrice);
    AbstractProduct(const AbstractProduct&) = default;
    AbstractProduct& operator=(const AbstractProduct&) = default;
    AbstractProduct(AbstractProduct&&) noexcept = default;
    AbstractProduct& operator=(AbstractProduct&&) noexcept = default;
    virtual ~AbstractProduct() = default;

    virtual double calculateTotalValue() const = 0;

    virtual std::string getProductType() const = 0;

    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }

    void setName(std::string_view name) { this->name.assign(name.data(), name.size()); }
    void setCategory(std::string_view category) { this->category.assign(category.data(), category.size()); }
    void setQuantity(int quantity);
    void setUnitPrice(double unitPrice);

    friend double calculateDiscountedPrice(const AbstractProduct& product, double discount);
};

double calculateDiscountedPrice(const AbstractProduct& product, double discount);

