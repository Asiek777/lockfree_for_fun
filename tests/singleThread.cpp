#include "pch.h"
#include "../../lockfree_for_fun/queue.h"
#include <queue>
#include <random>

namespace SingleThread {

	TEST(SingleThread, BasicTest) {
		Queue<int> queue;

		EXPECT_EQ(queue.isEmpty(), true);
		queue.push(5);
		queue.push(1);
		queue.push(42);

		EXPECT_EQ(queue.isEmpty(), false);
		EXPECT_EQ(queue.pop(), 5);
		EXPECT_EQ(queue.pop(), 1);
		EXPECT_EQ(queue.pop(), 42);
		EXPECT_EQ(queue.isEmpty(), true);
		EXPECT_ANY_THROW(queue.pop());
	}

	TEST(SingleThread, ResizeTest) {
		Queue<int> queue(1);
		int tab[] = { 5, 1,42, 3 ,2, 2 };
		for (int i = 0; i < 6; i++)
			queue.push(tab[i]);
		for (int i = 0; i < 6; i++)
			EXPECT_EQ(queue.pop(), tab[i]);
	}
	TEST(SingleThread, BoolsQueue) {
		Queue<int> queue;
		int tab[] = { 5, 1,42, 3 ,2, 2 };
		for (int i = 0; i < 6; i++)
			queue.push(tab[i]);
		for (int i = 0; i < 6; i++) {
			EXPECT_EQ(queue.isEmpty(), false);
			EXPECT_EQ(queue.pop(), tab[i]);
		}
		EXPECT_EQ(queue.isEmpty(), true);
		EXPECT_ANY_THROW(queue.pop());
	}

	TEST(SingleThread, RandomOps) {
		constexpr int buffer_size = 10;
		Queue<double> queue(buffer_size);
		std::queue<double> stl_queue;
		constexpr int stepCount = 100000;
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> nextNumber(
			std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
		std::uniform_int_distribution<int> nextOp(0, 1);
		for (int i = 0; i < stepCount; i++) {
			if (nextOp(mt)) {
				double val = nextNumber(mt);
				queue.push(val);
				stl_queue.push(val);
				EXPECT_EQ(queue.count(), stl_queue.size());
			}
			else if (queue.count() > 0) {
				double firstVal = queue.pop();
				double secondVal = stl_queue.front();
				stl_queue.pop();
				EXPECT_EQ(firstVal, secondVal);
				EXPECT_EQ(queue.count(), stl_queue.size());
			}
		}
	}
}