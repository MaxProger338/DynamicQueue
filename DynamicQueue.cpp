#include <iostream>
#include <initializer_list>
using namespace std;

typedef unsigned int uint;

template <typename T>
class DynamicQueue
{
    private:
		class DoubleLinkedList
		{
			private:
				class Node
				{
				public:
					T value;
					Node* next;
					Node* prev;

					Node(T value) : value{ value }, next{ nullptr }, prev{ nullptr } {}
				};

				Node* _first;
				Node* _last;

			public:
				DoubleLinkedList() : _first{ nullptr }, _last{ nullptr } {}

				DoubleLinkedList(initializer_list<T> list) : DoubleLinkedList()
				{
					for (auto i : list)
					{
						addBack(i);
					}
				}

				DoubleLinkedList(const DoubleLinkedList& list) : DoubleLinkedList()
				{
					Node* current = list._first;

					while (current)
					{
						addBack(current->value);
						current = current->next;
					}
				}

				DoubleLinkedList(DoubleLinkedList&& list)
				{
					Node* current = list._first;
					_first = current;
					_last = list._last;

					while (current)
					{
						_first->next = current->next;
						_first->prev = current->prev;
						_first = _first->next;

						current = current->next;
					}
					_first = list._first;

					list._first = nullptr;
					list._last = nullptr;
				}

				~DoubleLinkedList()
				{
					if (!isEmpty())
					{
						Node* current = _first->next;
						while (current)
						{
							delete _first;
							_first = current;
							current = current->next;
						}

						delete _first;

						_first = nullptr;
						_last = nullptr;
					}
				}

				bool isEmpty() const
				{
					return _first == nullptr;
				}

				uint getSize() const
				{
					uint counter = 0;

					Node* current = _first;
					while (current)
					{
						counter++;
						current = current->next;
					}

					return counter;
				}

				DoubleLinkedList& clear()
				{
					this->~DoubleLinkedList();

					return *this;
				}

				T getFirst() const
				{
					return _first->value;
				}

				DoubleLinkedList& addBack(T value)
				{
					Node* node = new Node(value);
					if (isEmpty())
					{
						_first = node;
						_last = node;
					}
					else
					{
						_last->next = node;
						node->prev = _last;
						_last = node;
					}

					return *this;
				}

				T deleteForward()
				{
					if (isEmpty())
						throw "Out of range";

					Node* current = _first;
					T value = current->value;

					_first = current->next;
					_first->prev = nullptr;

					delete current;

					return value;
				}

				friend ostream& operator<<(ostream& out, const DoubleLinkedList& list)
				{
					Node* current = list._first;
					while (current)
					{
						out << current->value << '\t';
						current = current->next;
					}
					out << endl;

					return out;
				}
		};

		DoubleLinkedList _list;

    public:
		DynamicQueue() {}

		explicit DynamicQueue(initializer_list<T> list) : _list{ list } {}

		DynamicQueue(const DynamicQueue& stack) : _list{ stack._list } {}

		DynamicQueue(DynamicQueue&& stack) : _list{ move(stack._list) } {}

		bool isEmpty() const
		{
			return _list.isEmpty();
		}

		uint getSize() const
		{
			return _list.getSize();
		}

		DynamicQueue& clear()
		{
			_list.clear();

			return *this;
		}

		DynamicQueue& enqueue(T value)
		{
			_list.addBack(value);

			return *this;
		}

		T dequeue()
		{
			return _list.deleteForward();
		}

		T peek()
		{
			return _list.getFirst();
		}

		friend ostream& operator<<(ostream& out, const DynamicQueue& queue)
		{
			out << queue._list << endl;

			return out;
		}
};

int main()
{
	DynamicQueue<int> q;

    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    cout << q << endl;

    return 0;
}
