
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

	void setValue(unsigned int column, unsigned int row, double value)
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

private:
	const unsigned int numberOfColumns;
	const unsigned int numberOfRows;
	std::map<std::pair<unsigned int, unsigned int>, double> elements;
};

class UpperTriangularSparseMatrix : public SparseMatrix
{
public:
	UpperTriangularSparseMatrix(unsigned int dimension)
		: SparseMatrix(dimension, dimension)
	{
	}

	void setValue(unsigned int column, unsigned int row, double value)
	{
		if (row > column)
		{
			throw UnexpectedException();
		}
		SparseMatrix::setValue(column, row, value);
	}
};

#endif // #ifndef DISPERSE_MATRICS