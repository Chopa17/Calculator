//Calculator.h by Sam Brind (Class definitions)
#ifndef CALC_H
#define CALC_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
//calculator class
class Calculator {
public:
	Calculator();
	~Calculator();
	//the type of buttons that can be pressed
	enum Button {
		Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		Plus, Minus, Multiply, Divide, Equals, Decimal, Back, Reciprocal, Root, Sign, Percentage, Cancel, CancelEntry,
		MemSave, MemRecall, MemCancel, MemAdd, MemMinus
	};
	//calculator button is combination of a rectangular boundary and the type of button
	struct CalcButton {
	public:
		sf::Rect<int> rect;
		Button action;
	};
	const sf::Vector2i GetSize() const;
	void SetupButtons();
	void Show(sf::RenderWindow &window);
	void Refresh(sf::RenderWindow &window);
	void PrintStatus() const;
	void NumberKey(int num);
	void DecimalKey();
	void OperatorKey(int num);
	void CancelKey(bool full);
	void BackKey();
	void MemOperate(bool add);
	void MemS();
	void MemC();
	void MemR();
private:
	const int CALC_HEIGHT{ 240 };
	const int CALC_WIDTH{ 563 };
	bool refresh; //TRUE pressing a number key refreshes the no display
	std::list<CalcButton> CalcButtons; //lis of all the calculator buttons
	int dp, dp_m;  //decimal places
	double screen_value; //no on screen
	double store_value; //no being stored 
	double memory_value; //no in memory
	int previous_operator; //last operator pressed
	sf::Texture background;
	sf::Text screen_text;
	sf::Text second_text;
	sf::Text memory_text;
	void GetResponse(sf::RenderWindow &window);
	void HandleClick(int x, int y);
	void Overflow(std::string string_in);
	void ScreenNumber();
	void SecondNumber(std::string &current);
	void RemoveSpecial();
	void DivisionByZero();
};
#endif