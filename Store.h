
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Product.h"

class Store {
    // TODO: Fill out
    public:
        Store();

        // Methods virtual so that class is not statically linked
        virtual std::vector<std::string> get_all_product_ids();
        virtual bool purchase_by_id(std::string, const std::string &);
        virtual std::vector<std::string> get_all_product_ids_for_family(std::string);
    private:
        std::unordered_map<std::string, Product> products;
        std::unordered_map<std::string, std::vector<Product *>> products_by_family;
};