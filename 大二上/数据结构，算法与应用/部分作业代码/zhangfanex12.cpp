#include <iostream>
#include <stdio.h>
template <class T>

class line {
	public:
		//节点类
		class note {
			public:
				T data;
				note *next;
		};
		//迭代器类
		class iterator {
			public:
				note *there;//节点指针
				iterator(note *p) {
					there = p;
				}
				iterator() {
					there = NULL;
				}
				//解引用
				T &operator*()const {
					return there->data;
				}
				//访问
				T *operator->()const {
					return &there->data;
				}

				//自增
				iterator &operator++() {
					if (there != NULL) {
						there = there->next;
						return *this;
					} else
						throw "pin wrong";
				}
				//自增
				iterator &operator++(int) {
					if (there != NULL) {
						there = there->next;
						return *this;
					} else
						throw "pin wrong";
				}
				//判断是否到达尾部
				int isend() {
					if (there == NULL)
						return 0;
					else
						return 1;
				}
				//判断相等
				int operator ==(iterator i) {
					if (there != NULL && there == i.there)
						return 1;
					else
						return 0;
				}
		};
		//属性区
		note *first;//指向头节点
		int listsize;//链表大小
		//方法区
		//构造方法
		line() {
			listsize = 0;
			first = NULL;
		}
		//---------P124_T3_set()---------
		//复杂度为O(index)
		void set(int index, T element) {
			if (index >= listsize)//下标检查
				throw "index is too big.";
			note *point = first;
			for (int i = 0; i < index ; i++)
				point = point->next;
			point->data = element;
		}
		//--------P125_T15_reverse()---------
		//复制度为O(listsize)
		void reverse() {
			note *re = first; //待反转节点
			note *obj = NULL; //反转后的目标节点
			note *member = first; //记忆指针
			//member的初值很重要，因为第一个反转first所有不能改变re，
			//且member不能为空，否则进不了循环
			//如果member初值为空可以用do结构完成循环
			while (member != NULL) {
				if (member != NULL)
					re = member;//更新反转节点
				member = re->next; //记住下一个反转的节点
				re->next = obj; //反转
				obj = re; //更新目标
			}
			//更新首节点
			first = re;
		}
		//---------P125_T17_melt()---------
		//复制度为O(a.listsize+b.listsize)
		//静态函数
		static line melt(line a, line b) {
			line re;//result结果
			note *pa = a.first, *pb = b.first; //指向a和b节点的指针
			while (pa != NULL || pb != NULL) {
				//交替插入，如果一个为空则不插，循环条件为或
				//这样保证再两个线性表都非空是交通插入
				//而一个空了时保证另一个能被插玩

				if (pa != NULL)
					re.insert(pa->data), pa = pa->next;
				if (pb != NULL)
					re.insert(pb->data), pb = pb->next;

			}

			return re;
		}
		//插入从尾部
		void insert(T data) {
			if (first == NULL) {
				first = new note;
				first->next = NULL;
				first->data = data;
			} else {

				note *t = first; //临时指针
				//找到末尾节点
				for (; t->next != NULL; t = t->next) {}

				t->next = new note;
				t->next->next = NULL;
				t->next->data = data;
			}

			listsize++;

		}
		//指定位置插入
		void insert(T data, int index) {
			if (index > listsize || index < 0)
				throw"index wrong";
			note *t, *t2; //临时变量
			if (index == 0) {
				t = new note;
				t->data = data;
				t->next = first;
				first = t;
			} else {
				int i;//循环变量
				t = first;
				for (i = 0; i < index - 1; i++)
					t = t->next;
				t2 = t->next;
				t->next = new note;
				t = t->next;
				t->data = data;
				t->next = t2;

			}

			listsize++;

		}
		//查找
		int check(T data) {
			int i = 0;
			note *pin = first;
			for (; pin != NULL; pin = pin->next) {
				if (pin->data == data)
					return i;
				else
					i++;
			}
			return -1;

		}
		//删除
		void del(int index) {
			if (index >= listsize)
				throw "index is too big.";
			note *delet;//待删除的元素指针
			note *point = first; //前一个指针
			if (index == 0) {
				delet = first;
				first = first->next;
			} else {
				for (int i = 0; i < index - 1; i++)
					point = point->next;
				delet = point->next;
				point->next = delet->next;
			}
			delete delet;
			listsize--;
		}
		//删除元素
		int ddel(T data) {
			note *pin = first, *fpin = NULL;
			for (; pin != NULL; fpin = pin, pin = pin->next) {
				if (pin->data == data) {

					if (fpin == NULL)
						first = first->next;
					else
						fpin->next = pin->next;
					delete pin;
					listsize--;
					return 1;
				}
			}
			return -1;
		}
		//获得迭代器
		iterator getiterator() {
			return iterator(first);
		}
		//展示
		void show() {
			int i = 0;
			for (note *t = first; t != NULL; t = t->next, i++) {
				std::cout << "[" << i << "]:"; //元素下标
				std::cout << t->data << " ";
			}
		}
		//插入到合适位置
		void insertsort(T data) {
			note *pin = first;
			if (first == 0)
				this->insert(data);
			else if (data <= first->data) {
				this->insert(data, 0);
			} else {
				for (; pin->next != NULL && pin->next->data < data; pin = pin->next) {}
				note *newnote = new note;
				newnote->data = data;
				newnote->next = pin->next;
				pin->next = newnote;
				listsize++;
			}

			std::cout << '[' << listsize << ']';
		}
};


