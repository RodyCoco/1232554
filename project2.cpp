#include <iostream>
#include <fstream>
using namespace std;

template<class T>
class Queue {
public:
	Queue(int queueCapacity = 10) :capacity(queueCapacity), front(-1), rear(-1) { queue = new T[capacity]; }
	bool isempty() { return rear == front; }
	T& Front() { return queue[(front + 1) % capacity]; }
	T& Rear() { return queue[rear]; }
	void push(const T& x) {
		if (isempty()) {
			front = rear = 0;
		}

		if ((rear + 1) % capacity == front) {
			T* tmp = new T[2 * capacity];
			for (int i = 0; i < capacity; i++) {
				tmp[i] = queue[i];
			}
			capacity = 2 * capacity;
			delete queue;
			queue = tmp;
		}
		rear = (rear + 1) % capacity;
		queue[rear] = x;
	}
	void pop() {
		front = (front + 1) % capacity;
		queue[front].~T();
	}
	void popRear() {
		queue[rear].~T();
		rear = (rear - 1) % capacity;
	}
private:
	T* queue;
	int front, rear;
	int capacity;
};
struct pos {
	pos(int R = 0, int C = 0) :r(R), c(C) {}
	int r;
	int c;
};

struct floormap {
	int type;//-1:R  -2:cleaned
	bool visited;
	int BFScal;
};
int count = 0;//the step robot use
int R, C, B, unclean = 0;
ifstream inputFile;
fstream outputFile;
floormap map[1000][1000];


pos findunclean(pos cur) {
	pos tmp = cur;
	Queue<pos> Q;
	Q.push(tmp);
	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++) {
			map[i][j].visited = false;
			map[i][j].BFScal = -1;
		}
	map[tmp.r][tmp.c].BFScal = 0;
	if (cur.r == 1 && cur.c == 5)
		int a = 0;

	while (true) {
		tmp = Q.Front();
		Q.pop();
		map[tmp.r][tmp.c].visited = true;
		if (tmp.c + 1 < C) {
			if (map[tmp.r][tmp.c + 1].type != 1 && map[tmp.r][tmp.c + 1].visited == false) {
				map[tmp.r][tmp.c + 1].BFScal = map[tmp.r][tmp.c].BFScal + 1;
				if (map[tmp.r][tmp.c + 1].type == 0) {
					tmp.c++;
					return tmp;
				}
				Q.push(pos(tmp.r, tmp.c + 1));
			}
		}
		if (tmp.c - 1 >= 0) {
			if (map[tmp.r][tmp.c - 1].type != 1 && map[tmp.r][tmp.c - 1].visited == false) {
				map[tmp.r][tmp.c - 1].BFScal = map[tmp.r][tmp.c].BFScal + 1;
				if (map[tmp.r][tmp.c - 1].type == 0) {
					tmp.c--;
					return tmp;
				}
				Q.push(pos(tmp.r, tmp.c - 1));
			}
		}
		if (tmp.r + 1 < R) {
			if (map[tmp.r + 1][tmp.c].type != 1 && map[tmp.r + 1][tmp.c].visited == false) {
				map[tmp.r + 1][tmp.c].BFScal = map[tmp.r][tmp.c].BFScal + 1;
				if (map[tmp.r + 1][tmp.c].type == 0) {
					tmp.r++;
					return tmp;
				}
				Q.push(pos(tmp.r + 1, tmp.c));
			}
		}
		if (tmp.r - 1 >= 0) {
			if (map[tmp.r - 1][tmp.c].type != 1 && map[tmp.r - 1][tmp.c].visited == false) {
				map[tmp.r - 1][tmp.c].BFScal = map[tmp.r][tmp.c].BFScal + 1;
				if (map[tmp.r - 1][tmp.c].type == 0) {
					tmp.r--;
					return tmp;
				}
				Q.push(pos(tmp.r - 1, tmp.c));
			}
		}
	}
}

void do_one_clean(pos des) {
	Queue<pos> Q;
	pos tmp = des;
	while (map[tmp.r][tmp.c].BFScal != 0) {
		Q.push(tmp);
		if (tmp.c + 1 < C) {
			if (map[tmp.r][tmp.c + 1].BFScal == map[tmp.r][tmp.c].BFScal - 1) {
				tmp.c++;
				continue;
			}

		}
		if (tmp.c - 1 >= 0) {
			if (map[tmp.r][tmp.c - 1].BFScal == map[tmp.r][tmp.c].BFScal - 1) {
				tmp.c--;
				continue;
			}

		}
		if (tmp.r + 1 < R) {
			if (map[tmp.r + 1][tmp.c].BFScal == map[tmp.r][tmp.c].BFScal - 1) {
				tmp.r++;
				continue;
			}
		}
		if (tmp.r - 1 >= 0) {
			if (map[tmp.r - 1][tmp.c].BFScal == map[tmp.r][tmp.c].BFScal - 1) {
				tmp.r--;
				continue;
			}
		}
	}
	while (!Q.isempty()) {
		pos p = Q.Rear();
		Q.popRear();
		outputFile << p.r << " " << p.c << endl;
	}
}

pos clean(pos cur) {
	pos des = findunclean(cur);
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++)
			cout << map[i][j].BFScal << " ";
		cout << endl;
	}
	cout << endl;
	do_one_clean(des);
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++)
			cout << map[i][j].BFScal << " ";
		cout << endl;
	}
	cout << endl;
	map[des.r][des.c].type = -2;
	return des;
}

int main() {
	pos battery;

	inputFile.open("floor.data");
	if (!inputFile)
		cout << "Fail to open 'floor.data'" << endl;
	outputFile.open("floor.output", ios::out);
	if (!outputFile)
		cout << "Fail to open 'floor.output'" << endl;
	inputFile >> R >> C >> B;

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			map[i][j].visited = false;
			map[i][j].BFScal = -1;
		}
	}

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			char c;
			inputFile >> c;
			if (c == 'R') {
				map[i][j].type = -1;
				battery.r = i;
				battery.c = j;
			}
			else if (c == '0') {
				map[i][j].type = c - '0';
				unclean++;
			}
			else
				map[i][j].type = c - '0';
		}
	}

	outputFile << battery.r << " " << battery.c << endl;

	pos cur = battery;
	while (unclean > 0) {
		cur = clean(cur);
		unclean--;
	}


	inputFile.close();
	outputFile.close();
	return 0;
}

