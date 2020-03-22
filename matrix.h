#pragma once
#include <iostream>

template<typename T>
class Matrix
{
public:
	Matrix(int rows, int cols) : m_iRows(rows), m_iCols(cols) {
		m_tMatrix = creatMatrix(rows, cols);};
	Matrix(int rows, int cols, T** values);
	Matrix(const Matrix&);
	~Matrix() { deleteMatrix(m_iRows, m_iCols, m_tMatrix); }

	int rows() { return m_iRows; }
	int cols() { return m_iCols; }
	void set(int i, int j, T& value) { m_tMatrix[i][j] = value; }
	int get(int i, int j) { return m_tMatrix[i][j]; }

	void print() const;

protected:
	static T** creatMatrix(int, int);
	static void deleteMatrix(int, int, T**);

	int m_iRows, m_iCols;
	T** m_tMatrix;
};

template<typename T>
T** Matrix<T>::creatMatrix(int rows, int cols)
{
	T** m = new T * [m_iRows];
	for (int i = 0; i < m_iRows; i++)
		m[i] = new T[m_iCols];
	return m;
}

template<typename T>
void Matrix<T>::deleteMatrix(int rows, int cols, T** m)
{
	for (int i = 0; i < m_iRows; i++) delete m[i];
	delete m;
}

template<typename T>
Matrix<T>::Matrix(int rows, int cols, T** values) : 
	m_iRows(rows), m_iCols(cols) 
{
	m_tMatrix = creatMatrix(rows, cols);
	for (int i = 0; i < m_iRows; i++)
		std::memcpy(m_tMatrix[i], values + i * cols, cols * sizeof(T));
}

template<typename T>
Matrix<T>::Matrix(const Matrix& m)
{
	int rows = m.rows(); int cols = m.cols();
	m_tMatrix = creatMatrix(rows, cols);
	for (int i = 0; i < rows; i++)
		std::memcpy(m_tMatrix[i], m[i], cols * sizeof(T));
}

template<typename T>
void Matrix<T>::print() const
{
	for (int i = 0; i < m_iRows; i++) {
		T* rowSlice = m_tMatrix[i];
		for (int j = 0; i < m_iCols; i++) {
			std::cout << rowSlice[j] << " ";
		}
		std::cout << std::endl;
	}
}