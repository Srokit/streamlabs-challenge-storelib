
#pragma once

#include <string>

#include "nlohmann/json.hpp"

class Product {
public:
    Product(nlohmann::json);
    Product() {};

    std::string to_string() const;
    std::string get_id() const;
    std::string get_name() const;
    std::string get_family() const;
    int get_price() const;
    std::mutex &get_puchase_mutex();

    bool available() const;
    void buy();
private:
    std::string id;
    std::string family;
    std::string name;
    int amountAvailable;
    int amountSold;
    int price;
    std::mutex purchase_mutex;
};