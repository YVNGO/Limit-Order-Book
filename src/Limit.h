#ifndef LIMIT_H
#define LIMIT_H

#include "Order.h"

struct Limit
{
    int price;
    int total_volume;
    std::shared_ptr<Order> head_order;
    std::shared_ptr<Order> tail_order;
    Limit(int price, int quantity) : price(price), total_volume(quantity), head_order(nullptr), tail_order(nullptr) {}
    void printOrders(){
        std::shared_ptr<Order> order = head_order;
        while(order != nullptr){
            std::cout << order->quantity << " ";
            order = order->next;
        }
        std::cout<<std::endl;
    }
    void removeHead(int& quantity, std::unordered_map<int, std::shared_ptr<Order>>& all_orders){
        total_volume-=quantity;
        head_order->quantity-=quantity;
        if(head_order->quantity == 0){
            remove(head_order, all_orders);
        }  
    }
    void insertBack(std::shared_ptr<Order> new_order){
        if(tail_order==nullptr){
            head_order = new_order;
            tail_order = new_order;
        }
        else{
            tail_order->next = new_order;
            new_order->prev = tail_order;
            tail_order = new_order;
        }
        
    }

    void remove(std::shared_ptr<Order> order, std::unordered_map<int, std::shared_ptr<Order>>& all_orders) {
        total_volume-=order->quantity;
        if(order->next != nullptr && order->prev != nullptr) {
            order->next->prev = order->prev;
            order->prev->next = order->next;
        }
        else if(order->next != nullptr){
            head_order->next->prev = nullptr;
            head_order = head_order->next;
        }
        else if(order->prev != nullptr) {
            tail_order->prev->next = nullptr;
            tail_order = tail_order->prev;
        }
        else{
            head_order = nullptr;
            tail_order = nullptr;
        }
        all_orders.erase(order->id);
    }
};

#endif