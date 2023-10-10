#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <unordered_map>
#include <memory>
#include "Limit.h"
#include "Order.h"
#include <optional>

class OrderBook
{
private:
    std::map<int, std::shared_ptr<Limit>> bids, asks;
    std::unordered_map<int, std::shared_ptr<Order>> all_orders;
    void addBidOrder(std::shared_ptr<Order> order);
    void addAskOrder(std::shared_ptr<Order> order);
    void executeOrder(bool after_buy_side_order);
    std::optional<int> getSpread();
    void remove(std::shared_ptr<Limit> limit, bool is_bid);
    void remove(int& quantity, std::shared_ptr<Limit> best_bid, std::shared_ptr<Limit> best_ask);
    void logTrade(int& quantity, int& price);
public:
    void addOrder(std::shared_ptr<Order> new_order);
    void remove(int& order_id, int& order_quantity);
    void print();
};

void OrderBook::addOrder(std::shared_ptr<Order> new_order){
    new_order->buy_side ? addBidOrder(new_order) : addAskOrder(new_order);
    all_orders[new_order->id] = new_order;
    executeOrder(new_order->buy_side);
}

void OrderBook::addBidOrder(std::shared_ptr<Order> order){
    std::shared_ptr<Limit> limit;
    if(bids.count(order->price) > 0){
        limit = bids[order->price];
        limit->total_volume+=order->quantity;
    }        
    else{
        limit = std::make_shared<Limit>(order->price, order->quantity);
        bids[order->price] = limit;
    }
    limit->insertBack(order);
}

void OrderBook::addAskOrder(std::shared_ptr<Order> order){
    std::shared_ptr<Limit> limit;
    if(asks.count(order->price) > 0){
        limit = asks[order->price];
        limit->total_volume+=order->quantity;
    }        
    else{
        limit = std::make_shared<Limit>(order->price, order->quantity);
        asks[order->price] = limit;
    }
    limit->insertBack(order);
}

void OrderBook::print(){
    const std::string border{"=================\n"};
    const std::string mid_border{"------------\n"};

    std::cout << border;
    std::cout << "ASK\n";
    for(auto it = asks.rbegin(); it!=asks.rend(); ++it){
        std::cout << it->first << ": ";
        it->second->printOrders();
    }
    std::cout << mid_border;
    for(auto it = bids.rbegin(); it!=bids.rend(); ++it){
        std::cout << it->first << ": ";
        it->second->printOrders();
    }
    std::cout << "BID\n";
    std::cout << border;
}

std::optional<int> OrderBook::getSpread(){
    std::optional<int> result;
    std::optional<int> best_bid;
    std::optional<int> best_ask;

    if(bids.rbegin()!=bids.rend())
        best_bid = bids.rbegin()->first;
    if(asks.begin()!=asks.end())
        best_ask = asks.begin()->first;

    if(best_bid.has_value() && best_ask.has_value()){
        result = best_ask.value() - best_bid.value();
    }

    return result;
}

void OrderBook::remove(std::shared_ptr<Limit> limit, bool is_bid) {
    auto removeLimit = [&](std::map<int, std::shared_ptr<Limit>>& map){
        map[limit->price].reset();
        map.erase(limit->price);
    };
    removeLimit(is_bid ? bids : asks);
}

void OrderBook::remove(int& quantity, std::shared_ptr<Limit> best_bid, std::shared_ptr<Limit> best_ask){
    best_bid->removeHead(quantity, all_orders);
    if(best_bid->total_volume == 0)
        remove(best_bid, true);
    
    best_ask->removeHead(quantity, all_orders);
    if(best_ask->total_volume == 0)
        remove(best_ask, false);
}

void OrderBook::logTrade(int& quantity, int& price) {
    std::cout << quantity << " shares of XYZ were sold at price " << price << " USD\n";
}

void OrderBook::executeOrder(bool after_buy_side_order){
    std::optional<int> spread = getSpread();
    while(spread.has_value() && spread.value()<=0){
        auto best_bid = bids.rbegin()->second;
        auto best_ask = asks.begin()->second;
        int price = after_buy_side_order ? best_ask->price : best_bid->price;
        int quantity = std::min(best_bid->head_order->quantity, best_ask->head_order->quantity);
        remove(quantity, best_bid, best_ask);
        logTrade(quantity, price);
        spread = getSpread();
    }
}

void OrderBook::remove(int& order_id, int& order_quantity) {
    if(all_orders.count(order_id) == 0 || all_orders[order_id]->quantity != order_quantity)
        return;
    std::shared_ptr<Order> order = all_orders[order_id];
    std::shared_ptr<Limit> limit;

    limit = order->buy_side ? bids[order->price] : asks[order->price];
    limit->remove(order, all_orders);
        if(limit->total_volume == 0)
            remove(limit, order->buy_side);

    order.reset();
}

#endif