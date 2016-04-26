#include "Driver.h"
#include "Interpreter.h"
#include "Node.h"
#include "Parser.h"
#include "Visitor.h"

#include <vector>

namespace profiler {
class Profiler : public Visitor {
  public:
    Profiler(Program &node);

    void instruction_time_profile(int runs);
    void user_time_profile(int runs);

    void visit(Program &node);
    void visit(Function &node);
    void visit(Case &node);
    void visit(Or &node);
    void visit(And &node);
    void visit(Equal &node);
    void visit(NotEqual &node);
    void visit(Lesser &node);
    void visit(Greater &node);
    void visit(LesserEq &node);
    void visit(GreaterEq &node);
    void visit(Add &node);
    void visit(Sub &node);
    void visit(Mul &node);
    void visit(Div &node);
    void visit(Mod &node);
    void visit(ListAdd &node);
    void visit(Par &node);
    void visit(Not &node);
    void visit(Int &node);
    void visit(Float &node);
    void visit(Bool &node);
    void visit(Char &node);
    void visit(String &node);
    void visit(ListPattern &node);
    void visit(TuplePattern &node);
    void visit(ListSplit &node);
    void visit(List &node);
    void visit(Tuple &node);
    void visit(Id &node);
    void visit(Call &node);
    void visit(Type &node);

  private:
    enum ProfilerMode {
        INSTRUCTION_TIME, // count instructions
        USER_TIME,        // measures run time
        DEPENDENCY_CHECK  // calculate the function deps
    };

    // indicates which mode the profiler is in
    ProfilerMode mode;

    parser::Driver driver;

    int runs;
    vector<int> time_units;
    Program *node;
    Function curr_fun;
    int case_num;
};
}
