#include "stdio.h" 
//循环队列
template<class T>
class QUEUE {
	//数据
	T*v = NULL; int m_head = 0, m_end = 0, m_capacity = 0;
	//调整幅度
	int m_dc = 16, m_step = 16, m_shc = 0, m_dsh = 4;
public:
	//信息
	bool isempty() { if (m_head == m_end)return 1; return 0; };
	int size() { return m_end - m_head; };
	void setdcstepshstep(int dc, int step, int dsh) { m_dc = dc; m_step = step; m_dsh = dsh; return; };
	//调整大小
	void changesize(int newcapacity) {
		int s1 = m_head, l1 = size(), s2 = 0, l2 = 0;
		if (m_end > m_capacity) { l2 = m_end - m_capacity; l1 -= l2; };
		T*temp = new T[newcapacity]; int i;
		for (i = s1; i < s1 + l1; i++)temp[i - s1] = v[i];
		for (i = s2; i < s2 + l2; i++)temp[i - s2 + l1] = v[i];
		delete[]v; v = temp; temp = NULL;
		m_head = 0; m_end = l1 + l2; m_capacity = newcapacity;
		return;
	};
	void checkcapa(int ns/*需要的额外空间数*/) {
		int size = QUEUE::size() + ns;
		size = ((size - m_dc) / m_step + 1)*m_step + m_dc;
		if (size > m_capacity) { changesize(size); m_shc = m_dsh; return; };
		if (m_shc) { m_shc--; return; };
		if (size < m_capacity)changesize(size);
		return;
	};
	//push_end
	void push_end(T&inp) {
		checkcapa(1);
		v[m_end%m_capacity] = inp; m_end++;
		return;
	};
	//head
	T&head() {
		return v[m_head];
	};
	//pop_head
	void pop_head() {
		m_head++;
		if (m_head == m_capacity) {
			m_head -= m_capacity; m_end -= m_capacity;
		};
		checkcapa(0);
		return;
	};
	//构造/析构
	virtual ~QUEUE<T>() {
		delete[]v; v = NULL;
	};
};


//双向循环链表 (含有key)
template<class T,typename K>
class LIST {
public:
	struct N {
		K key;
		T data;
		N*next = NULL, *prev = NULL;
	};
private:
	//数据
	struct N *m_head = NULL;
	int m_size = 0;
public:
	//信息
	bool isempty() { if (m_size)return 0; return 1; };
	int size() { return m_size; };
	N*head() { return m_head; };
	void sethead(N*newhead) { 
		if (m_head == NULL) { 
			m_size = 1; 
			newhead->next = newhead; 
			newhead->prev = newhead;
		}; 
		m_head = newhead; 
	};
	//插入
	void insert(N *px, N *ppre = NULL) {
		//首节点
		if (ppre == NULL) {
			m_head = px;
		}
		//常规
		else {
			N *pnex = ppre->next;
			px->prev = ppre; ppre->next = px;
			px->next = pnex; pnex->prev = px;
		};
		m_size++;
		return;
	};
	//查找
	N*search(K key) {
		if (isempty())return NULL;
		N *cur = m_head;
		for (int i = 0; i < m_size; i++) {
			if (cur->key == key)return cur;
			cur = cur->next;
		};
		return NULL;
	};
	//删除
	void erase(N *px) {
		//最后一个节点
		if (m_size == 1) {
			delete m_head; m_head = NULL;
		}
		else {
			px->prev->next = px->next;
			px->next->prev = px->prev;
			if (px == m_head)m_head = px->next;
			delete px; px = NULL;
		};
		m_size--;
		return;
	};
	//构造/析构
	virtual ~LIST() {
		if (isempty())return;
		while (m_head)erase(m_head);
	};
};


//优先队列 (队列链表实现)
template<class T>
class PRIQUE {
public:
	using Q = QUEUE<T>;
	LIST<Q, int> list;
	using N = typename LIST<Q, int>::N;
	void push_queue(N*note, T &data) {
		note->data.push_end(data);
	};
	void push_list(T &data, int key) {
		N *temp = new N;
		temp->data.push_end(data); temp->key = key;
		if (list.isempty()) { list.sethead(temp); return; };
		int size = list.size();
		N *cur = list.head();
		int i; for (i = 0; i < size; i++) {
			if (key < cur->key)break;
			cur = cur->next;
		};
		list.insert(temp, cur->prev);
		if (i == 0 && cur == list.head())list.sethead(temp);
	};
	void push(T &data, int count) {
		N*s = list.search(count);
		if (s)push_queue(s, data);
		else push_list(data, count);
	};
	T pop(int *emptyflag) {
		T temp = { 0,0 }; *emptyflag = 0;
		if (list.isempty()) { *emptyflag = 1; return temp; };
		Q&q = list.head()->data;
		temp = q.head(); q.pop_head();
		if (q.isempty())list.erase(list.head());
		return temp;
	};
};


