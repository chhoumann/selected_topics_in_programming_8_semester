#include <gtest/gtest.h>
#include "../src/types.cpp"
#include "../src/symbol_table.cpp"

// Solves requirement 9: Implement unit tests (e.g. test symbol table methods and pretty-printing of reaction rules).

TEST(MyTestSuite, TestReactionOstreamOverload) {
    // Arrange
    System s = System();

    auto A = s("A", 1);
    auto B = s("B", 1);
    auto C = s("C", 1);

    auto r = s(A + B >>= C, 0.001);

    std::ostringstream ss;

    // Act
    ss << r;

    // Assert
    EXPECT_EQ(ss.str(), "A + B → C (rate: 0.001)");
}

TEST(SymbolTableTest, StoreAndLookup) {
    // Arrange
    SymbolTable<int> symbolTable;
    int expectedValue = 100;

    // Act
    symbolTable.store("A", expectedValue);
    int actualValue = symbolTable.lookup("A");

    // Assert
    EXPECT_EQ(actualValue, expectedValue);
}

TEST(SymbolTableTest, LookupThrowsExceptionWhenKeyNotFound) {
    // Arrange
    SymbolTable<int> symbolTable;

    // Assert
    EXPECT_THROW(symbolTable.lookup("A"), std::runtime_error);
}

TEST(SymbolTableTest, StoreThrowsExceptionWhenKeyFound) {
    // Arrange
    SymbolTable<int> symbolTable;

    // Act
    symbolTable.store("A", 100);

    // Assert
    EXPECT_THROW(symbolTable.store("A", 100), std::runtime_error);
}

TEST(SymbolTableTest, Update) {
    // Arrange
    SymbolTable<int> symbolTable;
    int expectedValue = 100;

    // Act
    symbolTable.store("A", 0);
    int actualValue = symbolTable.update("A", expectedValue);

    // Assert
    EXPECT_EQ(actualValue, expectedValue);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
