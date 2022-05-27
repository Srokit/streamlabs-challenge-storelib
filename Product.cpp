
#include "Product.h"

#include <sstream>
#include <iostream>

#include "nlohmann/json.hpp"

Product::Product(nlohmann::json json_obj) {

    std::cout << "before" << std::endl;
    id = json_obj["id"];
    std::cout << "before" << std::endl;
    family = json_obj["family"];
    std::cout << "before" << std::endl;
    name = json_obj["name"];
    std::cout << "before" << std::endl;
    amountAvailable = json_obj["amountAvailable"];
    std::cout << "before" << std::endl;
    amountSold = json_obj["amountSold"];
    std::cout << "before" << std::endl;
    price = json_obj["price"];
    std::cout << "before" << std::endl;
}

bool Product::available() const {
    return amountAvailable > 0;
}
void Product::buy() {
    amountAvailable--;
    amountSold++;
}

std::string Product::get_id() const {
    return id;
}

std::string Product::get_name() const {
    return name;
}

std::string Product::get_family() const {
    return family;
}

int Product::get_price() const {
    return price;
}

std::mutex &Product::get_puchase_mutex() {
    return purchase_mutex;
}