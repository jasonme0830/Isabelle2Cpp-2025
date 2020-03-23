#pragma once

#include <string>
#include <vector>
#include <memory>

namespace hol2cpp {
struct AST {
  virtual ~AST() = 0;
};

struct FunDecl : AST {
    virtual ~FunDecl() = default;
};

struct Type : AST {
    virtual ~Type() = default;
};

struct Expr : AST {
    virtual ~Expr() = default;
};

struct VarExpr : Expr {
    std::string name;

    VarExpr(std::string name) : name(std::move(name)) {}
    virtual ~VarExpr() = default;
};

struct ConsExpr : Expr {
    ConsExpr(std::string constructor,
      std::vector<std::unique_ptr<Expr>> patterns);
    virtual ~ConsExpr() = default;
};

struct Equation : AST {
    Equation(std::unique_ptr<ConsExpr> pattern,
      std::unique_ptr<Expr> expr);
    virtual ~Equation() = default;
};
}
