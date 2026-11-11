#include<iostream>
using namespace std;

template<typename T>
class Element {
public:
	int row;
	int column;
	T value;
	Element();
	void operator = (Element<T>& temp);
	int getIndex(int rows, int columns);
};

template<typename T>
Element<T>::Element() {
	row = 0;
	column = 0;
}

template<typename T>
void Element<T>::operator = (Element<T>& temp) {
	row = temp.row;
	column = temp.column;
	value = temp.value;
}

template<typename T>
int Element<T>::getIndex(int rows, int columns) {
	return (row - 1) * columns + column;
}

template<typename T>
class Matrix {
private:
	int rows;
	int columns;
	int number;
	Element<T>* matrix;
public:
	Matrix();
	void input();
	void create();
	void transpose();
	void invert();
	void add(Matrix<T>& temp);
	void multiply(Matrix<T>& temp);
	void output();
	~Matrix();
};

template<typename T>
Matrix<T>::Matrix() {
	rows = 0;
	columns = 0;
	number = 0;
	matrix = NULL;
}

template<typename T>
void Matrix<T>::input() {
	cin >> rows;
	cin >> columns;
	if (matrix != NULL) {
		delete[] matrix;
	}
	matrix = new Element<T>[rows * columns];
	int index = 0;
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			T temp;
			cin >> temp;
			if (temp != 0) {
				matrix[index].row = i;
				matrix[index].column = j;
				matrix[index].value = temp;
				index++;
			}
		}
	}
	number = index;
	Element<T>* new_matrix = new Element<T>[number + 1];
	for (int i = 0; i <= number - 1; i++) {
		new_matrix[i] = matrix[i];
	}
	delete[] matrix;
	matrix = new_matrix;
}

template<typename T>
void Matrix<T>::create() {
	cin >> rows >> columns >> number;
	matrix = new Element<T>[number + 1];
	int row = 0;
	int column = 0;
	T value;
	for (int i = 0; i <= number - 1; i++) {
		cin >> row >> column >> value;
		matrix[i].row = row;
		matrix[i].column = column;
		matrix[i].value = value;
	}
}

template<typename T>
void Matrix<T>::transpose() {
	int max_column = 0;
	for (int i = 0; i <= number - 1; i++) {
		if (matrix[i].column > max_column) {
			max_column = matrix[i].column;
		}
	}
	/*开始进行计数排序*/
	int* Count = new int[max_column + 1];
	Element<T>* Result = new Element<T>[number + 1];
	for (int i = 0; i <= max_column; i++) {
		Count[i] = 0;
	}
	for (int i = 0; i <= number - 1; i++) {
		Count[matrix[i].column] += 1;
	}
	for (int i = 1; i <= max_column; i++) {
		Count[i] += Count[i - 1];
	}
	for (int i = number - 1; i >= 0; i--) {
		Result[Count[matrix[i].column] - 1] = matrix[i];
		Count[matrix[i].column] -= 1;
	}
	/*对每个元素进行转置*/
	for (int i = 0; i <= number - 1; i++) {
		int temp = Result[i].row;
		Result[i].row = Result[i].column;
		Result[i].column = temp;
	}
	int temp = rows;
	rows = columns;
	columns = temp;
	/*删除无用的数据*/
	delete[] matrix;
	delete[] Count;
	matrix = Result;
}

template<typename T>
void Matrix<T>::invert() {
	int max_column = 0;
	for (int i = 0; i <= number - 1; i++) {
		if (matrix[i].column > max_column) {
			max_column = matrix[i].column;
		}
	}
	/*开始进行计数排序*/
	int* Count = new int[max_column + 1];
	Element<T>* Result = new Element<T>[number + 1];
	for (int i = 0; i <= max_column; i++) {
		Count[i] = 0;
	}
	for (int i = 0; i <= number - 1; i++) {
		Count[matrix[i].column] += 1;
	}
	for (int i = 1; i <= max_column; i++) {
		Count[i] += Count[i - 1];
	}
	for (int i = number - 1; i >= 0; i--) {
		Result[Count[matrix[i].column] - 1] = matrix[i];
		Count[matrix[i].column] -= 1;
	}
	/*删除无用的数据*/
	delete[] matrix;
	delete[] Count;
	matrix = Result;
}

