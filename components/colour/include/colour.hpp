// Not used yet - 20240930

// Refs.:
//	- https://misc.flogisoft.com/bash/tip_colors_and_formatting
//	- https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal

/*
	\e[31m sets the text color to red.
	\e[0m resets the text formatting to the default.

	ANSI Code	Color
	31			Red
	32			Green
	33			Yellow
	34			Blue
	35			Magenta
	36			Cyan
*/


#include <ostream>

namespace Color {
	enum Code {
		FG_RED      = 31,
		FG_GREEN    = 32,
		FG_BLUE     = 34,
		FG_DEFAULT  = 39,
		BG_RED      = 41,
		BG_GREEN    = 42,
		BG_BLUE     = 44,
		BG_DEFAULT  = 49
	};
	class Modifier {
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
		operator<<(std::ostream& os, const Modifier& mod) {
			return os << "\033[" << mod.code << "m";
		}
	};
}

// USAGE
// #include "colour.hpp" // namespace Color

// #include <iostream>
// using namespace std;
// int main() {
//     Color::Modifier red(Color::FG_RED);
//     Color::Modifier def(Color::FG_DEFAULT);
//     cout << "This ->" << red << "word" << def << "<- is red." << endl;
// }