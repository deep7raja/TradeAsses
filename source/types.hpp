#pragma once
#include <cstring>

namespace TradeTypes
{
  constexpr int MAX_LENGTH_SYMBOL = 10;
  constexpr int MAX_LENGTH_TIME = 10;
  enum OrderType
  {
    LIMIT = 0,
    STOP,
    ORDER_TYPE_MAX
  };

  enum ExitCode
  {
    SUCCESS = 0,
    INVALID_ORDER_FORMAT
  };

  class Order
  {
  public:
    int index;
    char symbol[MAX_LENGTH_SYMBOL + 1];
    OrderType type;
    float price;
    int quantity;

    Order(int a_index, const char a_symbol[], OrderType a_type, float a_price, int a_quanity)
     : index(a_index), type(a_type), price(a_price), quantity(a_quanity)
    {
      memcpy(symbol, a_symbol, MAX_LENGTH_SYMBOL + 1);
    }

    bool isValid() const
    {
      return !(('\0' == symbol[0]) || (OrderType::ORDER_TYPE_MAX == type) || (0 == price) || (0 == quantity));
    }

    friend bool operator<(const Order &obj1, const Order &obj2)
    {
      return obj1.price < obj2.price;
    }
  };

  class Tick
  {
  public:
    char symbol[MAX_LENGTH_SYMBOL + 1];
    float price;
    char timestamp[MAX_LENGTH_TIME + 1]; //using string time for simplicity, since it is not used except for printing

    Tick(const char a_symbol[], float a_price, const char a_timestamp[])
     : price(a_price)
    {
      memcpy(symbol, a_symbol, MAX_LENGTH_SYMBOL + 1);
      memcpy(timestamp, a_timestamp, MAX_LENGTH_TIME +1);
    }

    bool isValid()
    {
      return !(('\0' == symbol[0]) || (0 == price) || ('\0' == timestamp[0]));
    }
  };
}
