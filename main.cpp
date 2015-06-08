#include "includes/runtime.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using std::string;
using std::cout;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;

using namespace dharma_vm;

int main(int argc, char* argv[]) {
	vector<string> vec;
	vec.push_back("mov r0 [$0]");
	vec.push_back("mov [x_0] r0");
	vec.push_back("func temp_0");
	vec.push_back("mov r1 [x_0]");
	vec.push_back("mov r2 [$1]");
	vec.push_back("mov [x_0] r2");
	vec.push_back("efunc");
	vec.push_back("func temp_0 y_3");
	vec.push_back("mov r3 [x_0]");
	vec.push_back("mov r4 [$2]");
	vec.push_back("mov [x_0] r4");
	vec.push_back("efunc");
	vec.push_back("mov r6 [temp_0]");
	vec.push_back("list r5 r6");
	vec.push_back("mov [__temp__0] r5");
	vec.push_back("mov r5 [__temp__0]");
	vec.push_back("mov [list_4] r5");
	vec.push_back("mov r9 [$0]");
	vec.push_back("mov r10 [$'23']");
	vec.push_back("mov r7 [list_4|r9:r10]");
	vec.push_back("mov r11 [x_0]");
	vec.push_back("mov [y_4] r11");
	runtime r(vec, vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>());
	r.run_program();
}
