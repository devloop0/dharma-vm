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
	vec.push_back("mov r1 [$1]");
	vec.push_back("mov r2 [$2]");
	vec.push_back("tupl r0 r1 r2");
	vec.push_back("mov [__temp__0] r0");
	vec.push_back("mov r0 [__temp__0]");
	vec.push_back("mov [x] r0");
	vec.push_back("mov r3 [x]");
	vec.push_back("mov r5 [$3]");
	vec.push_back("mov r6 [$4]");
	vec.push_back("tupl r4 r5 r6");
	vec.push_back("mov [__temp__1] r4");
	vec.push_back("mov r4 [__temp__1]");
	vec.push_back("mov [x] r4");
	vec.push_back("mov r9 [$1]");
	vec.push_back("mov r7 [x|r9]");
	vec.push_back("mov r10 [$45]");
	vec.push_back("mov [x|r9] r10");
	runtime r(vec);
	r.run_program();
}