template<class T>

class line1 {
	public:
		T *data;
		int arraysize;
		int first;
		int end;

		line1 () {
			arraysize = 500;
			first = 0;
			end = 0;
			data = new T[arraysize];
		}
		void changesize(int newsize) {
			T *pin = new T[newsize];
			int pos = (first + 1) % arraysize;
			int i = 1;
			while (1) {
				pin[i] = data[pos];
				if (pos == end) {
					break;
				}
				pos = (1 + pos) % arraysize;
				i++;

			}
			data = pin;
			arraysize = newsize;
			first = 0;
			end = i;
		}
		T &front() {
			return data[(first + 1) % arraysize];
		}
		T &back() {
			return data[end];
		}
		void pop() {
			data[(first + 1) % arraysize].~T();
			first = (first + 1 ) % arraysize;
		}
		void push(T e) {
			if ((end + 1) % arraysize == first)
				changesize(2 * arraysize);
			end = (end + 1) % arraysize;
			data[end] = e;
		}
		int getsize() {
			return  (end - first + arraysize) % arraysize;
		}
		void show() {
			std::cout << first << ',' << end << '\n';
			for (int i = 0; i < arraysize; i++) {
				std::cout << data[i] << ' ';
			}
		}

};

class graph {
	public:
		int  size;
		line<int> *data;
		int *list;
		int *get;
		int listsize;
		graph(int n) {
			size = n;
			data = new line<int>[n + 1];
			list = new int[n + 1];
			get = new int[n + 1];
		}
		void  insert(int a, int b) {

			if (data[a].check(b) != -1)
				return;
			data[a].insertsort(b);
			data[b].insertsort(a);
			return ;
		}
		void del(int a, int b) {

			data[a].ddel(b);
			data[b].ddel(a);

		}
		void show() {
			int i;
			for (i = 1; i <= size; i++) {
				std::cout << "{" << i << "}" << "==";
				data[i].show();
				std::cout << '\n';
			}

		}
		void showl() {
			for (int i = 1; i <= listsize; i++) {
				std::cout << list[i] << ' ';
			}

		}
		void dfs(int n) {
			for (int i = 1; i <= size; i++)
				get[i] = 0;
			listsize = 0;
			dfsp(n);

		}
		void dfsp(int n) {
			listsize++;
			list[listsize] = n;
			get[n] = 1;
			line<int>::note *nod = data[n].first;
			while (nod != NULL) {
				if (get[nod->data] == 0)
					dfsp(nod->data);
				nod = nod->next;
			}
		}
		void dfst(int n, int t) {

			dfspt(n, t);

		}
		void dfspt(int n, int t) {
			get[n] = t;
			line<int>::note *nod = data[n].first;
			while (nod != NULL) {
				if (get[nod->data] == 0)
					dfspt(nod->data, t);
				nod = nod->next;
			}
		}

