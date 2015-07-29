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
	vec.push_back("func print_mul_table_0 row_limit_1 column_limit_1 ");
vec.push_back("mov r0 [row_limit_1]");
vec.push_back("mov r1 [$0]");
vec.push_back("lt r0 r1");
vec.push_back("mov r2 [column_limit_1]");
vec.push_back("mov r3 [$0]");
vec.push_back("lt r2 r3");
vec.push_back("lor r0 r2");
vec.push_back("bneg r0");
vec.push_back("jmp r0 .L0");
vec.push_back("scope");
vec.push_back("mov r5 [$false]");
vec.push_back("ret r5");
vec.push_back("escope");
vec.push_back("mov r4 [$true]");
vec.push_back("jmp r4 .L1");
vec.push_back(".L0");
vec.push_back("scope");
vec.push_back("escope");
vec.push_back(".L1");
vec.push_back("mov r7 [$0]");
vec.push_back("dmov [i_3] r7");
vec.push_back(".L2");
vec.push_back("scope");
vec.push_back("mov r8 [i_3]");
vec.push_back("mov r9 [row_limit_1]");
vec.push_back("lte r8 r9");
vec.push_back("bneg r8");
vec.push_back("jmp r8 .L3");
vec.push_back("lambda __temp__0 i_5 j_5 ");
vec.push_back("mov r11 [i_5]");
vec.push_back("mov r12 [$' * ']");
vec.push_back("mov r13 [j_5]");
vec.push_back("mov r14 [$' = ']");
vec.push_back("mov r15 [i_5]");
vec.push_back("mov r16 [j_5]");
vec.push_back("mul r15 r16");
vec.push_back("mov r17 [$'\n']");
vec.push_back("mov r10 [print:r11,r12,r13,r14,r15,r17]");
vec.push_back("elambda");
vec.push_back("mov r10 [__temp__0]");
vec.push_back("dmov [call_print_6] r10");
vec.push_back("mov r18 [$0]");
vec.push_back("dmov [j_6] r18");
vec.push_back(".L4");
vec.push_back("scope");
vec.push_back("mov r19 [j_6]");
vec.push_back("mov r20 [column_limit_1]");
vec.push_back("lte r19 r20");
vec.push_back("bneg r19");
vec.push_back("jmp r19 .L5");
vec.push_back("mov r22 [i_3]");
vec.push_back("mov r23 [j_6]");
vec.push_back("mov r21 [call_print_6:r22,r23]");
vec.push_back("mov r24 [j_6]");
vec.push_back("inc [j_6]");
vec.push_back("escope");
vec.push_back("mov r25 [$true]");
vec.push_back("jmp r25 .L4");
vec.push_back(".L5");
vec.push_back("mov r26 [i_3]");
vec.push_back("mov r27 [$10]");
vec.push_back("equ r26 r27");
vec.push_back("bneg r26");
vec.push_back("jmp r26 .L6");
vec.push_back("scope");
vec.push_back("cont .L2");
vec.push_back("escope");
vec.push_back("mov r28 [$true]");
vec.push_back("jmp r28 .L7");
vec.push_back(".L6");
vec.push_back("scope");
vec.push_back("escope");
vec.push_back(".L7");
vec.push_back("mov r30 [i_3]");
vec.push_back("inc [i_3]");
vec.push_back("escope");
vec.push_back("mov r31 [$true]");
vec.push_back("jmp r31 .L2");
vec.push_back(".L3");
vec.push_back("efunc");
vec.push_back("mov r33 [$12]");
vec.push_back("mov r34 [$12]");
vec.push_back("mov r32 [print_mul_table_0:r33,r34]");
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
