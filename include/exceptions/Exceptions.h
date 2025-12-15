#pragma once

#include <stdexcept>
#include <string>

class ProductException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class NegativeQuantityException : public ProductException {
public:
    explicit NegativeQuantityException(const std::string& message = "Quantity cannot be negative")
        : ProductException(message) {}
};

class InvalidPriceException : public ProductException {
public:
    explicit InvalidPriceException(const std::string& message = "Price must be positive")
        : ProductException(message) {}
};

class InvalidDateException : public ProductException {
public:
    explicit InvalidDateException(const std::string& message = "Invalid date format")
        : ProductException(message) {}
};

class ProductNotFoundException : public ProductException {
public:
    explicit ProductNotFoundException(const std::string& message = "Product not found")
        : ProductException(message) {}
};

