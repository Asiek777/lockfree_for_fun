#include "pch.h"
#include "../../lockfree_for_fun/queue.h"

namespace SingleThread {

	TEST(SingleThreadIntTest, BATest) {
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

	TEST(ResizeBuffer, SmallStartSize) {
		Queue<int> queue(1); 
		int tab[] = { 5, 1,42, 3 ,2, 2 };
		for (int i = 0; i < 6; i++)
			queue.push(tab[i]);
		for (int i = 0; i < 6; i++)
			EXPECT_EQ(queue.pop(), tab[i]);
	}
	TEST(TeplateTest, BoolsQueue) {
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
}