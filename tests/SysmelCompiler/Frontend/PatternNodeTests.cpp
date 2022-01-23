#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/LanguageSupport.hpp"
#include "Environment/ASTSourcePosition.hpp"

#include "UnitTest++/UnitTest++.h"
#include <iostream>

using namespace Sysmel::Environment;

static AnyValuePtr evaluateString(const std::string &source)
{
    return RuntimeContext::getActive()->getSysmelLanguageSupport()->evaluateSourceStringNamed(source, "unit-test");
}

bool checkStringEvaluationASTIsSameTo(const std::string &source, const std::string &expectedSSA)
{
    auto sexpression = stripSExpressionFromPositions(evaluateString(source)->asSExpression());
    auto sexpressionPretty = sexpressionToPrettyString(sexpression);
    auto sexpressionCanonical = sexpressionCanonicalizeString(sexpressionPretty);
    auto expectedCanonical = sexpressionCanonicalizeString(expectedSSA);
    if(expectedCanonical == sexpressionCanonical)
        return true;

    std::cerr << "Expected AST:\n" << expectedSSA << "\nGotten AST:\n" << sexpressionPretty << std::endl;
    //std::cerr << "Expected SSA:\n" << expectedCanonical << "\nGotten SSA:\n" << sexpressionCanonical << std::endl;
    return false;
}

SUITE(PatternMatching)
{
    TEST(ParseAsPatternNode)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                // Value
                CHECK(checkStringEvaluationASTIsSameTo("`'(1) parseAsPatternNode",
R"(
(valuePattern
  (literal 1 #LiteralPositiveInteger))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'someValue parseAsPatternNode",
R"(
(valuePattern
  (identifier nil #someValue nil))
)"));

                // Any value
                CHECK(checkStringEvaluationASTIsSameTo("`'_ parseAsPatternNode", "(anyValuePattern)"));


                // Range
                CHECK(checkStringEvaluationASTIsSameTo("`'(0 to: 3) parseAsPatternNode",
R"(
(rangePattern (valuePattern
  (literal 0 #LiteralPositiveInteger)) (valuePattern
  (literal 3 #LiteralPositiveInteger)) true)
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(0 until: 3) parseAsPatternNode",
R"(
(rangePattern (valuePattern
  (literal 0 #LiteralPositiveInteger)) (valuePattern
  (literal 3 #LiteralPositiveInteger)) false)
)"));

                // Alternatives
                CHECK(checkStringEvaluationASTIsSameTo("`'(a|b) parseAsPatternNode",
R"(
(alternativesPattern (valuePattern
  (identifier nil #a nil)) (valuePattern
  (identifier nil #b nil)))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(a|b|c) parseAsPatternNode",
R"(
(alternativesPattern (valuePattern
  (identifier nil #a nil)) (valuePattern
  (identifier nil #b nil)) (valuePattern
  (identifier nil #c nil)))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(a|b|c|d) parseAsPatternNode",
R"(
(alternativesPattern (valuePattern
  (identifier nil #a nil)) (valuePattern
  (identifier nil #b nil)) (valuePattern
  (identifier nil #c nil)) (valuePattern
  (identifier nil #d nil)))
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'((a|b)|(c|d)) parseAsPatternNode",
R"(
(alternativesPattern (valuePattern
  (identifier nil #a nil)) (valuePattern
  (identifier nil #b nil)) (valuePattern
  (identifier nil #c nil)) (valuePattern
  (identifier nil #d nil)))
)"));
                // Predicated
                CHECK(checkStringEvaluationASTIsSameTo("`'(a when: a > 2) parseAsPatternNode",
R"(
(predicatedPattern
  (valuePattern
    (identifier nil #a nil))
  (send nil
    (literal #> #LiteralSymbol)
    (identifier nil #a nil)
    ((literal 2 #LiteralPositiveInteger))))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(when: a > 2) parseAsPatternNode",
R"(
(predicatedPattern
  (anyValuePattern)
  (send nil
    (literal #> #LiteralSymbol)
    (identifier nil #a nil)
    ((literal 2 #LiteralPositiveInteger))))
)"));

                // Binding
                CHECK(checkStringEvaluationASTIsSameTo("`'(let x) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  nil
  nil)
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(let x type: SomeType) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  (valuePattern
    (identifier nil #SomeType nil))
  nil)
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(let x type: SomeType := 5) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  (valuePattern
    (identifier nil #SomeType nil))
  (valuePattern
    (literal 5 #LiteralPositiveInteger)))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(let x := 5) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  nil
  (valuePattern
    (literal 5 #LiteralPositiveInteger)))
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(x type: SomeType := 5) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  (valuePattern
    (identifier nil #SomeType nil))
  (valuePattern
    (literal 5 #LiteralPositiveInteger)))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(x := 5) parseAsPatternNode",
R"(
(bindingPattern
  (literal #x #LiteralSymbol)
  nil
  (valuePattern
    (literal 5 #LiteralPositiveInteger)))
)"));

                // Sequence
                CHECK(checkStringEvaluationASTIsSameTo("`'() parseAsPatternNode",
R"(
(sequencePattern nil)
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(1, 2) parseAsPatternNode",
R"(
(sequencePattern nil (valuePattern
  (literal 1 #LiteralPositiveInteger)) (valuePattern
  (literal 2 #LiteralPositiveInteger)))
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(SomeType()) parseAsPatternNode",
R"(
(sequencePattern
  (identifier nil #SomeType nil))
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(SomeType(1)) parseAsPatternNode",
R"(
(sequencePattern
  (identifier nil #SomeType nil)
  (literal 1 #LiteralPositiveInteger))
)"));

                CHECK(checkStringEvaluationASTIsSameTo("`'(SomeType(1, 2)) parseAsPatternNode",
R"(
(sequencePattern
  (identifier nil #SomeType nil)
  (literal 1 #LiteralPositiveInteger)
  (literal 2 #LiteralPositiveInteger))
)"));
                CHECK(checkStringEvaluationASTIsSameTo("`'(SomeType(1, 2, 3)) parseAsPatternNode",
R"(
(sequencePattern
  (identifier nil #SomeType nil)
  (literal 1 #LiteralPositiveInteger)
  (literal 2 #LiteralPositiveInteger)
  (literal 3 #LiteralPositiveInteger))
)"));


          });
        });
    }
}
