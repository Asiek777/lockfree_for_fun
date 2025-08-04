#pragma once
#include <string>
#include <array>

constexpr int DEFAULT_SIZE = 128;

template <typename T, int SIZE = DEFAULT_SIZE>
class FixedSizeQueue {
	std::array<T, SIZE> buffer;
	std::atomic<unsigned int> front = 0, back = 0;

public:

	bool pop_or_empty(T& result);
	bool push_or_full(T val);
	bool isEmpty() const;
	bool isFull() const;
	int count() const;
};

template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::pop_or_empty(T& result)
{
	if (isEmpty()) 
		return false;
	
	result = buffer[front++ % SIZE];
	return true;
}

template <typename T, int SIZE>
bool FixedSizeQueue<T, SIZE>::push_or_full(T val)
{
	if (isFull()) 
		return false;
	
	int write_back = back++;
	buffer[write_back % SIZE] = val;
	return true;

}

template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::isEmpty() const
{
	return front >= back;
}
template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::isFull() const
{
	return back - SIZE >= front;
}

template<typename T, int SIZE>
inline int FixedSizeQueue<T, SIZE>::count() const
{
	return back - front;
}