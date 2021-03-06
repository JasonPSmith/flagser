#pragma once

#include <cstring>
#include <iostream>

#include "../definitions.h"

#include "../argparser.h"
#include "barcode.h"
#include "base.h"
#include "betti.h"

#ifdef WITH_HDF5
#include "barcode_hdf5.h"
#endif

bool has_zero_filtration_and_no_explicit_output(const named_arguments_t& named_arguments) {
	return strlen(get_argument_or_default(named_arguments, "out-format", "")) == 0 &&
	       std::string(get_argument_or_default(named_arguments, "filtration", "zero")) == "zero";
}

template <typename Complex> output_t<Complex>* get_output(const named_arguments_t& named_arguments) {
	std::string output_name = "barcode";
	auto it = named_arguments.find("out-format");
	if (it != named_arguments.end()) { output_name = it->second; }

	if (output_name == "betti" || has_zero_filtration_and_no_explicit_output(named_arguments))
		return new betti_output_t<Complex>(named_arguments);
	if (output_name == "barcode") return new barcode_output_t<Complex>(named_arguments);

#ifdef WITH_HDF5
	if (output_name == "barcode:hdf5") return new barcode_hdf5_output_t<Complex>(named_arguments);
#endif

	std::cerr << "The output format \"" << output_name << "\" could not be found." << std::endl;
	exit(1);
}

std::vector<std::string> available_output_formats = {"barcode", "betti"
#ifdef WITH_HDF5
                                                     ,
                                                     "barcode:hdf5"
#endif
};