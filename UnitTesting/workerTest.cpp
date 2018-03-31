#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker.h"

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

            Assert::AreEqual(short(0), result);
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
            bool left2[] = { true, false };
            bool center2[] = {false, false };
            bool right2[] = {false, true };

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCellsBottomEdge_case4) {
            /*
            * xxx
            * xxx
            */
            bool left2[] = { true, true };
            bool center2[] = { true, true };
            bool right2[] = { true, true };

            const auto result = countLivingCellsBottomEdge(left2, center2, right2);

            Assert::AreEqual(short(5), result);
        }

        //----------------------------------------------------

        TEST_METHOD(Worker_countLivingCellsTopEdge_case1) {
            /*
            * .x.
            * ...
            */
            bool left2[] = { false, false };
            bool center2[] = { false, true };
            bool right2[] = { false, false };

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(1), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case2) {
            /*
            * .x.
            * .x.
            */
            bool left2[] = { false, false };
            bool center2[] = { true, true };
            bool right2[] = { false, false };

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(1), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case3) {
            /*
            * ..x
            * x..
            */
            bool left2[] = { true, false };
            bool center2[] = { false, false };
            bool right2[] = { false, true };

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(2), result);
        }

        TEST_METHOD(Worker_countLivingCellsTopEdge_case4) {
            /*
            * xxx
            * xxx
            */
            bool left2[] = { true, true };
            bool center2[] = { true, true };
            bool right2[] = { true, true };

            const auto result = countLivingCellsTopEdge(left2, center2, right2);

            Assert::AreEqual(short(5), result);
        }
    };
}
