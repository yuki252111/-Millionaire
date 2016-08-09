#include "Stock.h"

//构造函数
Stock::Stock(int stockCode, String* stockName, int nowPrice, int makedealprice, float percent, int storeNumber)//构造函数
{
	this->stockCode = stockCode;
	this->stockName = stockName;
	this->nowPrice = nowPrice;
	this->makedealprice = makedealprice;
	this->percent = percent;
	this->storeNumber = storeNumber;
}

Stock* Stock::create(int stockCode, String* stockName, int nowPrice, int makedealprice, float percent, int storeNumber)
{
	Stock* stock = new Stock(stockCode, stockName, nowPrice, makedealprice, percent, storeNumber);
	stock->init();
	stock->autorelease();
	return stock;
}
//各种get和set方法
void Stock::setCode(int stockCode)
{
	this->stockCode = stockCode;
}
int Stock::getCode()
{
	return stockCode;
}

void Stock::setStockName(String* stockName)
{
	this->stockName = stockName;
}
String* Stock::getStockName()
{
	return stockName;
}

void Stock::setNowPrice(int nowPrice)
{
	this->nowPrice = nowPrice;
}
int Stock::getNowPrice()
{
	return nowPrice;
}

void Stock::setMakedealprice(int makedealprice)
{
	this->makedealprice = makedealprice;
}
int Stock::getMakedealprice()
{
	return makedealprice;
}

void Stock::setPercent(float percent)
{
	this->percent = percent;
}
float Stock::getPercent()
{
	return percent;
}

void Stock::setStoreNumber(int storeNumber)
{
	this->storeNumber = storeNumber;
}
int Stock::getStoreNumber()
{
	return storeNumber;
}