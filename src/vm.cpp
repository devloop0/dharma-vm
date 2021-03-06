#include "includes/vm.hpp"
#include "includes/runtime.hpp"

using namespace dharma_vm;
using namespace unified_includes;

namespace dharma_vm {

	vm::vm(file sf) : source_file(sf) {
		shared_ptr<function> print_function = make_shared<function>(builtins::builtin_print, vector<string>(), vector<string>(), true, false, true);
		shared_ptr<function> exit_function = make_shared<function>(builtins::builtin_exit, vector<string>(), vector<string> { "", "" }, false, false, true);
		shared_ptr<function> dict_add_function = make_shared<function>(builtins::builtin_add, vector<string>(), vector<string> { "", "", "" }, false, false, true);
		shared_ptr<function> list_string_add_function = make_shared<function>(builtins::builtin_add, vector<string>(), vector<string> { "", "" }, false, false, true);
		shared_ptr<function> insert_function = make_shared<function>(builtins::builtin_insert, vector<string>(), vector<string> { "", "", "" }, false, false, true);
		shared_ptr<function> list_string_dict_remove_function = make_shared<function>(builtins::builtin_remove, vector<string>(), vector<string> { "", "" }, false, false, true);
		shared_ptr<function> list_string_remove_function = make_shared<function>(builtins::builtin_remove, vector<string>(), vector<string> { "", "", "" }, false, false, true);
		shared_ptr<function> load_library_function = make_shared<function>(builtins::builtin_load_library, vector<string>(), vector<string> { "" }, false, false, true);
		shared_ptr<function> input_function = make_shared<function>(builtins::builtin_input, vector<string>(), vector<string> { "" }, false, false, true);
		shared_ptr<function> is_pure_function = make_shared<function>(builtins::builtin_is_pure, vector<string>(), vector<string> { "" }, false, false, true);
		shared_ptr<runtime_variable> print_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_print, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_print, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { print_function });
		shared_ptr<runtime_variable> exit_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_exit, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_exit, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { exit_function });
		shared_ptr<runtime_variable> add_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_add, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_add, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { dict_add_function, list_string_add_function });
		shared_ptr<runtime_variable> insert_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_insert, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_insert, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { insert_function });
		shared_ptr<runtime_variable> remove_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_remove, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_remove, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { list_string_dict_remove_function, list_string_remove_function });
		shared_ptr<runtime_variable> load_library_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_load_library, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_load_library, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { load_library_function });
		shared_ptr<runtime_variable> input_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_input, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_input, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { input_function });
		shared_ptr<runtime_variable> is_pure_variable = make_shared<runtime_variable>(storage_field(-1, builtins::builtin_is_pure, storage_field_kind::STORAGE_FIELD_IDENTIFIER),
			-1, -1, builtins::builtin_is_pure, false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { is_pure_function });
		vector<shared_ptr<runtime_variable>> rvar_list{ print_variable, exit_variable, add_variable, insert_variable, remove_variable, load_library_variable, input_variable, is_pure_variable };
		vector<vector<shared_ptr<runtime_variable>>> scope_stack;
		program = make_shared<runtime>(sf.get_source(), rvar_list, vector<vector<shared_ptr<runtime_variable>>>(), scope_stack, vector<vector<shared_ptr<runtime_variable>>>(),
			vector<shared_ptr<runtime_variable>>());
	}

	vm::~vm() {

	}

	shared_ptr<runtime> vm::get_program() {
		return program;
	}
	
	file vm::get_source_file() {
		return source_file;
	}

	shared_ptr<runtime_variable> vm::run() {
		return program->run_program();
	}

	dharma::dharma(int argc, char* argv[]) : cl_parser("", "", {}) {
		command_line_argument fn_cla(argument_kind::ANY_ARGUMENT, "", "", "File Name",
			"This argument should contain the name of the file to run.", true, true);
		command_line_argument ai_cla(argument_kind::VALUE_ARGUMENT, "ai", "add-import", "Add Import",
			"This argument should contain the import you want to add (generally for operator overloading support).", false, false);
		command_line_argument_list = { fn_cla, ai_cla };
		cl_parser = command_line_parser("dharma -- the reference virtual machine implementation to run the karma programming language.",
			"Use this command to run karma source files compiled with the 'karmac' command.", command_line_argument_list);
		command_line_argument_result_list = cl_parser.parse(argc, argv);
		bool fn = false, ai = false;
		string file_name = "";
		vector<string> import_list;
		for (int i = 0; i < command_line_argument_result_list.size(); i++) {
			if (command_line_argument_result_list[i].get_present()) {
				if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "File Name") {
					fn = true;
					file_name = command_line_argument_result_list[i].get_argument();
				}
				else if (command_line_argument_result_list[i].get_command_line_argument().get_reference_name() == "Add Import") {
					ai = true;
					if (command_line_argument_result_list[i].get_argument().length() != 0)
						import_list = vector<string>{ command_line_argument_result_list[i].get_argument() };
					else
						import_list = command_line_argument_result_list[i].get_argument_list();
				}
			}
		}
		if (!fn) {
			cerr << "Expected a file to run.\n";
			exit(1);
		}
		file source(file_name);
		if (source.exists() && source.is_file());
		else {
			cerr << "File not found: " << source.get_full_name() << '\n';
			exit(1);
		}
		if (source.get_extension() != dharma_vm_source_file_extension) {
			cerr << "This source file needs to have the extension '.kac:' " << file_name << '\n';
			exit(1);
		}
		if (ai) {
			vector<string> src = source.get_source();
			vector<string> import_list_final;
			for (int i = 0; i < import_list.size(); i++)
				import_list_final.push_back("imprt " + import_list[i] + " __temp_import_generated__" + to_string(i));
			src.insert(src.begin(), import_list_final.begin(), import_list_final.end());
			source.write(src);
		}
		virtual_machine = make_shared<vm>(source);
		virtual_machine->run();
	}

	dharma::~dharma() {

	}

	shared_ptr<vm> dharma::get_virtual_machine() {
		return virtual_machine;
	}
}