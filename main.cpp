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
	vec.push_back("func fac n ");
vec.push_back("mov r0 [n]");
vec.push_back("mov r1 [$0]");
vec.push_back("lt r0 r1");
vec.push_back("bneg r0");
vec.push_back("jmp r0 .L0");
vec.push_back("scope");
vec.push_back("mov r3 [$0]");
vec.push_back("ret r3");
vec.push_back("escope");
vec.push_back("mov r2 [$true]");
vec.push_back("jmp r2 .L1");
vec.push_back(".L0");
vec.push_back("mov r4 [n]");
vec.push_back("mov r5 [$0]");
vec.push_back("equ r4 r5");
vec.push_back("mov r6 [n]");
vec.push_back("mov r7 [$1]");
vec.push_back("equ r6 r7");
vec.push_back("lor r4 r6");
vec.push_back("bneg r4");
vec.push_back("jmp r4 .L1");
vec.push_back("scope");
vec.push_back("mov r8 [$1]");
vec.push_back("ret r8");
vec.push_back("escope");
vec.push_back(".L1");
vec.push_back("mov r9 [n]");
vec.push_back("mov r11 [n]");
vec.push_back("mov r12 [$1]");
vec.push_back("sub r11 r12");
vec.push_back("mov r10 [fac:r11]");
vec.push_back("mul r9 r10");
vec.push_back("ret r9");
vec.push_back("efunc");
vec.push_back("func fac one two ");
vec.push_back("mov r14 [one]");
vec.push_back("mov r15 [two]");
vec.push_back("mul r14 r15");
vec.push_back("mov r13 [^@fac:r14]");
vec.push_back("mov r17 [one]");
vec.push_back("mov r18 [two]");
vec.push_back("add r17 r18");
vec.push_back("mov r16 [^@fac:r17]");
vec.push_back("add r13 r16");
vec.push_back("ret r13");
vec.push_back("efunc");
vec.push_back("module mod");
vec.push_back("func fac n ");
vec.push_back("mov r19 [n]");
vec.push_back("mov r20 [$0]");
vec.push_back("lte r19 r20");
vec.push_back("bneg r19");
vec.push_back("jmp r19 .L2");
vec.push_back("scope");
vec.push_back("mov r22 [$0]");
vec.push_back("ret r22");
vec.push_back("escope");
vec.push_back("mov r21 [$true]");
vec.push_back("jmp r21 .L3");
vec.push_back(".L2");
vec.push_back("scope");
vec.push_back("escope");
vec.push_back(".L3");
vec.push_back("mov r24 [n]");
vec.push_back("mov r26 [n]");
vec.push_back("mov r27 [$1]");
vec.push_back("sub r26 r27");
vec.push_back("mov r25 [fac:r26]");
vec.push_back("add r24 r25");
vec.push_back("mov r29 [n]");
vec.push_back("mov r28 [^@fac:r29]");
vec.push_back("add r24 r28");
vec.push_back("mov r31 [n]");
vec.push_back("mov r32 [$1]");
vec.push_back("sub r31 r32");
vec.push_back("mov r30 [^@fac:r31]");
vec.push_back("add r24 r30");
vec.push_back("mov r34 [n]");
vec.push_back("mov r35 [n]");
vec.push_back("mov r33 [^@fac:r34,r35]");
vec.push_back("add r24 r33");
vec.push_back("ret r24");
vec.push_back("efunc");
vec.push_back("emodule");
vec.push_back("mov r38 [$4]");
vec.push_back("mov r37 [fac:r38]");
vec.push_back("mov r39 [$' ']");
vec.push_back("mov r41 [$4]");
vec.push_back("mov r40 [mod@fac:r41]");
vec.push_back("mov r42 [$'\n']");
vec.push_back("mov r36 [print:r37,r39,r40,r42]");
	shared_ptr<function> print_function = make_shared<function>(builtins::builtin_print, vector<string>(), vector<string>(), true, false);
	shared_ptr<function> exit_function = make_shared<function>(builtins::builtin_exit, vector<string>(), vector<string> { "", "" }, false, false);
	shared_ptr<runtime_variable> print_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_print, storage_field_kind::STORAGE_FIELD_IDENTIFIER, true),
		-1, -1, builtins::builtin_print, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
		make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
			vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), type_information_list::_func, vector<shared_ptr<function>> { print_function });
	shared_ptr<runtime_variable> exit_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_exit, storage_field_kind::STORAGE_FIELD_IDENTIFIER, true),
		-1, -1, builtins::builtin_exit, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
		make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
			vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), type_information_list::_func, vector<shared_ptr<function>> { exit_function });
	vector<shared_ptr<function>> func_list;
	func_list.push_back(print_function);
	func_list.push_back(exit_function);
	vector<vector<shared_ptr<function>>> function_list{ func_list };
	vector<shared_ptr<runtime_variable>> rvar_list;
	rvar_list.push_back(print_variable);
	rvar_list.push_back(exit_variable);
	vector<vector<shared_ptr<runtime_variable>>> scope_stack;
	runtime r(vec, rvar_list, vector<vector<shared_ptr<runtime_variable>>>(), scope_stack, vector<vector<shared_ptr<runtime_variable>>>(),
		vector<shared_ptr<runtime_variable>>());
	r.run_program();
}
