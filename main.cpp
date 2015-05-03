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
	vec.push_back("mov r0 [$3]");
	vec.push_back("mov r1 [$4]");
	vec.push_back("lt [$2] r0");
	runtime r(vec);
	r.run_program();
}