template<typename T>
void Matrix<T>::add(Matrix<T>& temp) {
	if (rows != temp.rows || columns != temp.columns) {
		cout << -1 << endl;
		rows = temp.rows;
		columns = temp.columns;
		number = temp.number;
		Element<T>* new_matrix = new Element<T>[temp.number + 1];
		for (int i = 0; i <= temp.number - 1; i++) {
			new_matrix[i] = temp.matrix[i];
		}
		delete[] matrix;
		matrix = new_matrix;
		return;
	}
	int index = 0;
	int index1 = 0;
	int index2 = 0;
	Element<T>* new_matrix = new Element<T>[rows * columns];
	while (index1 != number && index2 != temp.number) {
		if (matrix[index1].getIndex(rows, columns) == temp.matrix[index2].getIndex(rows, columns)) {
			if ((matrix[index1].value + temp.matrix[index2].value) != 0) {
				new_matrix[index].value = matrix[index1].value + temp.matrix[index2].value;
				new_matrix[index].row = matrix[index1].row;
				new_matrix[index].column = matrix[index1].column;
				index++;
			}
			index1++;
			index2++;
		}
		else if (matrix[index1].getIndex(rows, columns) > temp.matrix[index2].getIndex(rows, columns)) {
			new_matrix[index] = temp.matrix[index2];
			index2++;
			index++;
		}
		else if (matrix[index1].getIndex(rows, columns) < temp.matrix[index2].getIndex(rows, columns)) {
			new_matrix[index] = matrix[index1];
			index1++;
			index++;
		}
	}
	if (index1 != number) {
		while (index1 != number) {
			new_matrix[index] = matrix[index1];
			index1++;
			index++;
		}
	}
	else if (index2 != temp.number) {
		while (index2 != temp.number) {
			new_matrix[index] = temp.matrix[index2];
			index2++;
			index++;
		}
	}
	number = index;
	Element<T>* result = new Element<T>[number + 1];
	for (int i = 0; i <= number - 1; i++) {
		result[i] = new_matrix[i];
	}
	delete[] matrix;
	delete[] new_matrix;
	matrix = result;
}

template<typename T>
void Matrix<T>::multiply(Matrix<T>& temp) {
	if (columns != temp.rows) {
		cout << -1 << endl;
		rows = temp.rows;
		columns = temp.columns;
		number = temp.number;
		Element<T>* new_matrix = new Element<T>[temp.number + 1];
		for (int i = 0; i <= temp.number - 1; i++) {
			new_matrix[i] = temp.matrix[i];
		}
		delete[] matrix;
		matrix = new_matrix;
		return;
	}
	temp.invert();                 //将矩阵temp变成列主表现形式
	int index = 0;
	Element<T>* new_matrix = new Element<T>[rows * temp.columns];
	int star1 = 0;
	int end1 = 0;
	int star2 = 0;
	int end2 = 0;
	for (int i = 1; i <= rows && end1 < number; i++) {
		star1 = end1;
		if (matrix[star1].row != i) {
			continue;
		}
		star2 = end2 = 0;
		for (int j = 1; j <= temp.columns && end2 < temp.number; j++) {
			star2 = end2;
			if (temp.matrix[star2].column != j) {
				continue;
			}
			/*如果if不成立，说明可以进行*/
			end1 = star1;
			int result = 0;
			while (end1 < number && end2 < temp.number && matrix[end1].row < i + 1 && temp.matrix[end2].column < j + 1) {
				if (matrix[end1].column == temp.matrix[end2].row) {
					result += matrix[end1].value * temp.matrix[end2].value;
					end1++;
					end2++;
				}
				else if (matrix[end1].column < temp.matrix[end2].row) {
					end1++;
				}
				else if (matrix[end1].column > temp.matrix[end2].row) {
					end2++;
				}
			}
			if (result != 0) {                        //将结果添加到数组的末尾
				new_matrix[index].row = i;
				new_matrix[index].column = j;
				new_matrix[index].value = result;
				index++;
			}
			/*要保证end2是在下一列元素的第一个*/
			while (end2 < temp.number && temp.matrix[end2].column < j + 1) {
				end2++;
			}
		}
		/*保证end1是在下一行元素的第一个*/
		while (end1 < number && matrix[end1].row < i + 1) {
			end1++;
		}
	}
	number = index;
	Element<T>* Result = new Element<T>[number + 1];
	for (int i = 0; i <= number - 1; i++) {
		Result[i] = new_matrix[i];
	}
	columns = temp.columns;
	delete[] new_matrix;
	delete[] matrix;
	matrix = Result;
}

template<typename T>
void Matrix<T>::output() {
	cout << rows << " " << columns << endl;
	int index = 0;
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			if (matrix[index].row == i && matrix[index].column == j) {
				cout << matrix[index].value << " ";
				index++;
			}
			else {
				cout << 0 << " ";
			}
		}
		cout << endl;
	}
}

template<typename T>
Matrix<T>::~Matrix() {
	delete[] matrix;
}

class Solution {
private:
	int operation_numbers;
	Matrix<int> matrix;
public:
	Solution();
	void operations();
};

Solution::Solution() {
	cin >> operation_numbers;
}

void Solution::operations() {
	int selection;
	for (int i = 1; i <= operation_numbers; i++) {
		cin >> selection;
		if (selection == 1) {
			matrix.input();
		}
		else if (selection == 2) {
			Matrix<int> temp;
			temp.create();
			matrix.multiply(temp);
		}
		else if (selection == 3) {
			Matrix<int> temp;
			temp.create();
			matrix.add(temp);
		}
		else if (selection == 4) {
			matrix.output();
		}
		else if (selection == 5) {
			matrix.transpose();
		}
	}
}

int main() {
	Solution solution;
	solution.operations();
	return 0;
}
