#include "gtest/gtest.h"
#include <memory>

#include "04SmartPointers.h"

TEST(SmartPointersTestItem18, UniquePtrFactoryWithCustomDelete)
{
    {
        auto rgb = getColorCode(255, 255, 255);
        auto cmyk = getColorCode(0, 0, 0, 0);

        // smart_ptr with custom deletor has the same size as plain ptr
        EXPECT_EQ(sizeof(rgb), sizeof(Color*));
    }

    // custom deletor modifies class variable
    EXPECT_EQ(Color::deletedCount, 2);
}

TEST(SmartPointersTestItem18, UniquePtrConversionToSharedPtr)
{
    std::shared_ptr<Color> rgb = getColorCode(255, 255, 255);

    EXPECT_EQ(rgb->toHex(), "#FFFFFF");
}

TEST(SmartPointersTestItem19, SharedPtrIsTwiceTheSizeOfPtr)
{
    int* pInt = new int(42);
    std::shared_ptr<int> spInt(pInt);

    EXPECT_EQ(sizeof(spInt), 2 * sizeof(pInt));
}

TEST(SmartPointersTestItem19, SharedPtrWithCustomDelete)
{
    std::size_t deletedCount = 0;
    auto delInt = [&deletedCount](int* pInt)
    {
        deletedCount++;
        delete pInt;
    };

    {
        std::shared_ptr<int> spInt(nullptr, delInt);
        EXPECT_EQ(deletedCount, 0);
    }
    EXPECT_EQ(deletedCount, 1);
}

TEST(SmartPointersTestItem19, CreateTwoControlBlocksForOneObject) {
    auto doubleFreeSharedPtr = []()
    {
        int* ptr = new int(42);

        std::shared_ptr<int> sharedPtr1(ptr);  
        std::shared_ptr<int> sharedPtr2(ptr); 
    };

    ASSERT_DEATH(doubleFreeSharedPtr(), "double free");
}

TEST(SmartPointersItem20, LinkedListConstruction)
{
    LinkedList<int> emptyList;
    LinkedList<int> listWithElements { 1, 2, 3 };

    EXPECT_EQ(emptyList.size(), 0);
    EXPECT_EQ(listWithElements.size(), 3);
    EXPECT_NE(emptyList, listWithElements);

    listWithElements.erase();

    EXPECT_EQ(listWithElements.size(), 0);
    EXPECT_EQ(emptyList, listWithElements);
}

TEST(SmartPointersItem20, LinkedListCopy)
{
    LinkedList<int> listWithElements { 1, 2, 3 };
    LinkedList<int> copiedList(listWithElements);

    EXPECT_EQ(copiedList.size(), 3);
    EXPECT_EQ(copiedList, listWithElements);
}

TEST(SmartPointersItem20, LinkedListMove)
{
    LinkedList<int> listWithElements { 1, 2, 3 };
    LinkedList<int> copiedList(listWithElements);
    LinkedList<int> movedList(std::move(listWithElements));

    EXPECT_EQ(movedList.size(), 3);
    EXPECT_EQ(movedList, copiedList);
}

TEST(SmartPointersItem20, LinkedListInsertAndRemove)
{
    LinkedList<int> desiredList { 1, 2, 3 };
    LinkedList<int> newList;
    newList.insert(1, 0).insert(2, 1).insert(3, 2);

    EXPECT_EQ(newList, desiredList);

    newList.remove(2).remove(1).remove(0);
    LinkedList<int> emptyList;

    EXPECT_EQ(newList, emptyList);
}

TEST(SmartPointersItem20, LinkedListErrorHandling)
{
    LinkedList<int> emptyList;

    EXPECT_THROW(emptyList.remove(0), std::runtime_error);
    EXPECT_THROW(emptyList.remove(5), std::out_of_range);
    EXPECT_THROW(emptyList.insert(1, 1), std::out_of_range);
    EXPECT_THROW(emptyList.pop_back(), std::runtime_error);
    EXPECT_THROW(emptyList.pop_front(), std::runtime_error);
}

// TODO: Pimpl (?)