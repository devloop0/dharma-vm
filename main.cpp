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
	vector<string> vec;;
	vec.push_back("func generate_quadratic_0 a_1 b_1 c_1 ");
vec.push_back("mov r0 [a_1]");
vec.push_back("mov r1 [$1]");
vec.push_back("add r0 r1");
vec.push_back("dmov [A_1] r0");
vec.push_back("mov r2 [b_1]");
vec.push_back("mov r3 [$1]");
vec.push_back("add r2 r3");
vec.push_back("dmov [B_1] r2");
vec.push_back("mov r4 [c_1]");
vec.push_back("mov r5 [$1]");
vec.push_back("add r4 r5");
vec.push_back("dmov [C_1] r4");
vec.push_back("lambda __temp__0 x_2 ");
vec.push_back("mov r6 [x_2]");
vec.push_back("mov r7 [$1]");
vec.push_back("add r6 r7");
vec.push_back("dmov [X_2] r6");
vec.push_back("mov r8 [A_1]");
vec.push_back("mov r9 [X_2]");
vec.push_back("mov r10 [$2]");
vec.push_back("exp r9 r10");
vec.push_back("mul r8 r9");
vec.push_back("mov r11 [B_1]");
vec.push_back("mov r12 [X_2]");
vec.push_back("mul r11 r12");
vec.push_back("add r8 r11");
vec.push_back("mov r13 [C_1]");
vec.push_back("add r8 r13");
vec.push_back("ret r8");
vec.push_back("elambda");
vec.push_back("mov r6 [__temp__0]");
vec.push_back("ret r6");
vec.push_back("efunc");
vec.push_back("mov r16 [$11]");
vec.push_back("mov r17 [$15]");
vec.push_back("mov r18 [$44]");
vec.push_back("mov r19 [$2]");
vec.push_back("mov r15 [generate_quadratic_0:r16,r17,r18:r19]");
vec.push_back("mov r20 [$'\n']");
vec.push_back("mov r14 [print:r15,r20]");
	shared_ptr<runtime_variable> print_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_print, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
		-1, -1, builtins::builtin_print, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
		make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>(), vector<vector<shared_ptr<runtime_variable>>>(), 
			vector<vector<shared_ptr<runtime_variable>>>()), type_information_list::_func);
	shared_ptr<function> print_function = make_shared<function>(builtins::builtin_print, vector<string>(), vector<string>(), print_variable, true, false);
	vector<shared_ptr<function>> func_list;
	func_list.push_back(print_function);
	vector<shared_ptr<runtime_variable>> rvar_list;
	rvar_list.push_back(print_variable);
	vector<vector<shared_ptr<runtime_variable>>> scope_stack;
	runtime r(vec, rvar_list, func_list, vector<vector<shared_ptr<runtime_variable>>>(), scope_stack);
	r.run_program();
}
