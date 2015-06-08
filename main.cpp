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
	vec.push_back("mov r1 [$'qwer wer t wqe r t ']");
	vec.push_back("mov r2 [$\" asdf sd f dghfgj khg\"]");
	vec.push_back("list r0 r1 r2");
	vec.push_back("mov [__temp__0] r0");
	vec.push_back("mov r0 [__temp__0]");
	vec.push_back("mov [x_0] r0");
	vec.push_back("mov r3 [$\"23\"]");
	vec.push_back("mov [y_0] r3");
	vec.push_back("func temp_0 x_1");
	vec.push_back("mov r4 [x_1]");
	vec.push_back("mov r5 [y_0]");
	vec.push_back("mov [x_1] r5");
	vec.push_back("efunc");
	vec.push_back("mov r10 [$1]");
	vec.push_back("list r9 r10");
	vec.push_back("mov [__temp__1] r9");
	vec.push_back("mov r12 [$0]");
	vec.push_back("mov r9 [__temp__1|r12]");
	vec.push_back("mov r7 [x_0|r9]");
	vec.push_back("mov r6 [temp_0:r7]");
	vec.push_back("mov r13 [x_0]");
	vec.push_back("mov [z_2] r13");
	runtime r(vec, vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>());
	r.run_program();
}
