#ifndef ORDER_H
#define ORDER_H

#include <memory>

struct Order
{
    int id;
    bool buy_side;
    int quantity;
    int price;
    std::shared_ptr<Order> next;
    std::shared_ptr<Order> prev;
    Order() : next(nullptr), prev(nullptr) {};
    Order(int order_id, bool is_buy_order, int order_quantity, int order_limit) : id(order_id), buy_side(is_buy_order), quantity(order_quantity), price(order_limit), next(nullptr), prev(nullptr) {};
};
#endif