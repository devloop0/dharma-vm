#ifndef DHARMA_VM_VM_HPP
#define DHARMA_VM_VM_HPP

#include "includes/command_line_parser.hpp"
#include "includes/file.hpp"
#include "includes/unified.hpp"

using namespace unified_includes;

namespace dharma_vm {

	class runtime;
	class runtime_variable;
	class function;
	class storage_field;
	enum storage_field_kind;
	class runtime_type_information_list;

	class vm {
		file source_file;
		shared_ptr<runtime> program;
		public:
			vm(file sf);
			~vm();
			shared_ptr<runtime_variable> run();
			file get_source_file();
			shared_ptr<runtime> get_program();
	};

	class dharma {
		command_line_parser cl_parser;
		vector<command_line_argument> command_line_argument_list;
		vector<command_line_argument_result> command_line_argument_result_list;
		shared_ptr<vm> virtual_machine;
		public:
			dharma(int argc, char* argv[]);
			~dharma();
			shared_ptr<vm> get_virtual_machine();
	};
}

#endif