#include <iostream>
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
		//获得迭代器
		iterator getiterator() {
			return iterator(first);
		}
		//展示
		void show() {
			int i = 0;
			for (note *t = first; t != NULL; t = t->next, i++) {
				std::cout << "[" << i << "]:"; //元素下标
				std::cout << t->data << "\n";
			}
		}






};

class solution {
	public:
		int N, Q;
		void way() {
			line<int> list;
			std::cin >> N >> Q;
			int tin;//插入的数据
			int index;;//操作索引
			int operat;//操作类型
			for (int i = 0; i < N; i++) {
				std::cin >> tin;
				list.insert(tin);
				
			}


			for (int i = 0; i < Q; i++) {
				std::cout << "step" << i << ":\n";
				std::cin >> operat;
				if (operat == 1) {
					std::cin >> index >> tin;
					list.insert(tin, index);
				}
				if (operat == 2) {
					std::cin  >> tin;
					if (list.check(tin) == -1)
						std::cout << -1 << '\n';
					else
						list.del(list.check(tin));
				}
				if (operat == 3) {
					list.reverse();
				}
				if (operat == 4) {
					std::cin >> tin;
					std::cout << list.check(tin) << '\n';
				}
				if (operat == 5) {
					line<int>::iterator i = list.getiterator();
					int sum = 0, index = 0;
					for (; i.isend(); i++, index++) {
						sum += index ^ (*i);
						//	std::cout << (*i) << "<" << index << "\n";
					}
					std::cout << sum << '\n';

				}
				//	list.show();
			}
		}
};

int main() {
	solution s;
	s.way();

}
