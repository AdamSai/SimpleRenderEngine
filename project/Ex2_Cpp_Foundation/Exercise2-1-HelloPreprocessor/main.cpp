#include <iostream>
#include <ctime>

#define COMPILED_TIME __TIMESTAMP__



int main() {

	// Time source: https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
	time_t unixTime = time(nullptr);
	tm* local = localtime(&unixTime);
	char* dt = asctime(local);

	std::cout << "Project was compiled at: " << COMPILED_TIME << std::endl << "Current time: " << dt << std::endl;
	return 0;
}