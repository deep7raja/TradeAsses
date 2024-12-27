#pragma once
#include <cstdio>
#include <unordered_map>
#include <string>
#include <deque>
#include <array>
#include "types.hpp"

using namespace std;
using namespace TradeTypes;

class Trade
{
  // using namespace std;
  // using namespace TradeTypes;  //moved it outside as my compiler was giving error here
  int current_order_index = 0;
  unordered_map<string, array<deque<Order>, OrderType::ORDER_TYPE_MAX>> orders_map;
  //key string will represent the symbol of the order,
  //different order type(limit/stop/etc) will stored in different array of sorted queue
  //using deque so that get/delete from front and back be done in O(1) since newTick function is called very frequently
  //it will have O(log N) and O(N) in worst case for addOrder function

  Trade(){}
public:
  ExitCode addOrder(TradeTypes::Order order) //not taking by reference because it is changed in the function, which may/may not be desired
  {
    if(!order.isValid()){
      printf("[Error]: invalid order rcvd"); //contents of the order can be printed for further debugging
      return ExitCode::INVALID_ORDER_FORMAT;
    }
    order.index = ++current_order_index;
    auto &q = orders_map[order.symbol][size_t(order.type)];   // O(1)
    auto l_itr = std::lower_bound(q.begin(), q.end(), order); // O(log N)
    q.insert(l_itr, order);                                   // O(N) in worst case

    return ExitCode::SUCCESS;
  }

  ExitCode newTick(const TradeTypes::Tick &tick)
  {
    auto l_map_itr = orders_map.find(string(tick.symbol)); //assessing only orders for current tick in O(1)
    if(orders_map.end() == l_map_itr)
    {
      return ExitCode::SUCCESS;
    }

    //fullfilling Limit orders in O(1)
    auto &lr_limit_orders_q = l_map_itr->second[OrderType::LIMIT];  //O(1)

    while(!lr_limit_orders_q.empty())
    {
      const Order &order = lr_limit_orders_q.back(); //get the order with highest price(last element of q)
      if(order.price >= tick.price) //assuming price is precise upto two decimal digits, skipping additional check for floats comparision
      {
        printf("Order %d executed at %s, Price: %.2f, Quanity: %d\n",
         order.index, tick.timestamp, tick.price, order.quantity);
        lr_limit_orders_q.pop_back();
      }
      else
      {
        break;
      }
    }

    //fullfilling Stop orders
    auto &lr_stop_orders_q = l_map_itr->second[OrderType::STOP];

    while(!lr_stop_orders_q.empty())
    {
      const Order &order = lr_stop_orders_q.front();
      if(order.price <= tick.price)
      {
        printf("Stop Order %d executed at %s, Price: %.2f, Quanity: %d\n",
         order.index, tick.timestamp, tick.price, order.quantity); //get the order with lowest price(first element of q)
        lr_stop_orders_q.pop_front();
      }
      else
      {
        break;
      }
    }

    //time complexity: O(K) whre, k is number of orders fullfilled
    return ExitCode::SUCCESS;
  }
  public:
  static Trade* getInstance()
  {
    static Trade obj;
    return &obj;
  }
};
