#include "gtest/gtest.h"
#include <concepts>
#include <tuple>
#include <memory>

#include "03ModernCPP.h"

TEST(ModernCPPTestItem7, BracedInitializationWithoutInitializerList)
{
    Value x(5);
    Value y{ 5 };

    EXPECT_EQ(x.getValue(), y.getValue());
}

TEST(ModernCPPTestItem7, BracedInitializationWithInitializerList)
{
    ValueFlag x(5, true); // parentheses initialization
    ValueFlag y{ 5, true }; // braces initialization, initializer_list ctor is called

    EXPECT_TRUE(x.isFirstCtorCalled());
    EXPECT_TRUE(y.isSecondCtorCalled());

    EXPECT_NE(x.getValue(), y.getValue());
    EXPECT_NE(x.getFlag(), y.getFlag());
    EXPECT_NE(y.getData().size(), 0);
}

// doesn't work with MSVC
#ifdef __GNUC__
TEST(ModernCPPTestItem7, CopyConstructorIgnoredIfBracedInitialization)
{
    ValueFlag x( 5, true );
    ValueFlag y{ x }; // braces initialization, initializer_list ctor is called (because of operator int())

    EXPECT_NE(x.getValue(), y.getValue());
    EXPECT_TRUE(y.isSecondCtorCalled());
}
#endif

// works with MSVC only because NULL is defined there as 0,
// but in GCC it is defined as (void*)0, which leads to ambiguity error
#ifdef _MSC_VER
TEST(ModernCPPTestItem8, NULLDoesNotCallPointerOverload)
{
    CheckPointer x(NULL);
    CheckPointer y(nullptr);

    EXPECT_FALSE(x.isPointer());
    EXPECT_TRUE(y.isPointer());
}
#endif

TEST(ModernCPPTestItem9, TypedefAndAliasShowSameBehavior)
{
    IncTypedef typedefFunc = increment;
    IncAlias aliasFunc = increment;

    EXPECT_EQ(typedefFunc(5), aliasFunc(5));
    EXPECT_TRUE((std::is_same_v<decltype(typedefFunc), decltype(aliasFunc)>));
}

TEST(ModernCPPTestItem9, TypedefAndAliasTemplateShowSameBehavior)
{
    MatrixTypedef<int>::type typedefMatrix = { { 1, 2, 3 }, { 4, 5, 6 } };
    MatrixAlias<int> aliasMatrix = { { 1, 2, 3 }, { 4, 5, 6 } };

    EXPECT_EQ(typedefMatrix.size(), aliasMatrix.size());
    EXPECT_EQ(typedefMatrix[0].size(), aliasMatrix[0].size());
    EXPECT_EQ(typedefMatrix[1][2], aliasMatrix[1][2]);
    EXPECT_TRUE((std::is_same_v<decltype(typedefMatrix), decltype(aliasMatrix)>));
}

TEST(ModernCPPTestItem10, ScopedUnscopedEnumsShowSameBehavior)
{
    using namespace Enums;

    RGB color = { 0, 0, 255 };

    std::uint8_t blueFromUnscoped = std::get<ColorUnscoped::Blue>(color);
    std::uint8_t blueFromNamespace = std::get<Blue>(color); // enum member leaked to the outer scope
    std::uint8_t blueFromScoped = std::get<getType(ColorScoped::Blue)>(color);

    EXPECT_EQ(blueFromUnscoped, blueFromNamespace);
    EXPECT_EQ(blueFromNamespace, blueFromScoped);
    EXPECT_EQ(blueFromUnscoped, blueFromScoped);
}

TEST(ModernCPPTestItem11, ProcessPointerWithDeletedFunctions)
{
    ProcessPointerNoDelete processVoid;
    ProcessPointerNoDelete processChar;
    ProcessPointerNoDelete processAny;

    ProcessPointerWithDelete processWithDelete;

    void* voidPtr = nullptr;
    char* charPtr = nullptr;
    int* intPtr = nullptr;

    processVoid.takePtr(voidPtr);
    processChar.takePtr(charPtr);
    processAny.takePtr(intPtr);

    // compilation error, use of deleted function
    // processWithDelete.takePtr(voidPtr); 
    // processWithDelete.takePtr(charPtr);
    processWithDelete.takePtr(intPtr);

    EXPECT_TRUE(processVoid.isWrongPtrType());
    EXPECT_TRUE(processChar.isWrongPtrType());
    EXPECT_FALSE(processAny.isWrongPtrType());
}

TEST(ModernCPPTestItem12, DerivedClassFunctionsNotOverriden)
{
    std::unique_ptr<Base> upb = std::make_unique<ProblematicDerived>();

    // Derived class functions are never called so the values are always 0
    EXPECT_EQ(upb->func1(), 0);
    EXPECT_EQ(upb->func2(42), 0);
    EXPECT_EQ(upb->func3(), 0);
    EXPECT_EQ(upb->func4(), 0);
}

TEST(ModernCPPTestItem12, FunctionReferenceQualifiers)
{
    Component component { 1 };

    auto valuesFromLvalue = component.getValues();
    // makeComponent() returns Components with values { -1 }, 
    // but since getValues() are called from rvalue, it is replaced with { 0 }
    auto valuesFromRvalue = makeComponent().getValues(); 

    EXPECT_EQ(valuesFromLvalue[0], 1);
    EXPECT_EQ(valuesFromRvalue[0], 0);
}