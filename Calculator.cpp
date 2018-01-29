//Calculator.cpp by Sam Brind (Calculator member functions)
#include "Calculator.h"
//declare general functions (defined in Main.cpp)
std::string convertToString(double num, int dp);
int findDP(double num);
std::string scientific(double num);
bool checkSize(double num);
bool checkSpecial(std::string s);
//calculator member functions
//default constructor
Calculator::Calculator():screen_value{ 0 }, store_value{ 0 }, dp{ -1 }, previous_operator{ 0 }, refresh{ false } {
	std::cout << "Creating a calculator..." << std::endl;
}
//destructor
Calculator::~Calculator() { std::cout << "Destroying a calculator..." << std::endl; }
//function to get calculator size
const sf::Vector2i Calculator::GetSize() const { return sf::Vector2i(CALC_WIDTH, CALC_HEIGHT); }
//function to show the calculator in a window
void Calculator::Show(sf::RenderWindow &window) {
	//load the background calculator image
	if (!background.loadFromFile("calc.png")) {
		std::cerr << "Error: can't load 'calc.png'!" << std::endl;
		exit(1);
	}
	//load the font
	sf::Font font;
	if (!font.loadFromFile("whitrabt.ttf")) {
		std::cerr << "Error: can't load 'whitrabt.ttf'!" << std::endl;
		exit(2);
	}
	//setup the buttons
	SetupButtons();
	//setup the main screen text, secondary text line and memory indicator
	screen_text.setFont(font); second_text.setFont(font); memory_text.setFont(font);
	screen_text.setString("0");
	screen_text.setCharacterSize(65); second_text.setCharacterSize(24); memory_text.setCharacterSize(24);
	screen_text.setFillColor(sf::Color(23, 167, 97)); second_text.setFillColor(sf::Color(23, 167, 97)); memory_text.setFillColor(sf::Color(23, 167, 97));
	//refresh the display
	Refresh(window);
	//await user response
	GetResponse(window);
}
//function to refresh the calculator display
void Calculator::Refresh(sf::RenderWindow &window) {
	//clear the window
	window.clear();
	//draw background and text
	sf::Sprite sprite(background);
	window.draw(sprite);
	screen_text.setOrigin(screen_text.getGlobalBounds().width, 21.5);
	screen_text.setPosition(532, 80);
	window.draw(screen_text);
	second_text.setOrigin(second_text.getGlobalBounds().width, 10);
	second_text.setPosition(514, 51);
	window.draw(second_text);
	memory_text.setOrigin(memory_text.getGlobalBounds().width, 9.5);
	memory_text.setPosition(540, 20);
	window.draw(memory_text);
	//display the window
	window.display();
}
//function to output the current calculator values
void Calculator::PrintStatus() const {
	std::cout << "Calculator values:" << std::endl;
	std::cout << "   Screen value:" << screen_value << std::endl;
	std::cout << "   Store value:" << store_value << std::endl;
	std::cout << "   Memory value:" << memory_value << std::endl;
	std::cout << "   Decimal places:" << dp << std::endl;
	std::cout << "   Previous operator:" << previous_operator << std::endl;
}
//function to setup clickable button regions (action + region)
void Calculator::SetupButtons() {
	//memory buttons
	CalcButton MSave,MRecall,MCancel,MPlus,MMinus;
	MSave.action = MemSave; MRecall.action = MemRecall; MCancel.action = MemCancel;
	MPlus.action = MemAdd; MMinus.action = MemMinus;
	MSave.rect.top = MRecall.rect.top = MCancel.rect.top = MPlus.rect.top = MMinus.rect.top = 10;
	MSave.rect.left = 307; MRecall.rect.left = 346; MCancel.rect.left = 385; MPlus.rect.left = 424; MMinus.rect.left = 463;
	MSave.rect.height = MRecall.rect.height = MCancel.rect.height = MPlus.rect.height = MMinus.rect.height = 22;
	MSave.rect.width = MRecall.rect.width = MCancel.rect.width = MPlus.rect.width = MMinus.rect.width = 37;
	CalcButtons.push_back(MSave); CalcButtons.push_back(MRecall); CalcButtons.push_back(MCancel);
	CalcButtons.push_back(MPlus); CalcButtons.push_back(MMinus);
	//number buttons
	CalcButton ZeroButton,OneButton,TwoButton,ThreeButton,FourButton,FiveButton,SixButton,SevenButton,EightButton,NineButton;
	ZeroButton.action = Zero; OneButton.action = One; TwoButton.action = Two; ThreeButton.action = Three; FourButton.action = Four;
	FiveButton.action = Five; SixButton.action = Six; SevenButton.action = Seven; EightButton.action = Eight; NineButton.action = Nine;
	ZeroButton.rect.top = OneButton.rect.top = TwoButton.rect.top = ThreeButton.rect.top = FourButton.rect.top = 144;
	FiveButton.rect.top = SixButton.rect.top = SevenButton.rect.top = EightButton.rect.top = NineButton.rect.top = 191;
	ZeroButton.rect.left = FiveButton.rect.left = 105;
	OneButton.rect.left = SixButton.rect.left = 167;
	TwoButton.rect.left = SevenButton.rect.left = 229;
	ThreeButton.rect.left = EightButton.rect.left = 291;
	FourButton.rect.left = NineButton.rect.left = 353;
	ZeroButton.rect.height = OneButton.rect.height = TwoButton.rect.height = ThreeButton.rect.height = FourButton.rect.height = 45;
	FiveButton.rect.height = SixButton.rect.height = SevenButton.rect.height = EightButton.rect.height = NineButton.rect.height = 42;
	ZeroButton.rect.width = OneButton.rect.width = TwoButton.rect.width = ThreeButton.rect.width = FourButton.rect.width = 60;
	FiveButton.rect.width = SixButton.rect.width = SevenButton.rect.width = EightButton.rect.width = NineButton.rect.width = 60;
	CalcButtons.push_back(ZeroButton); CalcButtons.push_back(OneButton); CalcButtons.push_back(TwoButton);
	CalcButtons.push_back(ThreeButton); CalcButtons.push_back(FourButton); CalcButtons.push_back(FiveButton);
	CalcButtons.push_back(SixButton); CalcButtons.push_back(SevenButton); CalcButtons.push_back(EightButton);
	CalcButtons.push_back(NineButton);
	//operator buttons
	CalcButton CButton,CEButton,BackButton,PlusButton,MinusButton,MultiplyButton,DivideButton,EqualsButton;
	CButton.action = Cancel; CEButton.action = CancelEntry; BackButton.action = Back;
	PlusButton.action = Plus; MinusButton.action = Minus; MultiplyButton.action = Multiply; DivideButton.action = Divide; EqualsButton.action = Equals;
	CButton.rect.top = CEButton.rect.top = BackButton.rect.top = 144;
	PlusButton.rect.top = MinusButton.rect.top = EqualsButton.rect.top = 165;
	MultiplyButton.rect.top = DivideButton.rect.top = 199;
	CButton.rect.left = PlusButton.rect.left = MultiplyButton.rect.left = 437;
	CEButton.rect.left = MinusButton.rect.left = DivideButton.rect.left = 474;
	BackButton.rect.left = EqualsButton.rect.left = 511;
	CButton.rect.height = CEButton.rect.height = BackButton.rect.height = 19;
	PlusButton.rect.height = MinusButton.rect.height = MultiplyButton.rect.height = DivideButton.rect.height = 32;
	EqualsButton.rect.height = 49;
	BackButton.rect.width = EqualsButton.rect.width = 40;
	CButton.rect.width = CEButton.rect.width = PlusButton.rect.width = MinusButton.rect.width = MultiplyButton.rect.width = DivideButton.rect.width = 35;
	CalcButtons.push_back(CButton); CalcButtons.push_back(CEButton); CalcButtons.push_back(BackButton);
	CalcButtons.push_back(PlusButton); CalcButtons.push_back(MinusButton); CalcButtons.push_back(MultiplyButton);
	CalcButtons.push_back(DivideButton); CalcButtons.push_back(EqualsButton);
	//special operator buttons
	CalcButton DecimalButton,ReciprocalButton,SignButton,PercentButton,RootButton;
	DecimalButton.action = Decimal; ReciprocalButton.action = Reciprocal; SignButton.action = Sign;
	PercentButton.action = Percentage; RootButton.action = Root;
	DecimalButton.rect.top = 144; DecimalButton.rect.left = 47; DecimalButton.rect.height = 25;
	DecimalButton.rect.left = SignButton.rect.left = RootButton.rect.left = 47;
	ReciprocalButton.rect.left = PercentButton.rect.left = 12;
	ReciprocalButton.rect.top = SignButton.rect.top = 171;
	PercentButton.rect.top = RootButton.rect.top = 203;
	ReciprocalButton.rect.height = SignButton.rect.height = PercentButton.rect.height = RootButton.rect.height = 30;
	DecimalButton.rect.width = SignButton.rect.width = RootButton.rect.width = 34;
	ReciprocalButton.rect.width = PercentButton.rect.width = 33;
	CalcButtons.push_back(DecimalButton); CalcButtons.push_back(ReciprocalButton); CalcButtons.push_back(SignButton);
	CalcButtons.push_back(PercentButton); CalcButtons.push_back(RootButton);
}
//function to gather a user response
void Calculator::GetResponse(sf::RenderWindow &window) {
	sf::Event CalcEvent;
	while (true) {
		while (window.pollEvent(CalcEvent)) {
			//mouse click
			if (CalcEvent.type == sf::Event::MouseButtonPressed) { HandleClick(CalcEvent.mouseButton.x, CalcEvent.mouseButton.y); }
			//close calculator
			if (CalcEvent.type == sf::Event::Closed) { exit(0); }
			//refresh the calculator display
			Refresh(window);
		}
	}
}
//function to handle a click event
void Calculator::HandleClick(const int x, const int y) {
	//iterate over list of calculator buttons
	for (auto it{ CalcButtons.begin() }; it != CalcButtons.end(); it++) {
		sf::Rect<int> ButtonRect{ (*it).rect };
		//check if click is within a button region
		if (ButtonRect.top + ButtonRect.height > y && ButtonRect.top < y && ButtonRect.left < x && ButtonRect.left + ButtonRect.width > x) {
			//call member function depending on button's action
			switch ((*it).action) {
				case MemSave:
					MemS();
					break;
				case MemRecall:
					MemR();
					break;
				case MemCancel:
					MemC();
					break;
				case MemAdd:
					MemOperate(true);
					break;
				case MemMinus:
					MemOperate(false);
					break;
				case Zero:
					NumberKey(0);
					break;
				case One:
					NumberKey(1);
					break;
				case Two:
					NumberKey(2);
					break;
				case Three:
					NumberKey(3);
					break;
				case Four:
					NumberKey(4);
					break;
				case Five:
					NumberKey(5);
					break;
				case Six:
					NumberKey(6);
					break;
				case Seven:
					NumberKey(7);
					break;
				case Eight:
					NumberKey(8);
					break;
				case Nine:
					NumberKey(9);
					break;
				case Plus:
					OperatorKey(1);
					break;
				case Minus:
					OperatorKey(2);
					break;
				case Multiply:
					OperatorKey(3);
					break;
				case Divide:
					OperatorKey(4);
					break;
				case Equals:
					OperatorKey(0);
					break;
				case Decimal:
					DecimalKey();
					break;
				case Back:
					BackKey();
					break;
				case Cancel:
					CancelKey(true);
					break;
				case CancelEntry:
					CancelKey(false);
					break;
				case Root:
					OperatorKey(5);
					break;
				case Sign:
					OperatorKey(7);
					break;
				case Reciprocal:
					OperatorKey(6);
					break;
				case Percentage:
					OperatorKey(8);
					break;
			}
		}
	}
}
//function to press a number key
void Calculator::NumberKey(const int num) {
	std::cout << "Pressed [" << num << "] key" << std::endl;
	RemoveSpecial();
	std::string current{ screen_text.getString() };
	//clear screen if refresh is true
	if (refresh == true) { screen_text.setString("0"); screen_value = 0; dp = -1; refresh = false; }
	//prevent input past 13 digits
	if (screen_value >= 1 && (log10(screen_value) + dp >= 11)) { return; }
	if (screen_value <= -1 && (log10(-1 * screen_value) + dp >= 11)) { return; }
	if (screen_value > -1 && screen_value < 1 && (dp == 11)) { return; }
	//before a decimal point
	if (dp < 0) {
		if (screen_value == 0) { current = ""; } //prevent negligible zeros e.g. 0 -> press [1] -> 1 NOT 01
		//increase (+ve) or decrease (-ve) screen value
		screen_value *= 10;
		(screen_value < 0) ? screen_value -= num : screen_value += num; 
	}
	//after a decimal point
	if (dp > -1) {
		dp++;
		(screen_value < 0) ? screen_value -= num / std::pow(10, dp) : screen_value += num / std::pow(10, dp);
	}
	//update screen text
	screen_text.setString(current += std::to_string(num));
}
//function to press the decimal key
void Calculator::DecimalKey() {
	std::cout << "Pressed [.] key" << std::endl;
	//make sure a decimal point hasn't already been placed
	if (dp == -1) {
		dp++;
		std::string current{ screen_text.getString() };
		screen_text.setString(current += ".");
	}
}
//function to press an operator key
void Calculator::OperatorKey(const int num) {
	//do nothing if division by zero error is shown
	if (screen_text.getString() == "Div. by zero!") { return; }
	std::string current{ second_text.getString() };
	//perform basic operations (+,-,*,/)
	if (num < 5) {
		//store first number entered
		if (current == "") { store_value = screen_value; }
		//add number to secondary line
		SecondNumber(current);
		//add operation symbol to secondary line
		if (num == 0) { std::cout << "Pressed [=] key" << std::endl; }
		if (num == 1) { std::cout << "Pressed [+] key" << std::endl; current += " + "; }
		if (num == 2) { std::cout << "Pressed [-] key" << std::endl; current += " - "; }
		if (num == 3) { std::cout << "Pressed [*] key" << std::endl; current += " * "; }
		if (num == 4) { std::cout << "Pressed [/] key" << std::endl; current += " / "; }
		//deal with any overflow
		Overflow(current);
		//perform previous operation
		if (previous_operator == 1) { store_value += screen_value; }
		if (previous_operator == 2) { store_value -= screen_value; }
		if (previous_operator == 3) { store_value *= screen_value; }
		if (previous_operator == 4) { (screen_value == 0) ? DivisionByZero() : store_value /= screen_value; }
		//set the previous operator
		previous_operator = num;
		//update screen value as result
		screen_value = store_value;
		ScreenNumber();
		//reset second line and store value for an equals
		if (num == 0) { second_text.setString(""); store_value = 0; }
	}
	//special functions
	if (num > 4) {
		RemoveSpecial();
		bool store{ false };
		if (current == "") { store = true; }
		//add special function text if needed
		if (num < 7) {
			current = second_text.getString();
			if (num == 5) { current += "Sqrt("; }
			if (num == 6) { current += "Reciproc("; }
			SecondNumber(current);
			current += ')';
			Overflow(current);
		}
		//perform functions on screen value
		if (num == 5) { 
			//square root
			std::cout << "Pressed [square root] key" << std::endl;
			screen_value = sqrt(screen_value);
		}
		if (num == 6) {
			//reciprocal
			std::cout << "Pressed [reciprocal] key" << std::endl; 
			(screen_value == 0) ? DivisionByZero() : screen_value = 1 / screen_value;
		}
		if (num == 7) { 
			//change sign
			std::cout << "Pressed [+/-] key" << std::endl; 
			current = screen_text.getString();
			if (screen_value == 0) { return; }
			//change sign character
			if (screen_value > 0) { current = '-' + current; }
			if (screen_value < 0) { current.erase(0, 1); }
			//flip sign of value
			screen_value *= -1; 
		}
		if(num == 8) {
			//percentage
			std::cout << "Pressed [percentage] key" << std::endl;
			screen_value = screen_value * store_value / 100;
		}
		ScreenNumber();
		//store first number entered if special function called on it first
		if (store) { store_value = screen_value; }
		//prevent refreshing after sign change
		if (num == 7) { refresh = false; }
	}
}
//function to press cancel keys (full? cancel)
void Calculator::CancelKey(bool full) {
	if (!full) { std::cout << "Pressed [CE] key" << std::endl; }
	//do a full cancel for div. by zero error
	if (screen_text.getString() == "Div. by zero!") { full = true; }
	//cancel current entry
	RemoveSpecial();
	screen_value = 0;
	dp = -1;
	screen_text.setString("0");
	//full cancel
	if (full) {
		std::cout << "Pressed [C] key" << std::endl;
		second_text.setString("");
		previous_operator = 0;
		store_value = 0;
	}
}
//function to press back arrow key
void Calculator::BackKey() {
	std::cout << "Pressed [<-] key" << std::endl;
	std::string current{second_text.getString()};
	//cancel entry if special function has been called
	if (checkSpecial(current)) { CancelKey(false); }
	current = screen_text.getString();
	//ignore if scientific number is displayed or division by zero has occured
	if (current.find('e') != std::string::npos || current == "Div. by zero!") { return; }
	//if one digit, revert to zero
	if (dp == -1 && ((screen_value >= 0 && screen_value < 10) || (screen_value <= 0 && screen_value > -10))) {
		screen_text.setString("0");
		screen_value = 0;
		return;
	}
	//remove last digit
	if (dp == -1) { screen_value = std::trunc(screen_value / 10);}
	//remove last decimal digit
	if (dp > -1) {
		dp--;
		screen_value = std::trunc(screen_value * std::pow(10, dp));
		screen_value /= std::pow(10,dp);
	}
	//remove character of last digit
	screen_text.setString(current.erase(current.length() - 1, 1));
}
//function to press M+ (true) or M- (false) key
void Calculator::MemOperate(const bool add) {
	if (add) {
		//(M+) -> add screen value to stored memory value
		std::cout << "Pressed [M+] key" << std::endl;
		memory_value += screen_value;
	} else {
		//(M-) -> subtract screen value from stored memory value
		std::cout << "Pressed [M-] key" << std::endl;
		memory_value -= screen_value;
	}
	//indicate if memory has been created or destroyed
	if (memory_text.getString() == "") { memory_text.setString("M"); }
	if (memory_value == 0) { memory_text.setString(""); }
}
//function to press MS (memory save) key
void Calculator::MemS() {
	std::cout << "Pressed [MS] key" << std::endl;
	memory_value = screen_value;
	(dp <= 0) ? dp_m = -1 : dp_m = dp;
	(screen_value == 0) ? memory_text.setString("") : memory_text.setString("M");
}
//function to press MR (memory recall) key
void Calculator::MemR() {
	std::cout << "Pressed [MR] key" << std::endl;
	screen_value = memory_value;
	dp = dp_m;
	ScreenNumber();
}
//function to press MC (memory cancel) key
void Calculator::MemC() {
	std::cout << "Pressed [MC] key" << std::endl;
	memory_value = 0; dp_m = -1;
	memory_text.setString("");
}
//function to deal with overflow
void Calculator::Overflow(const std::string string_in) {
	//prevent overflow (max secondary text line = 36 digits)
	if (string_in.length() > 36) {
		second_text.setString(string_in.substr(string_in.length() - 36, 36));
	} else {
		//no overflow
		second_text.setString(string_in);
	}
}
//deal with division by zero
void Calculator::DivisionByZero() {
	screen_text.setString("Div. by zero!");
}
//function to display number to main screen
void Calculator::ScreenNumber() {
	//if division by zero error is displayed, do nothing
	if (screen_text.getString() == "Div. by zero!") { return; }
	//convert to sicentific notation if large enough
	if (checkSize(screen_value)) {
		screen_text.setString(scientific(screen_value));
	} else {
		//find decimal places and then display
		dp = findDP(screen_value);
		screen_text.setString(convertToString(screen_value, dp));
	}
	refresh = true;
}
//function to add a number to the secondary text line
void Calculator::SecondNumber(std::string &current) {
	//no need to add number if a special function has been called
	if (!checkSpecial(current)) {
		if (checkSize(screen_value)) {
			current += scientific(screen_value);
		} else {
			current += convertToString(screen_value, findDP(screen_value));
		}
	}
}
//function to remove special function text (Sqrt(...) or Reciproc(...))
void Calculator::RemoveSpecial() {
	std::string current{ second_text.getString() };
	if (checkSpecial(current)) {
		unsigned int pos{ current.find_last_of("SR") };
		second_text.setString(current.erase(pos, current.length() - pos));
	}
}