#include "Trade.hpp"
#include "types.hpp"

int main()
{
	Trade *lp_trade = Trade::getInstance();
	if(ExitCode::SUCCESS != lp_trade->addOrder(Order(0, "AAPL", OrderType::LIMIT, 100, 10))){
		//error handling
	}

	//error checks not added for simplicity
	lp_trade->addOrder(Order(0, "AAPL", OrderType::LIMIT, 105, 5));
	lp_trade->addOrder(Order(0, "AAPL", OrderType::LIMIT, 102, 15));
	lp_trade->addOrder(Order(0, "AAPL", OrderType::LIMIT, 98, 15));
	lp_trade->addOrder(Order(0, "GOOG", OrderType::LIMIT, 200, 10));
	lp_trade->addOrder(Order(0, "GOOG", OrderType::LIMIT, 201, 15));
	lp_trade->addOrder(Order(0, "GOOG", OrderType::LIMIT, 198, 10));

	lp_trade->newTick(Tick("AAPL", 106, "08:58"));
	lp_trade->newTick(Tick("AAPL", 104, "09:00"));
	lp_trade->newTick(Tick("AAPL", 102, "09:01"));
	lp_trade->newTick(Tick("AAPL", 98, "09:02"));
	lp_trade->newTick(Tick("GOOG", 200, "09:03"));
}