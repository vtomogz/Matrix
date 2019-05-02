#pragma once
#include <iostream>
#include <vector>

template<class type>
class Matrix
{
public:
	Matrix() :
		m_rows(0),
		m_cols(0)
	{
	}

	Matrix(size_t t_rows, size_t t_cols) :
		m_rows(t_rows),
		m_cols(t_cols)
	{
		m_data.resize(t_rows * t_cols, 0);
	}

	~Matrix()
	{
	}

public:
	void Fill(type t_value)
	{
		for (size_t i = 0; i < m_data.size(); ++i) {
			m_data[i] = t_value;
		}
	}

	size_t getRows()
	{
		return m_rows;
	}

	size_t getCols()
	{
		return m_cols;
	}

	void Resize(size_t t_rows, size_t t_cols)
	{
		auto size = [](size_t x, size_t y) {if (x < y) return x; return y; };
		std::vector<type> old = m_data;
		m_data.clear();
		m_data.resize(t_rows * t_cols, 0);
		for (size_t i = 0; i < size(t_rows, m_rows); ++i) {
			for (size_t j = 0; j < size(t_cols, m_cols); ++j) {
				m_data[i * t_cols + j] = old[i * m_cols + j];
			}
		}
		m_rows = t_rows;
		m_cols = t_cols;
	}

	void Transpose()
	{
		std::vector<type> old = m_data;
		for (size_t i = 0; i < m_rows; ++i) {
			for (size_t j = 0; j < m_cols; ++j) {
				m_data[j * m_rows + i] = old[i * m_cols + j];
			}
		}
		std::swap(m_rows, m_cols);
	}
private:
	class Row
	{
	public:
		Row(type* t_row) :
			m_row(t_row)
		{
		}
		type& operator[](size_t t_index)
		{
			m_row += t_index;
			return *m_row;
		}
	private:
		type *m_row;
	};
public:
	Row operator[] (size_t t_index)
	{
		return Row(&m_data[t_index*m_cols]);
	}

	Matrix& operator+= (Matrix& t_matrix)
	{
		if (m_rows == t_matrix.getRows() && m_cols == t_matrix.getCols()) {
			for (size_t i = 0; i < m_rows; ++i) {
				for (size_t j = 0; j < m_cols; ++j) {
					m_data[i*m_cols + j] += t_matrix[i][j];
				}
			}
		}
		else {
			std::cout << "SHAME!" << std::endl;
		}
		return *this;
	}

	friend Matrix operator+ (Matrix t_matrix1, Matrix& t_matrix2)
	{
		if (t_matrix1.getRows() == t_matrix2.getRows() && t_matrix1.getCols() == t_matrix2.getCols()) {
			t_matrix1 += t_matrix2;
		}
		else {
			std::cout << "SHAME!" << std::endl;
		}
		return t_matrix1;
	}

	Matrix& operator*= (double t_scale)
	{
		for (size_t i = 0; i < m_data.size(); ++i) {
			m_data[i] *= (type)t_scale;
		}
		return *this;
	}

	friend Matrix operator* (Matrix t_matrix, double t_scale)
	{
		t_matrix *= t_scale;
		return t_matrix;
	}

	Matrix& operator*= (Matrix& t_matrix)
	{
		*this = *this * t_matrix;
		return *this;
	}

	friend Matrix operator* (Matrix& t_matrix1, Matrix& t_matrix2)
	{
		if (t_matrix1.getCols() == t_matrix2.getRows()) {
			Matrix m;
			m.Resize(t_matrix1.getRows(), t_matrix2.getCols());
			for (size_t i = 0; i < t_matrix1.getRows(); ++i) {
				for (size_t j = 0; j < t_matrix1.getCols(); ++j) {
					for (size_t k = 0; k < t_matrix1.getCols(); ++k) {
						m[i][j] += t_matrix1[i][k] * t_matrix2[k][j];
					}
				}
			}
			return m;
		}
		else {
			std::cout << "SHAME!" << std::endl;
		}
	}
private:
	size_t m_rows;
	size_t m_cols;
	std::vector<type> m_data;
};