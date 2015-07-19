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
	vec.push_back("struc swappable_0 vals");
vec.push_back("tupl r0");
vec.push_back("mov [__temp__0] r0");
vec.push_back("mov r0 [__temp__0]");
vec.push_back("tyof r0");
vec.push_back("mov [vals] r0");
vec.push_back("estruc");
vec.push_back("func get0_0 swap_1 ");
vec.push_back("mov r3 [$0]");
vec.push_back("mov r1 [swap_1@vals|r3]");
vec.push_back("ret r1");
vec.push_back("efunc");
vec.push_back("func get1_0 swap_3 ");
vec.push_back("mov r6 [$1]");
vec.push_back("mov r4 [swap_3@vals|r6]");
vec.push_back("ret r4");
vec.push_back("efunc");
vec.push_back("func create_swappable_0 val1_5 val2_5 ");
vec.push_back("mov r7 [swappable_0]");
vec.push_back("mov [swap_5] r7");
vec.push_back("mov r8 [swap_5@vals]");
vec.push_back("mov r10 [val1_5]");
vec.push_back("mov r11 [val2_5]");
vec.push_back("tupl r9 r10 r11");
vec.push_back("mov [__temp__1] r9");
vec.push_back("mov r9 [__temp__1]");
vec.push_back("mov [swap_5@vals] r9");
vec.push_back("mov r12 [swap_5]");
vec.push_back("ret r12");
vec.push_back("efunc");
vec.push_back("func swap_0 s_7 ");
vec.push_back("mov r15 [s_7]");
vec.push_back("mov r14 [get1_0:r15]");
vec.push_back("mov r17 [s_7]");
vec.push_back("mov r16 [get0_0:r17]");
vec.push_back("mov r13 [create_swappable_0:r14,r16]");
vec.push_back("ret r13");
vec.push_back("efunc");
vec.push_back("mov r18 [$0]");
vec.push_back("mov [x_8] r18");
vec.push_back("mov r19 [$1]");
vec.push_back("mov [y_8] r19");
vec.push_back("mov r21 [$'x: ']");
vec.push_back("mov r22 [x_8]");
vec.push_back("mov r23 [$' y: ']");
vec.push_back("mov r24 [y_8]");
vec.push_back("mov r25 [$'\n']");
vec.push_back("mov r20 [print:r21,r22,r23,r24,r25]");
vec.push_back("mov r27 [x_8]");
vec.push_back("mov r28 [y_8]");
vec.push_back("mov r26 [create_swappable_0:r27,r28]");
vec.push_back("mov [temp_8] r26");
vec.push_back("mov r29 [temp_8]");
vec.push_back("mov r31 [temp_8]");
vec.push_back("mov r30 [swap_0:r31]");
vec.push_back("mov [temp_8] r30");
vec.push_back("mov r32 [x_8]");
vec.push_back("mov r34 [temp_8]");
vec.push_back("mov r33 [get0_0:r34]");
vec.push_back("mov [x_8] r33");
vec.push_back("mov r35 [y_8]");
vec.push_back("mov r37 [temp_8]");
vec.push_back("mov r36 [get1_0:r37]");
vec.push_back("mov [y_8] r36");
vec.push_back("mov r39 [$'x: ']");
vec.push_back("mov r40 [x_8]");
vec.push_back("mov r41 [$' y: ']");
vec.push_back("mov r42 [y_8]");
vec.push_back("mov r43 [$'\n']");
vec.push_back("mov r38 [print:r39,r40,r41,r42,r43]");
	shared_ptr<runtime_variable> print_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_print, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
		-1, -1, builtins::builtin_print, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
		make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>(), vector<vector<shared_ptr<runtime_variable>>>()), type_information_list::_func);
	shared_ptr<function> print_function = make_shared<function>(builtins::builtin_print, vector<string>(), vector<string>(), print_variable, true);
	vector<shared_ptr<function>> func_list;
	func_list.push_back(print_function);
	vector<shared_ptr<runtime_variable>> rvar_list;
	rvar_list.push_back(print_variable);
	runtime r(vec, rvar_list, func_list, vector<vector<shared_ptr<runtime_variable>>>());
	r.run_program();
}
