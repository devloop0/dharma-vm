#include "includes/runtime.hpp"
#include "includes/vm.hpp"
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
	dharma(argc, argv);
}
