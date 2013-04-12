//2D Array class
#ifndef ARRAY2D_H
#define ARRAY2D_H

template <typename Datatype>
class Array2D
{
public:
	Array2D(int p_width, int p_height)
	{
		m_array = new Datatype[p_width * p_height];
		m_width = p_width;
		m_height = p_height;
	}

	~Array2D()
	{
		if (m_array != 0)
		{
			delete [] m_array;
		}
		m_array = 0;
	}

	Datatype &Get(int p_x, int p_y)
	{
		return m_array[p_y * m_width + p_x];
	}

	void Resize(int p_width, int p_height)
	{
		Datatype *newArray = new Datatype[p_width * p_height];
		if (newArray == 0)
		{
			return;
		}

		int x, y, t1, t2;

		//determine the min
		int minX = (p_width < m_width ? p_width : m_width);
		int minY = (p_height < m_height ? p_height : m_height);

		for (y=0; y<minY; y++)
		{
			t1 = y * p_width;
			t2 = y * m_width;
			for (x = 0; x < minX; x++)
			{
				newArray[t1 + x] = m_array[t2 + x];
			}
		}

		if (m_array != 0)
		{
			delete [] m_array;
		}
		m_array = newArray;
		m_width = p_width;
		m_height = p_height;
	}

	int Size()
	{
		return m_width * m_height;
	}

	int Width()
	{
		return m_width;
	}

	int Height()
	{
		return m_height;
	}

	

private:
	Datatype *m_array;
	int m_width;
	int m_height;
};

#endif