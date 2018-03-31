#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(Worker) {
    public:

        TEST_METHOD(Worker_countLivingCells_case1) {
            /*
             * x..
             * ...
             * ..x
             */
            bool left3[] = {false, false, true};
            bool center3[] = {false, false, false};
            bool right3[] = {true, false, false};

            const auto result = countLivingCells(left3, center3, right3);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCells_case2) {
            /*
            * x..
            * ...
            * ..x
            */
            bool left3[] = {false, false, true};
            bool center3[] = {false, true, false};
            bool right3[] = {true, false, false};

            const auto result = countLivingCells(left3, center3, right3);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCells_case3) {
            /*
            * ...
            * .xx
            * xxx
            */
            bool left3[] = {true, false, false};
            bool center3[] = {true, true, false};
            bool right3[] = {true, true, false};

            const auto result = countLivingCells(left3, center3, right3);

            Assert::AreEqual(short(4), result);
        }

        TEST_METHOD(Worker_countLivingCells_case4) {
            /*
            * ...
            * .x.
            * ...
            */
            bool left3[] = {false, false, false};
            bool center3[] = {false, true, false};
            bool right3[] = {false, false, false};

            const auto result = countLivingCells(left3, center3, right3);

            Assert::AreEqual(short(0), result);
        }

        //----------------------------------------------------

        TEST_METHOD(Worker_countLivingCellsBottomEdge_case1) {
            /*
            * .x.
            * ...
            */
            bool left2[] = {false, false};
            bool center2[] = {false, true};
            bool right2[] = {false, false};

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(1), result);
        }

        TEST_METHOD(Worker_countLivingCellsBottomEdge_case2) {
            /*
            * .x.
            * .x.
            */
            bool left2[] = {false, false};
            bool center2[] = {true, true};
            bool right2[] = {false, false};

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(1), result);
        }

        TEST_METHOD(Worker_countLivingCellsBottomEdge_case3) {
            /*
            * ..x
            * x..
            */
            bool left2[] = {true, false};
            bool center2[] = {false, false};
            bool right2[] = {false, true};

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCellsBottomEdge_case4) {
            /*
            * xxx
            * xxx
            */
            bool left2[] = {true, true};
            bool center2[] = {true, true};
            bool right2[] = {true, true};

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(5), result);
        }

        //----------------------------------------------------

        TEST_METHOD(Worker_countLivingCellsTopEdge_case1) {
            /*
            * .x.
            * ...
            */
            bool left2[] = {false, false};
            bool center2[] = {false, true};
            bool right2[] = {false, false};

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(0), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case2) {
            /*
            * .x.
            * .x.
            */
            bool left2[] = {false, false};
            bool center2[] = {true, true};
            bool right2[] = {false, false};

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(1), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case3) {
            /*
            * ..x
            * x..
            */
            bool left2[] = {true, false};
            bool center2[] = {false, false};
            bool right2[] = {false, true};

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case4) {
            /*
            * xxx
            * xxx
            */
            bool left2[] = {true, true};
            bool center2[] = {true, true};
            bool right2[] = {true, true};

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(5), result);
        }

        //----------------------------------------------------

        struct CellStateDeciderTest {
            CellStateDeciderTest(bool currentCell, short neighbourCells, bool expectedCellState) :
                currentCell(currentCell),
                neighbourCells(neighbourCells),
                expectedCellState(expectedCellState) {
            }

            bool currentCell;
            short neighbourCells;
            bool expectedCellState;
        };

        TEST_METHOD(Worker_decideNewCellState) {
            std::vector<CellStateDeciderTest> tests = {
                CellStateDeciderTest(true, 0, false),
                CellStateDeciderTest(true, 1, false),
                CellStateDeciderTest(true, 2, true),
                CellStateDeciderTest(true, 3, true),
                CellStateDeciderTest(true, 4, false),
                CellStateDeciderTest(true, 5, false),
                CellStateDeciderTest(true, 6, false),
                CellStateDeciderTest(true, 7, false),
                CellStateDeciderTest(true, 8, false),
                CellStateDeciderTest(false, 0, false),
                CellStateDeciderTest(false, 1, false),
                CellStateDeciderTest(false, 2, false),
                CellStateDeciderTest(false, 3, true),
                CellStateDeciderTest(false, 4, false),
                CellStateDeciderTest(false, 5, false),
                CellStateDeciderTest(false, 6, false),
                CellStateDeciderTest(false, 7, false),
                CellStateDeciderTest(false, 8, false)
            };

            for (auto test : tests) {
                const auto result = decideNewCellState(&test.currentCell, &test.neighbourCells);
                Assert::AreEqual(test.expectedCellState, result);
            }
        }

        //----------------------------------------------------

        TEST_METHOD(Worker_generateNewColumn_case1) {
            /*
             * .x.
             * x..
             * x.x
             * .xx
             * ...
            */
            const int size = 5;
            bool oldLeft[size] = {false, false, true, true, false};
            bool oldCenter[size] = {false, true, false, false, true};
            bool oldRight[size] = {false, true, true, false, false};

            const auto result = generateNewColumn(oldLeft, oldCenter, oldRight, size);

            const bool expectedResult[size] = {false, true, false, false, false};

            for (int i = 0; i < size; i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }

        TEST_METHOD(Worker_generateNewColumn_case2) {
            /*
            * ...
            * x..
            * xx.
            * ..x
            * x..
            * x..
            * .x.
            */
            const int size = 7;
            bool oldLeft[size] = {false, true, true, false, true, true, false};
            bool oldCenter[size] = {true, false, false, false, true, false, false};
            bool oldRight[size] = {false, false, false, true, false, false, false};

            const auto result = generateNewColumn(oldLeft, oldCenter, oldRight, size);

            const bool expectedResult[size] = {false, true, true, false, true, true, false};

            for (int i = 0; i < size; i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }
    };
}
