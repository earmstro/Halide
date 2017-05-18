#ifndef HALIDE_VARIATIONS_H
#define HALIDE_VARIATIONS_H

#include "IR.h"

namespace Halide {

namespace Internal {

Stmt generate_variants(Stmt s);

}
}

#endif