#include "py_lib.hpp"



std::string getpass(const char* prompt, bool show_asterisk) {
	std::string password;
	uint8 ch = 0x00;
	DWORD con_mode;
	DWORD dw_read;

	std::cout << prompt;  // printing the prompt

	HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);  // asking for the cmd handle from the windows kernel

	// get console settings and disableing echo (hide chars) and line_input (no newline)
	GetConsoleMode(stdin_handle, &con_mode);
	SetConsoleMode(stdin_handle, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	// the following code is not efficient (string coppies all data to an other spot in memory when resizing)
	while(ReadConsoleA(stdin_handle, &ch, 1, &dw_read, NULL) && ch != CON_RETURN) {  // reading entered char
		if(ch == CON_BACKSPACE) {
			if(password.length() != 0) {
				if(show_asterisk){
					std::cout <<"\b \b";  // backspace, replace, backspace
				} password.resize(password.length() - 1);  // remove last char from password
			}
		}
		else {
			password += ch;  // add last char to password
			if(show_asterisk) { std::cout << '*'; }
		}
	}
	std::cout << '\n';
	return password;
}