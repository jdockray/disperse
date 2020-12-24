
#ifndef DISPERSE_MATRICS
#define DISPERSE_MATRICS

class SparseMatrix
{
public:
	SparseMatrix(unsigned int rows, unsigned int columns)
		: numberOfRows(rows), numberOfColumns(columns)
	{
	}

	double getValue(unsigned int row, unsigned int column) const
	{
		auto result	= elements.find(std::pair<unsigned int, unsigned int>(column, row));
		return result == elements.end() ? 0 : result->second;
	}

	void setValue(unsigned int row, unsigned int column, double value)
	{
		if (column >= numberOfColumns || row >= numberOfRows)
		{
			throw UnexpectedException();
		}
		const std::pair<unsigned int, unsigned int> key(column, row);
		if (value == 0)
		{
			elements.erase(key);
		}
		else
		{
			elements[key] = value;
		}
	}

	unsigned int columnCount() const
	{
		return numberOfColumns;
	}

	unsigned int rowCount() const
	{
		return numberOfRows;
	}

	const std::map<std::pair<unsigned int, unsigned int>, double>& matrixElements() const
	{
		return elements;
	}

	SparseMatrix getTranspose() const
	{
		SparseMatrix transpose(columnCount(), rowCount());
		for (const auto& element : matrixElements())
		{
			transpose.setValue(element.first.second, element.first.first, element.second);
		}
		return transpose;
	}

private:
	const unsigned int numberOfColumns;
	const unsigned int numberOfRows;
	std::map<std::pair<unsigned int, unsigned int>, double> elements;
};

class DiagonalSparseMatrix : public UpperTriangularSparseMatrix
{
public:
	DiagonalSparseMatrix(unsigned int dimension)
		: UpperTriangularSparseMatrix(dimension)
	{
	}

	DiagonalSparseMatrix(const std::vector<double>& diagonalValues)
		: DiagonalSparseMatrix(diagonalValues.size())
	{
		for (unsigned int i = 0; i < diagonalValues.size(); ++i)
		{
			UpperTriangularSparseMatrix::setValue(i, i, diagonalValues.at(i));
		}
	}

	void setValue(unsigned int diagonalPosition, double value)
	{
		UpperTriangularSparseMatrix::setValue(diagonalPosition, diagonalPosition, value);
	}

	void setValue(unsigned int row, unsigned int column, double value)
	{
		if (row != column)
		{
			throw UnexpectedException();
		}
		setValue(row, value);
	}
};

template<typename T>
T multiply(SparseMatrix a, SparseMatrix b)
{
	std::map<unsigned int, std::map<unsigned int, double> > matrixBLookup;
	for (auto const& elementOfB : b.matrixElements())
	{
		auto result = matrixBLookup.find(elementOfB.first.first);
		if (result == matrixBLookup.end())
		{
			matrixBLookup[elementOfB.first.first] = { {elementOfB.first.second, elementOfB.second} };
		}
		else
		{
			result->second.insert({ {elementOfB.first.second, elementOfB.second} });
		}
	}
	T product(a.columnCount(), b.rowCount());
	for (auto const& elementOfA : a.matrixElements())
	{
		auto const& matrixBRow = matrixBLookup.find(elementOfA.first.first);
		if (matrixBRow != matrixBLookup.end())
		{
			for (auto const& elementOfB : matrixBRow->second)
			{
				product.setValue(elementOfA.first.first, elementOfB.first, product.getValue(elementOfA.first.first, elementOfB.first) + elementOfA.second * elementOfB.second);
			}
		}
	}
	return product;
}

class UpperTriangularSparseMatrix : public SparseMatrix
{
public:
	UpperTriangularSparseMatrix(unsigned int dimension)
		: SparseMatrix(dimension, dimension)
	{
	}

	UpperTriangularSparseMatrix(const SparseMatrix& squareMatrix)
		: SparseMatrix(squareMatrix.rowCount(), squareMatrix.rowCount())
	{
		if (rowCount() != columnCount())
		{
			throw UnexpectedException();
		}
		for (const auto& element : squareMatrix.matrixElements())
		{
			if (element.first.first >= element.first.second)
			{
				setValue(element.first.second, element.first.first, element.second);
			}
		}
	}

	void setValue(unsigned int row, unsigned int column, double value)
	{
		if (row > column)
		{
			throw UnexpectedException();
		}
		SparseMatrix::setValue(row, column, value);
	}
};

#endif // #ifndef DISPERSE_MATRICS