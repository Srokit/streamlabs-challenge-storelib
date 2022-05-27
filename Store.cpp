
#include "Store.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <sstream>

#include "nlohmann/json.hpp"

std::string get_random_32_char_num_string() {
    constexpr size_t length = 32;
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        ss << std::to_string(rand() % 10);
    }
    return ss.str();
}

void print_receipt(const Product &product, const std::string &customer_name) {
    std::string receipt_path = "./receipts/";

    std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string now_readable_str(std::ctime(&now_time));

    std::string receipt_filename = receipt_path + customer_name + product.get_id() + get_random_32_char_num_string();

    std::filesystem::create_directory(receipt_path);
    
    std::ofstream receipt_file_obj;
    receipt_file_obj.open(receipt_filename);

    if (!receipt_file_obj) {
        std::cout << "Can't print ticket" << std::endl;
        std::cout << strerror(errno) << std::endl;
    }

    receipt_file_obj << "Customer Name: " << customer_name << "\n";
    receipt_file_obj << "Product ID: " << product.get_id() << "\n";
    receipt_file_obj << "Product Name: " << product.get_name() << "\n";
    receipt_file_obj << "Purchase Amount: $" << product.get_price() << ".00\n";
    receipt_file_obj << "Time of Purchase: " << now_readable_str << "\n";

    receipt_file_obj.close();
}

Store::Store() {
    // Load in data from json
    const char* json_file = "./assets/store.json";

    std::fstream json_file_obj;
    json_file_obj.open(json_file);
    // TODO: Check for file can't open

    std::stringstream ss;
    std::string w;
    while (!json_file_obj.eof()) {
        json_file_obj >> w;
        ss << w;
    }
    std::string json_str(ss.str());

    auto json_dict = nlohmann::json::parse(json_str);

    auto prods_json_array = json_dict["products"];
    for (const auto & prod_json_obj : prods_json_array) {
        const std::string id_str = prod_json_obj["id"].get<const std::string>();
        std::cout << "before" << id_str << std::endl;
        // const std::pair<const std::string, Product> insert_pair{id_str, prod_json_obj};
        std::cout << "after" << std::endl;
        products.emplace(id_str, prod_json_obj);
    }
    for (auto & [prod_id, prod] : products) {
        if (products_by_family.find(prod.get_family()) == products_by_family.end()) {
            products_by_family.insert({prod.get_family(), std::vector<Product *>()});
        }
        products_by_family[prod.get_family()].push_back(&prod);
    }
}

std::vector<std::string> Store::get_all_product_ids() {
    std::vector<std::string> out;
    for (const auto & [prod_id, prod] : products) {
        out.push_back(prod_id);
    }
    return out;
}

bool Store::purchase_by_id(std::string id, const std::string& customer_name) {
    Product &product = products.find(id)->second;

    std::cout << "buying " << id << std::endl;

    product.get_puchase_mutex().lock();
    bool ret_val = false;
    if (product.available()) {
        product.buy();
        std::cout << "Customer " << customer_name << " purchased product with id " << id << "\n";
        print_receipt(product, customer_name);
        ret_val = true;
    } else {
        std::cout << "Customer " << customer_name << " could not purchase product with id " << id;
        std::cout << "because it is sold-out\n";
    }

    product.get_puchase_mutex().unlock();

    if (ret_val) {
        print_receipt(product, customer_name);
    }

    return ret_val;
}

std::vector<std::string> Store::get_all_product_ids_for_family(std::string family) {
    std::vector<std::string> out;
    for (Product *prod : products_by_family[family]) {
        out.push_back(prod->get_id());
    }
    return out;
}

// Dynamic lib entry points for main executable
extern "C" Store* create_store() {
    return new Store;
}

extern "C" void destroy_store(Store* store) {
    delete store;
}