#include "includes/runtime.hpp"

namespace dharma_vm {

	shared_ptr<runtime_variable> runtime::print(shared_ptr<runtime_variable> rvar) {
		if (rvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (rvar->get_runtime_type_information() == runtime_type_information_list::_boolean) {
			cout << (rvar->get_boolean() ? "true" : "false");
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_decimal)
			cout << rvar->get_decimal();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_dict) {
			if (rvar->get_dict().first.size() != rvar->get_dict().second.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, rvar);
			cout << "{{{ ";
			for (int i = 0; i < rvar->get_dict().first.size(); i++) {
				print(rvar->get_dict().first[i]);
				cout << ": ";
				print(rvar->get_dict().second[i]);
				if (i != rvar->get_dict().first.size() - 1)
					cout << ", ";
			}
			cout << " }}}";
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_list || rvar->get_runtime_type_information() == runtime_type_information_list::_tuple) {
			if (rvar->get_runtime_type_information() == runtime_type_information_list::_list)
				cout << "[[[ ";
			else
				cout << "<<< ";
			for (int i = 0; i < rvar->get_list_tuple().size(); i++) {
				print(rvar->get_list_tuple()[i]);
				if (i != rvar->get_list_tuple().size() - 1)
					cout << ", ";
			}
			if (rvar->get_runtime_type_information() == runtime_type_information_list::_list)
				cout << " ]]]";
			else
				cout << " >>>";
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_int)
			cout << rvar->get_integer();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_nil)
			cout << "Nil";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_func)
			cout << "Function: " << rvar->get_string();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_boolean)
			cout << "[Boolean]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_decimal)
			cout << "[Decimal]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_dict)
			cout << "[Dict]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_func)
			cout << "[Function: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_int)
			cout << "[Integer]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_list)
			cout << "[List]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_nil)
			cout << "[Nil]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_string)
			cout << "[String]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_tuple)
			cout << "[Tuple]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_string)
			cout << rvar->get_string();
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Struct: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Enum: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Enum Child: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Module: " << rvar->get_string() << "]";
		else {
			vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_special_function(instruction_list, nullptr, builtins::builtin__print__);
			bool success = false;
			if (results.size() > 0) {
				for (int j = 0; j < results.size(); j++) {
					pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
					for (int i = 0; i < res.first->get_function().size(); i++) {
						shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { rvar }, res.second);
						if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
						else {
							print(temp);
							success = true;
							break;
						}
					}
					if (success)
						break;
				}
			}
			if (!success) {
				if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Struct: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Module: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Enum: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Enum Child: " << rvar->get_string();
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
			}
		}
		shared_ptr<runtime_variable> ret = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
			vector<shared_ptr<function>>());
		return checked_insertion(ret);
	}

	shared_ptr<runtime_variable> runtime::exit(shared_ptr<runtime_variable> exit_code, shared_ptr<runtime_variable> message) {
		if (exit_code == nullptr || message == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (exit_code->get_runtime_type_information() != runtime_type_information_list::_int)
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, exit_code);
		if (message->get_runtime_type_information() != runtime_type_information_list::_string)
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, message);
		print(message);
		std::exit(exit_code->get_integer());
		shared_ptr<runtime_variable> created_bool = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
			vector<shared_ptr<function>>());
		runtime_temporary_count++;
		return checked_insertion(created_bool);
	}

	shared_ptr<runtime_variable> runtime::add(shared_ptr<runtime_variable> dict, shared_ptr<runtime_variable> key, shared_ptr<runtime_variable> value) {
		if (dict == nullptr || key == nullptr || value == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dict->get_runtime_type_information() == runtime_type_information_list::_dict);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dict);
		if (dict->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dict);
		shared_ptr<runtime_variable> created_dict = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_dict,
			vector<shared_ptr<function>>());
		created_dict->set_dict(dict->get_dict());
		runtime_temporary_count++;
		vector<shared_ptr<runtime_variable>> key_list = created_dict->get_dict().first;
		vector<shared_ptr<runtime_variable>> value_list = created_dict->get_dict().second;
		if (dict->get_dict().first.size() != dict->get_dict().second.size())
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dict);
		if (dict->get_dict().first.size() == 0) {
			key_list.push_back(key);
			value_list.push_back(value);
		}
		else {
			runtime_type_information key_t_inf = created_dict->get_dict().first[0]->get_runtime_type_information();
			if (key_t_inf == key->get_runtime_type_information()) {
				if (key_t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
					if (key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE ||
						key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM || key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD) {
						if (dict->get_dict().first[0]->get_unique_id() == key->get_unique_id());
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
					}
				}
			}
			else if ((key_t_inf == runtime_type_information_list::_int && key->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
				(key_t_inf == runtime_type_information_list::_decimal && key->get_runtime_type_information() == runtime_type_information_list::_int)) {
				if (key_t_inf == runtime_type_information_list::_int) {
					key->set_integer(key->get_decimal());
					key->set_runtime_type_information(runtime_type_information_list::_int);
				}
				else {
					key->set_decimal(key->get_integer());
					key->set_runtime_type_information(runtime_type_information_list::_decimal);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
			for (int i = 0; i < created_dict->get_dict().first.size(); i++) {
				shared_ptr<runtime_variable> k_temp = make_shared<runtime_variable>(key->get_storage_field(), key->get_integer(), key->get_decimal(), key->get_string(), key->get_boolean(),
					key->get_list_tuple(), key->get_dict(), key->get_struct_enum_member_list(), key->get_module_runtime(), key->get_runtime_type_information(), key->get_function());
				k_temp->set_unmodifiable(key->get_unmodifiable());
				k_temp->set_unique_id(key->get_unique_id());
				if ((k_temp == created_dict->get_dict().first[i])->get_boolean())
					report_error_and_terminate_program(runtime_diagnostic_messages::key_already_exists, key);
			}
			key_list.push_back(key);
			runtime_type_information value_t_inf = created_dict->get_dict().second[0]->get_runtime_type_information();
			if (value_t_inf == value->get_runtime_type_information()) {
				if (value_t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
					if (value_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || value_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE ||
						value_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM || value_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD) {
						if (created_dict->get_dict().second[0]->get_unique_id() == value->get_unique_id());
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
					}
				}
			}
			else if ((value_t_inf == runtime_type_information_list::_int && value->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
				(value_t_inf == runtime_type_information_list::_decimal && value->get_runtime_type_information() == runtime_type_information_list::_int)) {
				if (value_t_inf == runtime_type_information_list::_int) {
					value->set_integer(value->get_decimal());
					value->set_runtime_type_information(runtime_type_information_list::_int);
				}
				else {
					value->set_decimal(value->get_integer());
					value->set_runtime_type_information(runtime_type_information_list::_decimal);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
			value_list.push_back(value);
		}
		created_dict->set_dict(make_pair(key_list, value_list));
		return created_dict;
	}

	shared_ptr<runtime_variable> runtime::add(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> element) {
		if (list_string == nullptr || element == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (list_string->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, list_string);
		shared_ptr<runtime_variable> created_list_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), list_string->get_runtime_type_information(),
			vector<shared_ptr<function>>());
		created_list_string->set_list_tuple(list_string->get_list_tuple());
		created_list_string->set_string(list_string->get_string());
		runtime_temporary_count++;
		if (list_string->get_runtime_type_information() == runtime_type_information_list::_list) {
			vector<shared_ptr<runtime_variable>> list = created_list_string->get_list_tuple();
			if (list_string->get_list_tuple().size() == 0)
				list.push_back(element);
			else {
				runtime_type_information t_inf = created_list_string->get_list_tuple()[0]->get_runtime_type_information();
				if (t_inf == element->get_runtime_type_information()) {
					if (t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
						if (t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE ||
							t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM || t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD) {
							if (list_string->get_list_tuple()[0]->get_unique_id() == element->get_unique_id());
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
						}
					}
				}
				else if ((t_inf == runtime_type_information_list::_int && element->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
					(t_inf == runtime_type_information_list::_decimal && element->get_runtime_type_information() == runtime_type_information_list::_int)) {
					if (t_inf == runtime_type_information_list::_int) {
						element->set_integer(element->get_decimal());
						element->set_runtime_type_information(runtime_type_information_list::_int);
					}
					else {
						element->set_decimal(element->get_integer());
						element->set_runtime_type_information(runtime_type_information_list::_decimal);
					}
				}
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
				list.push_back(element);
			}
			created_list_string->set_list_tuple(list);
		}
		else if (list_string->get_runtime_type_information() == runtime_type_information_list::_string) {
			if (element->get_runtime_type_information() == runtime_type_information_list::_string)
				created_list_string->set_string(created_list_string->get_string() + element->get_string());
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, list_string);
		return created_list_string;
	}

	shared_ptr<runtime_variable> runtime::insert(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> pos, shared_ptr<runtime_variable> element) {
		if (list_string == nullptr || pos == nullptr || element == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		shared_ptr<runtime_variable> created_list_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), list_string->get_runtime_type_information(),
			vector<shared_ptr<function>>());
		created_list_string->set_list_tuple(list_string->get_list_tuple());
		created_list_string->set_string(list_string->get_string());
		runtime_temporary_count++;
		if (pos->get_runtime_type_information() != runtime_type_information_list::_int)
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, pos);
		if (list_string->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, list_string);
		if (list_string->get_runtime_type_information() == runtime_type_information_list::_list) {
			vector<shared_ptr<runtime_variable>> list = created_list_string->get_list_tuple();
			if (list.size() == 0)
				report_error_and_terminate_program(runtime_diagnostic_messages::use_the_add_function_to_add_an_element_to_an_empty_list, list_string);
			if (pos->get_integer() < 0 || pos->get_integer() > list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, pos);
			runtime_type_information t_inf = created_list_string->get_list_tuple()[0]->get_runtime_type_information();
			if (t_inf == element->get_runtime_type_information()) {
				if (t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
					if (t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE ||
						t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM || t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD) {
						if (list_string->get_list_tuple()[0]->get_unique_id() == element->get_unique_id());
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
					}
				}
			}
			else if ((t_inf == runtime_type_information_list::_int && element->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
				(t_inf == runtime_type_information_list::_decimal && element->get_runtime_type_information() == runtime_type_information_list::_int)) {
				if (t_inf == runtime_type_information_list::_int) {
					element->set_integer(element->get_decimal());
					element->set_runtime_type_information(runtime_type_information_list::_int);
				}
				else {
					element->set_decimal(element->get_integer());
					element->set_runtime_type_information(runtime_type_information_list::_decimal);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
			list.insert(list.begin() + pos->get_integer(), element);
			created_list_string->set_list_tuple(list);
		}
		else if (list_string->get_runtime_type_information() == runtime_type_information_list::_string) {
			string str = created_list_string->get_string();
			if (element->get_runtime_type_information() != runtime_type_information_list::_string)
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, element);
			if (pos->get_integer() < 0 || pos->get_integer() > str.length())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, pos);
			str.insert(pos->get_integer(), element->get_string());
			created_list_string->set_string(str);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, list_string);
		return created_list_string;
	}

	shared_ptr<runtime_variable> runtime::remove(shared_ptr<runtime_variable> list_string_dict, shared_ptr<runtime_variable> key_index) {
		if (list_string_dict == nullptr || key_index == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		shared_ptr<runtime_variable> created_list_string_dict = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), list_string_dict->get_runtime_type_information(),
			vector<shared_ptr<function>>());
		created_list_string_dict->set_list_tuple(list_string_dict->get_list_tuple());
		created_list_string_dict->set_string(list_string_dict->get_string());
		created_list_string_dict->set_dict(list_string_dict->get_dict());
		runtime_temporary_count++;
		if (list_string_dict->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, list_string_dict);
		if (list_string_dict->get_runtime_type_information() == runtime_type_information_list::_dict) {
			pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> dict = created_list_string_dict->get_dict();
			if (dict.first.size() != dict.second.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, list_string_dict);
			if (dict.first.size() == 0)
				report_error_and_terminate_program(runtime_diagnostic_messages::cannot_remove_from_an_empty_dictionary, list_string_dict);
			runtime_type_information key_t_inf = dict.first[0]->get_runtime_type_information();
			if (key_t_inf == key_index->get_runtime_type_information()) {
				if (key_t_inf.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
					if (key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE ||
						key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM || key_t_inf.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD) {
						if (dict.first[0]->get_unique_id() == key_index->get_unique_id());
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key_index);
					}
				}
			}
			else if ((key_t_inf == runtime_type_information_list::_int && key_index->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
				(key_t_inf == runtime_type_information_list::_decimal && key_index->get_runtime_type_information() == runtime_type_information_list::_int)) {
				if (key_t_inf == runtime_type_information_list::_int) {
					key_index->set_integer(key_index->get_decimal());
					key_index->set_runtime_type_information(runtime_type_information_list::_int);
				}
				else {
					key_index->set_decimal(key_index->get_integer());
					key_index->set_runtime_type_information(runtime_type_information_list::_decimal);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key_index);
			int store = -1;
			for (int i = 0; i < dict.first.size(); i++) {
				shared_ptr<runtime_variable> k_temp = make_shared<runtime_variable>(key_index->get_storage_field(), key_index->get_integer(), key_index->get_decimal(), key_index->get_string(), key_index->get_boolean(),
					key_index->get_list_tuple(), key_index->get_dict(), key_index->get_struct_enum_member_list(), key_index->get_module_runtime(), key_index->get_runtime_type_information(), key_index->get_function());
				k_temp->set_unmodifiable(key_index->get_unmodifiable());
				k_temp->set_unique_id(key_index->get_unique_id());
				if ((k_temp == dict.first[i])->get_boolean()) {
					store = i;
					break;
				}
			}
			if (store == -1);
			else {
				dict.first.erase(dict.first.begin() + store, dict.first.begin() + store + 1);
				dict.second.erase(dict.second.begin() + store, dict.second.begin() + store + 1);
			}
			created_list_string_dict->set_dict(dict);
		}
		else if (list_string_dict->get_runtime_type_information() == runtime_type_information_list::_list) {
			vector<shared_ptr<runtime_variable>> list = created_list_string_dict->get_list_tuple();
			if (key_index->get_runtime_type_information() != runtime_type_information_list::_int)
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key_index);
			if (key_index->get_integer() < 0 || key_index->get_integer() > list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, key_index);
			list.erase(list.begin() + key_index->get_integer(), list.begin() + key_index->get_integer() + 1);
			created_list_string_dict->set_list_tuple(list);
		}
		else if (list_string_dict->get_runtime_type_information() == runtime_type_information_list::_string) {
			string str = created_list_string_dict->get_string();
			if (key_index->get_runtime_type_information() != runtime_type_information_list::_int)
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key_index);
			if (key_index->get_integer() < 0 || key_index->get_integer() > str.length())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, key_index);
			str.erase(key_index->get_integer(), 1);
			created_list_string_dict->set_string(str);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, list_string_dict);
		return created_list_string_dict;
	}
	
	shared_ptr<runtime_variable> runtime::remove(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> start, shared_ptr<runtime_variable> end) {
		shared_ptr<runtime_variable> created_list_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), list_string->get_runtime_type_information(),
			vector<shared_ptr<function>>());
		created_list_string->set_list_tuple(list_string->get_list_tuple());
		created_list_string->set_string(list_string->get_string());
		runtime_temporary_count++;
		if (list_string->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, list_string);
		if (start->get_runtime_type_information() == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
		if (end->get_runtime_type_information() == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
		if (list_string->get_runtime_type_information() == runtime_type_information_list::_list) {
			vector<shared_ptr<runtime_variable>> list = created_list_string->get_list_tuple();
			if (start->get_integer() < 0 || start->get_integer() > list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, start);
			if (end->get_integer() < 0 || end->get_integer() > list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
			if (start->get_integer() > end->get_integer())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
			list.erase(list.begin() + start->get_integer(), list.begin() + end->get_integer());
			created_list_string->set_list_tuple(list);
		}
		else if (list_string->get_runtime_type_information() == runtime_type_information_list::_string) {
			string str = created_list_string->get_string();
			if (start->get_integer() < 0 || start->get_integer() > str.length())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, start);
			if (end->get_integer() < 0 || end->get_integer() > str.length())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
			if (start->get_integer() > end->get_integer())
				report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
			str.erase(start->get_integer(), end->get_integer() - start->get_integer());
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, list_string);
		return created_list_string;
	}

	shared_ptr<runtime_variable> runtime::load_library(shared_ptr<runtime_variable> dll_name) {
		if (dll_name == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dll_name->get_runtime_type_information() == runtime_type_information_list::_string);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dll_name);
		shared_ptr<runtime_variable> created_dll = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, dll_name->get_string(), false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector <vector<shared_ptr<runtime_variable>>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information(runtime_type_kind::TYPE_MODULE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, builtins::builtin_runtime_dll_module_prefix + dll_name->get_string()), vector<shared_ptr<function>>());
		runtime_temporary_count++;
		return created_dll;
	}

	shared_ptr<runtime_variable> runtime::input(shared_ptr<runtime_variable> prompt) {
		if (prompt == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (prompt->get_runtime_type_information() == runtime_type_information_list::_string);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, prompt);
		cout << prompt->get_string();
		string str;
		cin >> str;
		shared_ptr<runtime_variable> ret = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, str, false,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_string, vector<shared_ptr<function>>());
		return ret;
	}

	shared_ptr<runtime_variable> runtime::is_pure(shared_ptr<runtime_variable> rvar) {
		if (rvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		bool pure = false;
		if (rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			pure = true;
		shared_ptr<runtime_variable> ret = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", pure,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
			make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(),
				vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean, vector<shared_ptr<function>>());
		return ret;
	}
}