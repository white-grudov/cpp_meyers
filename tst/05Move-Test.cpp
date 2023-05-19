#include "gtest/gtest.h"
#include <vector>
#include <utility>

#include "05Move.h"

TEST(MoveTestItem23, CopyCtorIsInvokedInsteadOfMove)
{
    Item item(42);
    // copy ctor is invoked instead of move ctor because of const qualifier
    IncorrectItemContainer itemContainer(item);

    EXPECT_EQ(CtorIndicator::getCtorType(), CtorInvoked::Copy);
}

TEST(MoveTestItem23, MisuseOfForwardLeadsToCopyNotMove)
{
    IncorrectItemContainer itemContainer(IncorrectItemContainer(42));

    EXPECT_EQ(CtorIndicator::getCtorType(), CtorInvoked::Copy);
}

TEST(MoveTestItem23, MoveCtorIsInvokedAsShould)
{
    Item item(42);
    CorrectItemContainer itemContainer(item);

    EXPECT_EQ(CtorIndicator::getCtorType(), CtorInvoked::Move);

    CorrectItemContainer itemContainer2(CorrectItemContainer(42));

    EXPECT_EQ(CtorIndicator::getCtorType(), CtorInvoked::Move);
}

TEST(MoveItem24, LvalueIsPassed)
{
    std::vector<int> v;

    EXPECT_FALSE(func(v));
}

TEST(MoveItem24, RvalueIsPassed)
{
    std::vector<int> v;

    EXPECT_TRUE(func(std::move(v)));
}

TEST(MoveItem25, MoveForRvalueReference)
{
    Person person("Test");
    Person movedPerson(std::move(person));

    EXPECT_EQ(movedPerson.getName(), "Test");
    EXPECT_EQ(person.getName(), "");
}

TEST(MoveItem25, ForwardForUniversalReference)
{
    Person person("");
    person.setName("CStr");
    EXPECT_EQ(person.getName(), "CStr");

    person.setName(std::string("rvalueStr"));
    EXPECT_EQ(person.getName(), "rvalueStr");

    std::string lvalueStr("lvalueStr");
    person.setName(lvalueStr);
    EXPECT_EQ(person.getName(), "lvalueStr");
    EXPECT_EQ(lvalueStr, "lvalueStr");
}

TEST(MoveItem27, TagDispatchForUniversalReference)
{
    std::string lvalueStr("lvalueStr");

    EXPECT_FALSE(processStrOrNumeral(lvalueStr));
    EXPECT_FALSE(processStrOrNumeral(std::string("rvalueStr")));
    EXPECT_FALSE(processStrOrNumeral("cStr"));

    EXPECT_TRUE(processStrOrNumeral(42));
    EXPECT_TRUE(processStrOrNumeral(static_cast<short>(42)));
}

TEST(MoveItem27, ConstrainedUniversalReferenceTemplate)
{
    std::string lvalueStr("lvalueStr");

    PersonConstrained personLvalue(lvalueStr);
    EXPECT_EQ(personLvalue.getName(), "lvalueStr");
    EXPECT_EQ(personLvalue.getCtorIndicator(), PersonCtorType::Universal);

    PersonConstrained personRvalue(std::string("rvalueStr"));
    EXPECT_EQ(personRvalue.getName(), "rvalueStr");
    EXPECT_EQ(personRvalue.getCtorIndicator(), PersonCtorType::Universal);

    PersonConstrained personCStr("cStr");
    EXPECT_EQ(personCStr.getName(), "cStr");
    EXPECT_EQ(personCStr.getCtorIndicator(), PersonCtorType::Universal);

    PersonConstrained personInt(42);
    EXPECT_EQ(personInt.getName(), "nameFromIdx");
    EXPECT_EQ(personInt.getCtorIndicator(), PersonCtorType::Integral);

    PersonConstrained personShort(static_cast<short>(42));
    EXPECT_EQ(personShort.getName(), "nameFromIdx");
    EXPECT_EQ(personShort.getCtorIndicator(), PersonCtorType::Integral);

    PersonConstrained personForCopy("copy");
    PersonConstrained copiedPerson(personForCopy);
    EXPECT_EQ(copiedPerson.getName(), "copy");
    EXPECT_EQ(personForCopy.getName(), "copy");
    EXPECT_EQ(copiedPerson.getCtorIndicator(), PersonCtorType::Copy);

    PersonConstrained personForMove("move");
    PersonConstrained movedPerson(std::move(personForMove));
    EXPECT_EQ(movedPerson.getName(), "move");
    EXPECT_EQ(personForMove.getName(), "");
    EXPECT_EQ(movedPerson.getCtorIndicator(), PersonCtorType::Move);
}