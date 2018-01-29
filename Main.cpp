//Main.cpp by Sam Brind (general functions and main program)
#include "Calculator.h"
//function to convert a number to correctly formatted string
std::string convertToString(const double num, const int dp) {
	if (num == 0) { return "0"; }
	//use a stringstream to get screen text with correct precision
	std::stringstream str_stream;
	if (dp > -1) { str_stream << std::fixed << std::setprecision(dp) << num; }
	if (dp == -1) { str_stream << std::fixed << std::setprecision(0) << num; }
	return str_stream.str();
}
//function to determine no of decimal places in a number
int findDP(double num) {
	//if number is exactly zero then no decimal places
	if (num == 0) { return -1; }
	std::string temporary;
	//make positive if a negative number
	if (num < 0) { num *= -1; }
	//numbers such as 0.[...] -> max 11 dp's
	if (num > 0 && num < 1) { temporary = convertToString(num, 11); }
	//numbers larger than 0 leading the decimal point
	if (num >= 1) {
		//number is too large for decimals to fit (13 digits - 1[.] = 10^11)
		if (std::trunc(std::log10(num)) >= 11) { return -1; }
		//create a 13 digit string from number
		temporary = convertToString(num, 11 - std::trunc(std::log10(num)));
	}
	int dp{ 0 };
	//loop backwards through number as string
	for (unsigned int i{ temporary.length() - 1 }; i >= 0; i--) {
		std::string character{ temporary.substr(i, 1) };
		//found decimal point
		if (character == ".") { break; }
		//add a decimal place for significant numbers after decimal point
		if (character != "0" || (character == "0" && dp > 0)) { dp++; }
	}
	if (dp == 0) { dp = -1; }
	return dp;
}
//function to convert number to scientific notation text
std::string scientific(const double num) {
	std::string temporary{ "" };
	int exponent{ 0 };
	double mantissa{ num };
	//1 digit for sign followed by 13 digits for number
	//allowed decimal places for mantissa = 13 - 2(. & e) - exponent digits [2] -> max dp = 8
	//discover +ve mantissa for large numbers
	while (mantissa >= 10 || mantissa <= -10) {
		exponent++;
		mantissa /= 10;
	}
	//discover -ve mantissa for small numbers
	while ((mantissa > 0 && mantissa < 1) || (mantissa < 0 && mantissa > -1)) {
		exponent--;
		mantissa *= 10;
	}
	//round to 8 dp.
	mantissa = floor(mantissa*pow(10, 8) + 0.5) / pow(10, 8);
	if (mantissa == 10) { mantissa = 1; exponent++; }
	if (mantissa == -10) { mantissa = -1; exponent++; }
	temporary = convertToString(mantissa, findDP(mantissa)) + "e" + convertToString(exponent, 0);
	return temporary;
}
//function to check if number is large enough for scientific representation
bool checkSize(const double num) {
	//larger than 10^13 or smaller than 10^-10 (and negative equivalents)
	if (num > 1e13 || num < -1e13 || (num > 0 && num < 1e-10) || (num < 0 && num > -1e-10)) { return true; }
	return false;
}
//function to check if a special function has been called on current number
bool checkSpecial(const std::string s) {
	//a special function would leave an ending bracket
	if (s.length() == 0 || s.back() != ')') { return false; }
	return true;
}
//main program
int main() {
	//create a calculator
	std::unique_ptr<Calculator> calc{ new Calculator() };
	//create a window and show the calculator
	sf::RenderWindow MainWindow;
	MainWindow.create(sf::VideoMode(calc->GetSize().x, calc->GetSize().y, 32), "Calculator", sf::Style::Close | sf::Style::Titlebar);
	calc->Show(MainWindow);
	return 0;
}