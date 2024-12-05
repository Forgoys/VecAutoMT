#include "preprocessor_config.h"
#include "array_matcher.h"
#include "code_modifier.h"
#include "util.h"
#include <iostream>

// preprocessor_config.cpp
void PreprocessorConfig::configure(clang::CompilerInstance &CI)
{
    clang::Preprocessor &PP = CI.getPreprocessor();

    if (getLocateMode() || getRestoreMode()) {
        std::string predefs = PP.getPredefines();
        predefs += "\n"
                "#ifdef global\n"
                "#undef global\n"
                "#endif\n"
                "#define global\n"

                "#ifdef share\n"
                "#undef share\n"
                "#endif\n"
                "#define share\n"

                "#ifdef gsm\n"
                "#undef gsm\n"
                "#endif\n"
                "#define gsm\n"

                "#ifdef __attribute__\n"
                "#undef __attribute__\n"
                "#endif\n"
                "#define __attribute__(x)\n"

                "#ifdef section\n"
                "#undef section\n"
                "#endif\n"
                "#define section(x)\n";

        PP.setPredefines(predefs);
    }
}