//解迷宫
class MAZE_SOLVE {
public:
	//数据
	struct D { int x, y; };
	struct MAZE {
		int m, n; D start, end;
		struct CELL {
			int ba_cost = 0, ba_tag = 0;//基础信息
			int bf_count = 0, bf_tag = 0, p_tag = 0;//bfs计数 bfs标记 寻找路径时的计数
		}**v = NULL;
	}maze;
	void setmazesizeandnew(int m, int n, int si, int sj, int ei, int ej) {
		maze.m = m; maze.n = n; maze.start = { sj,si }; maze.end = { ej, ei };
		maze.v = new MAZE::CELL*[m];
		for (int i = 0; i < m; i++)maze.v[i] = new MAZE::CELL[n];
	};
	void setmazecontent_single(int i, int j, int cost, int tag) {
		maze.v[i][j].ba_cost = cost; 
		maze.v[i][j].ba_tag = tag;
	};
	void delmaze() {
		int m = maze.m;
		for (int i = 0; i < m; i++)delete[]maze.v[i];
		delete maze.v; maze.v = NULL;
	};
	using T = D;
	//广度优先搜索涂色
	int bfsmain(bool &issolved) {
		auto around = [](int ord, T&cur)->T {static int offx[] = { 0,1,0,-1 }, offy[] = { -1,0,1,0 }; return{ offx[ord] + cur.x,offy[ord] + cur.y }; };
		auto checkandmark = [&](T&temp, int countcur)->bool {if (temp.x < 0 || temp.x >= maze.n || temp.y < 0 || temp.y >= maze.m)return 0;
		MAZE::CELL&cell = maze.v[temp.y][temp.x]; if (cell.ba_tag == 1 || cell.bf_tag == 1)return 0; cell.bf_tag = 1; cell.bf_count = countcur + cell.ba_cost; return 1; };
		auto isend = [&](T&temp)->bool {if (temp.x == maze.end.x&&temp.y == maze.end.y)return 1; return 0; };
		auto calcurcount = [&](T&cur) {MAZE::CELL&cell = maze.v[cur.y][cur.x]; return cell.bf_count; };
		issolved = 1;
		PRIQUE<T> ts; T cur, temp; int isempty = 0, curcount;
		cur = maze.start;//起点
		checkandmark(cur, -1);
		ts.push(cur, 0);
		while (1) {
			cur = ts.pop(&isempty); if (isempty)break;
			curcount = calcurcount(cur);
			for (int i = 0; i < 4; i++) {
				temp = around(i, cur);
				if (checkandmark(temp, curcount)) {
					if (isend(temp))return curcount + 1;
					ts.push(temp, curcount);
				};
			};
		};
		issolved = 0;
		return 0;
	};
	//计算最短路径
	int pathmain(bool &issolved) {
		auto searchthelargestcount = [&](T&max)->int {
			int isempty = 1;
			int maxc = -1, maxi, maxj;
			MAZE::CELL*temp = NULL;
			for (int i = 0; i < maze.m; i++)
				for (int j = 0; j < maze.n; j++) {
					temp = &maze.v[i][j];
					if (temp->bf_tag&&maxc < maze.v[i][j].bf_count) {
						if (isempty)isempty = 0;
						maxc = maze.v[i][j].bf_count; maxi = i; maxj = j;
					}
				}
			if (isempty)return 1;
			max = { maxj,maxi }; return 0;
		};
		auto isend = [&](T&temp)->bool {if (temp.x == maze.start.x&&temp.y == maze.start.y)return 1; return 0; };
		T max;
		if (issolved == 0) {
			int isempty = searchthelargestcount(max);
			if (isempty||isend(max))return 0;
		}
		else max = maze.end;
		auto mark = [&](T&temp) { maze.v[temp.y][temp.x].p_tag = 1; };
		auto around = [](int ord, T&cur)->T {static int offx[] = { 0,1,0,-1 }, offy[] = { -1,0,1,0 }; return{ offx[ord] + cur.x,offy[ord] + cur.y }; };
		auto check = [&](T&temp)->bool {if (temp.x < 0 || temp.x >= maze.n || temp.y < 0 || temp.y >= maze.m)return 0;
			MAZE::CELL&cell = maze.v[temp.y][temp.x]; if (cell.bf_tag == 0)return 0; return 1; };
		auto getcount = [&](T&temp)->int {return maze.v[temp.y][temp.x].bf_count; };
		auto choose = [&](T&cur)->T {
			T temp, minT; int f = 0, minc;
			for (int i = 0; i < 4; i++) {
				temp = around(i, cur);
				if (check(temp)) {
					if (f == 0) { f = 1; minc = getcount(temp); minT = temp; }
					if (minc > getcount(temp)) { minc = getcount(temp); minT = temp; }
				}
			}
			return minT;
		};
		issolved = 1;
		T cur = max;
		do {
			mark(cur);
			cur = choose(cur);
		} while (!isend(cur));
		return issolved;
	};
	//构造/析构
	~MAZE_SOLVE() {
		if (maze.v != NULL)delmaze();
	};
};