		void bfs(int n) {
			for (int i = 1; i <= size; i++)
				get[i] = 0;
			listsize = 0;
			line1<int> l;
			l.push(n);
			get[n] = 1;
			line<int>::note *nod;
			while (l.getsize() != 0) {
				n = l.front();
				listsize++;
				list[listsize] = n;

				l.pop();
				nod = data[n].first;
				while (nod != NULL) {
					if (get[nod->data] == 0)
						l.push(nod->data), get[nod->data] = 1;
					nod = nod->next;
				}
			}
		}
		void bfst(int n, int t) {
			line1<int> l;
			l.push(n);
			get[n] = t;
			while (l.getsize() != 0) {
				n = l.front();
				l.pop();
				line<int>::note *nod = data[n].first;
				while (nod != NULL) {
					if (get[nod->data] == 0)
						l.push(nod->data), get[nod->data] = t;
					nod = nod->next;
				}
			}
		}
		struct st {
			int node, cost;
		};
		int road(int s, int t) {
			for (int i = 1; i <= size; i++)
				get[i] = 0;
			line1<st> l;
			st a, b;
			a.node = s, a.cost = 1;
			l.push(a);
			while (l.getsize() != 0) {
				a = l.front();
				l.pop();

				line<int>::note *nod = data[a.node].first;
				while (nod != NULL) {
					if (nod->data == t) {
						return a.cost;
					}
					if (get[nod->data] == 0)
						b.node = nod->data, b.cost = a.cost + 1, l.push(b), get[nod->data] = 1;

					nod = nod->next;
				}



			}


		}

};

class solution {
	public:
		void begin() {
			int m, n, s, t, o, a, b;
			std::cin >> n >> m >> s >> t;
			line<int>::note *nod;
			line<int> l;
			int sum;
			graph g(n);
			for (int i = 0; i < m; i++) {
				scanf("%d %d %d", &o, &a, &b);
				if (o == 0)
					g.insert(a, b);
				else
					g.del(a, b);
			}
			//	g.show();

			g.bfs(1);
			for (int i = 2; i <= n; i++) {
				if (g.get[i] == 0)
					g.dfst(i, i);
			}
			int flag = (g.get[s] == g.get[t]);

			sum = 1, a = 1;
			for (int i = 1; i <= n; i++)
				if (g.get[i] > a)
					sum++, a = g.get[i];
			a = 0;
			std::cout<<sum<<'\n';
			for (int i = 1; i <= n; i++)
				if (g.get[i] > a)
					a = g.get[i], std::cout << a << ' ';


			g.dfs(s);
			std::cout << "\n" << g.listsize << '\n';
			g.showl();
			g.bfs(t);
			std::cout << "\n" << g.listsize << '\n';
			g.showl();
			std::cout << '\n';
			if (flag == 0)
				std::cout << -1;
			else
				std::cout << g.road(s, t);
		}
};

int main() {
//	graph g(8);
//	g.insert(1, 4) ;
//	g.insert(1, 2) ;
//	g.insert(1, 7) ;
//	g.insert(4, 2) ;
//	g.insert(5, 2) ;
//	g.insert(5, 3) ;
//	g.insert(6, 4) ;
//	g.insert(8, 6) ;
//	g.insert(8, 7) ;
//	g.bfs(1);
//
//	g.showl();
	solution s;
	s.begin();
//	line<int> l;
//	l.insertsort(1);
//	l.insertsort(2);
//	std::cout << l.ddel(3) << '\n';
//
//	l.show();

}
