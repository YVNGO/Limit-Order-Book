#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "Order.h"
#include "OrderBook.h"

class MatchingEngine {
private:
    OrderBook* book;
    void split_input(const std::string& s, std::vector<std::string>& tokens, const std::string& delim = ",");
public:
    MatchingEngine();
    void run( std::string& file_path) {
        std::ifstream file(file_path);
        std::string str;
        while(getline(file, str)){
            str.erase(str.find_last_not_of(" \n\r\t") + 1);
            std::vector<std::string> tokens;
            split_input(str, tokens, ",");
            
            int order_id{stoi(tokens[1])};
            bool is_buy_order{tokens[2] == "B" ? true : false};
            int order_quantity{stoi(tokens[3])};
            int order_limit{stoi(tokens[4])};
            
            if(tokens[0]=="A") {
                // Add Order
                std::shared_ptr<Order> newOrderPtr(new Order(order_id, is_buy_order, order_quantity, order_limit));
                book->addOrder(newOrderPtr);
            }
            else if(tokens[0]=="X"){
                // Delete Order
                book->remove(order_id, order_quantity);
            }
        }
    }

    void print_order_book() {
        book->print();
    }
};

MatchingEngine::MatchingEngine(){
    book = new OrderBook();
}

void MatchingEngine::split_input(const std::string& s, std::vector<std::string>& tokens, const std::string& delim) {
        std::string::size_type last_pos = s.find_first_not_of(delim, 0);
        std::string::size_type pos = s.find_first_of(delim, last_pos);
        while (std::string::npos != pos || std::string::npos != last_pos)
        {
            tokens.push_back(s.substr(last_pos, pos - last_pos));
            last_pos = s.find_first_not_of(delim, pos);
            pos = s.find_first_of(delim, last_pos);
        }
    }