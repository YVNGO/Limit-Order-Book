#include <iostream>
#include "MatchingEngine.h"

int main() {
    std::string order_file_path = "<path_to_orders.txt>";
    
    MatchingEngine engine;

    engine.run(order_file_path);
    
    engine.print_order_book();

    return 0;
}