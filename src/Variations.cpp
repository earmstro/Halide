#include "Variations.h"
#include "Expr.h"
#include "ExprUsesVar.h"
#include "IR.h"
#include "IRVisitor.h"
#include "IRMutator.h"
#include "IROperator.h"
#include "Simplify.h"
#include "StmtToHtml.h"
#include "Substitute.h"


#include <stdio.h>

namespace Halide {
namespace Internal {

using std::map;
using std::string;
using std::vector;
using std::pair;

// Finds the base variables of the code - no loop variables or params
class FindVariableNames : public IRGraphVisitor {
public:
    vector<string> var_names;

    using IRGraphVisitor::visit;

    void visit(const Variable *var) {

        if (std::find(var_names.begin(), var_names.end(), var->name) == 
            var_names.end()) {

            bool is_new_var = true;
    
            // check if this is the base variable or some sort of loop param
            for (uint i = 0; i < var_names.size(); i++) {
                if (var_names[i].find(var->name) != std::string::npos) {
                    var_names.erase(var_names.begin() + i);
                    i--;
                } else if ((var->name).find(var_names[i]) != std::string::npos) {
                    is_new_var = false;
                    break;
                }
            }

            if (is_new_var) {
                var_names.push_back(var->name);
            }
            
        }
    }
};

class MutateOperations : public IRMutator {
    vector<string> base_vars;
    vector<Stmt> generated_statements;

    using IRMutator::visit;

    void visit(const Add *add) {

        Expr a = mutate(add->a); // Recursively mutating stuff
        Expr b = mutate(add->b);

        // First, determine if this add is on base variables
        bool a_uses_var = false;
        bool b_uses_var = false;

        for (string &var : base_vars) {
            a_uses_var |= expr_uses_var(a, var);
            b_uses_var |= expr_uses_var(b, var);
        }
        
        // Replace the add with another operation
        if (a_uses_var && b_uses_var) {
            std::cout << "replacing add...\n";
            expr = Sub::make(a, b);
            generated_statements.push_back(stmt);
        } else {
            IRMutator::visit(add);
        }
    }

public:
    void set_base_vars(vector<string> vars) {
        base_vars = vars;
    }

    vector<Stmt> get_statements() {
        return generated_statements;
    }

};

Stmt generate_variants(Stmt s) {    
    FindVariableNames finder;
    s.accept(&finder);

    MutateOperations op_mutator = MutateOperations();
    op_mutator.set_base_vars(finder.var_names);
    op_mutator.mutate(s);

    vector<Stmt> variants = op_mutator.get_statements();
    std::cout << "Length: " << variants.size() << "\n";
    return variants[0];
}

}
}