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
	vec.push_back("func fib_0 n_1");
	vec.push_back("mov r0 [n_1]");
	vec.push_back("mov r1 [$0]");
	vec.push_back("lte r0 r1");
	vec.push_back("bneg r0");
	vec.push_back("jmp r0 .L0");
	vec.push_back("mov r3 [$0]");
	vec.push_back("ret r3");
	vec.push_back("mov r2 [$true]");
	vec.push_back("jmp r2 .L1");
	vec.push_back(".L0");
	vec.push_back(".L1");
	vec.push_back("mov r4 [n_1]");
	vec.push_back("mov r5 [$1]");
	vec.push_back("equ r4 r5");
	vec.push_back("mov r6 [n_1]");
	vec.push_back("mov r7 [$2]");
	vec.push_back("equ r6 r7");
	vec.push_back("lor r4 r6");
	vec.push_back("bneg r4");
	vec.push_back("jmp r4 .L2");
	vec.push_back("mov r9 [$1]");
	vec.push_back("ret r9");
	vec.push_back("mov r8 [$true]");
	vec.push_back("jmp r8 .L3");
	vec.push_back(".L2");
	vec.push_back("mov r11 [n_1]");
	vec.push_back("mov r12 [$2]");
	vec.push_back("sub r11 r12");
	vec.push_back("mov r10 [fib_0:r11]");
	vec.push_back("mov r14 [n_1]");
	vec.push_back("mov r15 [$1]");
	vec.push_back("sub r14 r15");
	vec.push_back("mov r13 [fib_0:r14]");
	vec.push_back("add r10 r13");
	vec.push_back("ret r10");
	vec.push_back(".L3");
	vec.push_back("mov r16 [$0]");
	vec.push_back("ret r16");
	vec.push_back("efunc");
	vec.push_back("mov r19 [$15]");
	vec.push_back("mov r18 [fib_0:r19]");
	vec.push_back("mov r17 [print:r18]");
	shared_ptr<runtime_variable> print_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_print, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
		-1, -1, builtins::builtin_print, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
		make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>()), type_information_list::_func);
	shared_ptr<function> print_function = make_shared<function>(builtins::builtin_print, vector<string>(), vector<string>(), print_variable, true);
	vector<shared_ptr<function>> func_list;
	func_list.push_back(print_function);
	vector<shared_ptr<runtime_variable>> rvar_list;
	rvar_list.push_back(print_variable);
	runtime r(vec, rvar_list, func_list);
	r.run_program();
}
