#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>

#include <memory>

class ImplicitCastVisitor : public clang::RecursiveASTVisitor<ImplicitCastVisitor>
{
  public:
    explicit ImplicitCastVisitor(clang::ASTContext* Context)
        : _Context(Context)
    {}

    bool VisitImplicitCastExpr(clang::ImplicitCastExpr* expression)
    {
      if (_Context->getSourceManager().isInMainFile(expression->getExprLoc()))
      {
        llvm::WithColor::error()
            << expression->getExprLoc().printToString(_Context->getSourceManager())
            << '\n';
        expression->printPretty(llvm::WithColor::error(),
                                nullptr,
                                clang::PrintingPolicy(_Context->getLangOpts()));
        llvm::outs() << '\n';
        expression->dumpColor();
        llvm::outs() << '\n';
      }
      return true;
    }

  private:
    clang::ASTContext* _Context;
};

class ImplicitCastConsumer : public clang::ASTConsumer
{
  public:
    explicit ImplicitCastConsumer(clang::ASTContext* Context)
        : Visitor(Context)
    {}

    void HandleTranslationUnit(clang::ASTContext& Context) override
    {
      Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

  private:
    ImplicitCastVisitor Visitor;
};

class ImplicitCastAction : public clang::ASTFrontendAction
{
  public:
    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance& Compiler,
                      llvm::StringRef InFile) override
    {
      llvm::WithColor::note() << InFile << '\n';
      return std::make_unique<ImplicitCastConsumer>(&Compiler.getASTContext());
    }
};

static llvm::cl::OptionCategory MyToolCategory("find-implicit-cast Options");

static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

static llvm::cl::extrahelp MoreHelp("Nitram Kavid\nhttps://github.com/nkavid\n\n");

class OptionsParser : public clang::tooling::CommonOptionsParser
{
  public:
    OptionsParser(int argc, const char** argv)
        : clang::tooling::CommonOptionsParser(argc, argv, MyToolCategory)
    {}
};

int main(int argc, const char** argv)
{
  auto optionsParser = OptionsParser(argc, argv);
  clang::tooling::ClangTool Tool(optionsParser.getCompilations(),
                                 optionsParser.getSourcePathList());

  Tool.run(clang::tooling::newFrontendActionFactory<ImplicitCastAction>().get());
}
