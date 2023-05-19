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