//迷宫
class MAZE_MAIN {
public:
	//数据
	struct D { int x, y; };
	struct MAZE {
		int m, n; D stars, end;
		int **v = NULL;
	}maze;
	//代称、字符、是否可走、cost
	//enum mazenum { 地面, 墙壁, 可破坏的墙壁1 };
#define chnum 3
	char se[2] = { 'r','a' };//起点和终点
	char ch[chnum] = { '.','#','x' };
	int tag[chnum] = { 0,1,0 };
	int cost[chnum] = { 1,0,1 + 1 };
	//创建、释放
	void setandnew(int m, int n) {
		maze.m = m; maze.n = n;
		maze.v = new int*[m];
		for (int i = 0; i < m; i++)maze.v[i] = new int[n];
	};
	void del() {
		int m = maze.m;
		for (int i = 0; i < m; i++)delete[]maze.v[i];
		delete maze.v; maze.v = NULL;
	};
	//从控制台输入迷宫
	void inputformscanf() {
		auto set = [&](int i, int j, char c) {
			for (int ci = 0; ci < chnum; ci++)
				if (ch[ci] == c) { maze.v[i][j] = ci; return; }
			maze.v[i][j] = 0;
			if (c == se[0])maze.stars = { j,i };
			if (c == se[1])maze.end = { j,i };
		};
		int m = maze.m, n = maze.n;
		char *temp = new char[n];
		for (int i = 0; i < m; i++) {
			scanf("%s", temp);
			for (int j = 0; j < n; j++) {
				set(i, j, temp[j]);
				temp[j] = 0;
			}
		}
		return;
	};
	//logistic生成迷宫
	void inputformlogistic(double a, double x0, double wall, double x, int si, int sj, int ei, int ej) {
		double xnext; int m = maze.m, n = maze.n;
		for (int i = 0; i < m; i++)for (int j = 0; j < n; j++) {
			xnext = x0*(1 - x0)*a;
			if (x0 > wall) maze.v[i][j] = 1;
			else if (x0 >= x)maze.v[i][j] = 2;
			else maze.v[i][j] = 0;
			x0 = xnext;
		}
		maze.stars = { sj,si }; maze.end = { ej,ei };
		return;
	};

	//解迷宫
	MAZE_SOLVE maze_solve;
	int solve_newsetandcalall(bool &issolved,bool &issolved2) {
		int m = maze.m, n = maze.n;
		maze_solve.setmazesizeandnew(m, n, maze.stars.y, maze.stars.x, maze.end.y, maze.end.x);
		for (int i = 0; i < m; i++)for (int j = 0; j < n; j++)
		maze_solve.setmazecontent_single(i, j, cost[maze.v[i][j]], tag[maze.v[i][j]]);
		int re = 0;
		re = maze_solve.bfsmain(issolved);
		issolved2 = issolved;
		maze_solve.pathmain(issolved2);
		return re;
	};
	void solve_del() {
		maze_solve.delmaze();
	};

	//打印

#undef chnum
};

//测试1 简单测试 从控制台输入 输出步数
int main() {
	int m, n;
	MAZE_MAIN maze;
	int re; bool iss1, iss2;
	while (~scanf("%d%d", &m, &n)) {
		maze.setandnew(m, n);
		maze.inputformscanf();
		re = maze.solve_newsetandcalall(iss1, iss2);
		if (iss1)printf("%d\n", re);
		else printf("Poor ANGEL has to stay in the prison all his life.\n");
		maze.solve_del();
		maze.del();
	};
	return 0;
}
