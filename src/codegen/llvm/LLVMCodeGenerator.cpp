#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
LLVMCodeGenerator::LLVMCodeGenerator(std::ostream &out)
    : common::CodeGenerator::CodeGenerator(out),
      Builder(getGlobalContext()),
      Module("SpplModule", getGlobalContext()) {}

void LLVMCodeGenerator::visit(common::Program &node) {
    for (auto &func : node.funcs) {
        func->accept(*this);
    }

    Module.dump();
}

void LLVMCodeGenerator::visit(common::Function &node) {
    // Input/output types
    //std::vector<Type *> Doubles(node.patterns.size(),
    //                            Type::getDoubleTy(getGlobalContext()));

	std::vector<Type *> func_types;
	for (auto& type: node.types) {
		switch (type->type) {
			case common::Types::FLOAT:
                func_types.push_back(Type::getFloatTy(getGlobalContext()));
                break;

            default:
                throw "Not supported";
		}
	}

    // Setup function input/out types
    auto func_type = FunctionType::get(Type::getDoubleTy(getGlobalContext()),
                                       func_types, false);

    // Create function
    cur_func = Function::Create(func_type, Function::ExternalLinkage, node.id, &Module);

    // Setup function arguments
    unsigned i = 0;
    for (auto &arg : cur_func->args()) arg.setName("x" + to_string(i++));

    for (auto &_case : node.cases) {
        _case->accept(*this);
    }
}

void LLVMCodeGenerator::visit(common::Case &node) {
    // Create function block
    BasicBlock *block =
        BasicBlock::Create(getGlobalContext(), "entry", cur_func);
    Builder.SetInsertPoint(block);

    ContextValues.clear();
    for (auto &arg : cur_func->args()) ContextValues[arg.getName()] = &arg;

    node.expr->accept(*this);

    cur_val = Builder.CreateRet(cur_val);

    // cur_val->dump();

    verifyFunction(*cur_func);
}

void LLVMCodeGenerator::visit(common::Add &node) {
    node.left->accept(*this);
    auto left = cur_val;
    node.right->accept(*this);
    auto right = cur_val;

    Builder.CreateFAdd(left, right, "addtmp");
}

void LLVMCodeGenerator::visit(common::Float &node) {
    cur_val = ConstantFP::get(getGlobalContext(), APFloat(node.value));
}

void LLVMCodeGenerator::visit(common::Call &node) {

    //switch (node.node_type->type)
    //{
    //    case common::Types::
    //}

    node.callee->accept(*this);
    auto callee = cur_val;

    std::vector<Value *> args;
    for (auto& arg: node.exprs) {
        arg->accept(*this);
        args.push_back(cur_val);
    }

    //Function *call = Module.getFunction(node.);

    cur_val = Builder.CreateCall(callee, args, "calltmp");
}

    void  LLVMCodeGenerator::visit(common::Id &node)
    {
        cur_val = Module.getFunction(node.id);
    }
}
