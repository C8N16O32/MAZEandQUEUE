//队列
template<class DATA>
class PQUEUE {//数据的地址保存在队列中的指针数组里 以符合广度优先搜索的要求
public:
	DATA**v = NULL;
	int head, end, size, minl = 64;
	//isempty 编译器会处理这种函数 不会造成过大开销
	int isempty() {
		if (end == head)return 1;
		return 0;
	}
	//动态数组
	void offsettoleft(DATA**dest, DATA**src) {
		if (isempty())
		{
			int len = end - head; head = 0; end = len;
			return;
		}//由于队列的特性，大部分特殊情况可忽略 并且一定是向左移动
		for (int i = head; i < end; i++) {
			dest[i - head] = src[i]; src[i] = NULL;//指针的值包含地址信息 地址为0具有的特殊含义是这个指针指向的目标为空
		}
		int len = end - head; head = 0; end = len;
		return;
	}
	void changesize(int needmorespace = 0) {

		//int len = end - head, sizenow = (len / minl + 1)*minl;					// ！！ BUG ！！  之前写的时候给忘了
		int len = end - head + needmorespace, sizenow = (len / minl + 1)*minl;		//正确的代码

		//数组尺寸为minl的整数倍 便于减小开销

		//只需要向前移动 不需要新开辟空间
		if (size == sizenow) {
			offsettoleft(v, v);
			return;
		}
		//需要缩小空间
		//需要扩大空间
		DATA**temp = new DATA*[sizenow];
		for (int i = 0; i < sizenow; i++)temp[i] = NULL;
		offsettoleft(temp, v); delete[]v; v = NULL; v = temp; temp = NULL;
		size = sizenow; return;
	}
	//push
	void push(DATA*data) {
		if (isempty()) {
			changesize();
			head = 0; end = 0;
			v[end] = data; end++;
		}
		if (end == size)changesize(1);
		v[end] = data; end++; return;
	}
	//pop
	DATA*pop() {
		if (isempty())return NULL;
		DATA*temp = v[head]; v[head] = NULL; head++;
		return temp;
	}
	//构造与析构函数
	PQUEUE(int manualminl = 64) {
		minl = manualminl;
		//防止队列在空的状态下被delete
		head = 0; end = 0;
	};
	~PQUEUE() {
		for (int i = head; i < end; i++)v[i] = NULL;
		delete[]v; v = NULL;
	}
};

class CELL {
public:
	int x, y, type;//迷宫的属性
	int color;//广度优先搜索需要的标记
	int color2;//打印路径时需要的额外的标记
	void set(int x, int y, int type, int color) {
		this->x = x; this->y = y; this->type = type; this->color = color; return;
	}
	//C++允许同名函数，但参数列表的类型要有区别。
	void set(int x, int y, char c) {
		//这是某题目中迷宫的格式 分别为墙、地面、可破坏的墙、终点、起点
		set(x, y, 0, 0);
		switch (c) {
		case '#':color = 1; type = 0; break;
		case '.':color = 0; type = 1; break;
		case 'x':color = 0; type = 2; break;
		case 'a':color = 0; type = 3; break;
		case 'r':color = 0; type = 4; break;
		default:type = 1; color = 0; break;
		};
	}
	void setcolor(int colorold) {
		//正在处理某格子，此格子被选中 则此格子的color标记要更新。这里color表示的是走到这里的最短步数
		//可破坏的墙壁需要多罚5步
		if (type != 2)color = colorold + 1;
		if (type == 2)color = colorold + 1 + 5;
		return;
	}
};

class SOLVE {
	//由于只是基础的队列 它能解的迷宫不能包含可破坏的墙壁
public:
	CELL**cell = NULL;
	int m, n;
	PQUEUE<CELL> *pq = NULL;

	void pq_new() {
		pq = new PQUEUE<CELL>;
		return;
	}
	void pq_delete() {
		delete pq; pq = NULL;
		return;
	}

	void cell_new(int m, int n) {
		this->m = m; this->n = n;
		cell = new CELL*[m];
		for (int i = 0; i < m; i++)
			cell[i] = new CELL[n];
	}
	void cell_delete() {
		for (int i = 0; i < m; i++) {
			delete[]cell[i]; cell[i] = NULL;
		}
		delete cell; cell = NULL;
	}

	SOLVE() {
		pq_new();
		//cell需要输入长度和宽度数据 不适合一开始就申请空间
	}
	~SOLVE() {
		if (pq != NULL)pq_delete();
		if (cell != NULL)cell_delete();
	}

	void inputformscanf() {
		char*temp = new char[n];
		for (int i = 0; i < m; i++) {
			scanf("%s", temp);
			for (int j = 0; j < n; j++) {
				cell[i][j].set(i, j, temp[j]);
				temp[j] = 0;
			}
		}
		return;
	}

	//遍历
	CELL* around(CELL*center, int ord) {
		int i = center->x, j = center->y;
		switch (ord) {
		case 1:i--; break;
		case 2:i++; break;
		default:break;
		}
		switch (ord) {
		case 3:j--; break;
		case 4:j++; break;
		default:break;
		}
		if (i < 0 || i >= m)return NULL;
		if (j < 0 || j >= n)return NULL;
		return cell[i] + j;
	}
	//判断是否可行
	int judge(CELL*temp) {
		//temp不是null color等于0 并且不是墙壁 就算可行;
		if (temp&&temp->color == 0 && temp->type != 0)return 1;
		return 0;
	}
	//广度优先搜索
	int fun() {
		//寻找起点
		int i, j;
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				if (cell[i][j].type == 4) {
					cell[i][j].color = 1;//起点与终点相邻时只需要1步
					goto A;
				}
	A:
		//广度优先搜索
		pq->push(cell[i] + j);
		CELL*now = NULL, *temp = NULL;
		while (1) {
			now = pq->pop();
			if (now == NULL) {
				return 0;//没有找到终点 但是所有可走的路已经探尽
			}
			if (now->type == 3) {
				return now->color - 1;//退出条件在终点进队之后 终点进队之前步数会再次+1 但终点不需要再往别的什么格子走一步
			}
			for (i = 1; i <= 4; i++) {
				temp = around(now, i);
				if (judge(temp)) {
					temp->setcolor(now->color);
					pq->push(temp);
					temp = NULL;
				}
			}
		}
	}

	//打印路径需要利用到第二个标记（否则会对之前的递推造成破坏）
	void printfpath() {
		int i, j, x, y;
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++) {
				cell[i][j].color2 = 0;
				if (cell[i][j].type == 3) { x = i; y = j; }//寻找终点。从终点往前递推
			}
		CELL*now = NULL, *temp = NULL;
		while (1) {
			if (cell[x][y].type == 4)break;
			now = cell[x] + y;
			now->color2 = 1;
			for (i = 1; i <= 4; i++) {
				temp = around(now, i);
				if (temp&&temp->type != 0 && temp->color != 0 && temp->color < cell[x][y].color) {
					x = temp->x; y = temp->y;
				}
			}
		}
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				if (cell[i][j].color2 == 0)printf(" |");
				else printf("@|");
			}
			printf("\n");
		}
	}

};

int main() {
	int m, n, re;
	SOLVE solve;
	while (~scanf("%d%d", &m, &n)) {
		solve.cell_new(m, n);
		solve.inputformscanf();
		re = solve.fun();
		printf("%d\n", re);//0表示无解
		if (re)solve.printfpath();
		solve.cell_delete();
		//重置队列 便于下一次使用
		solve.pq_delete();
		solve.pq_new();
	}
	solve.pq_delete();
	return 0;
}
