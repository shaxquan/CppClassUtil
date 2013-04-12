//Basic 3d Array class

#ifndef ARRAY3D_H
#define ARRAY3D_H

template <typename Datatype>
class Array3D
{
public:
	Array3D(int p_width, int p_height, int p_depth)
	{
		m_array = new Datatype[p_width * p_height * p_depth];

		m_width = p_width;
		m_height = p_height;
		m_depth = p_depth;
	}

	~Array3D()
	{
		if (m_array != 0)
		{
			delete [] m_array;
		}
		m_array = 0;
	}

	Datatype &Get(int p_x, int p_y, int p_z)
	{
		return m_array[(p_z * m_width * m_height) +
			(p_y * m_width) +
			p_x];
	}

	void Resize(int p_width, int p_height, int p_depth)
	{
        Datatype* newarray = new Datatype[ p_width * p_height * p_depth ];

        if( newarray == 0 )
            return;

        int x, y, z, t1, t2, t3, t4;

        // determine the minimum of all dimensions.
        int minx = (p_width < m_width ? p_width : m_width);
        int miny = (p_height < m_height ? p_height : m_height);
        int minz = (p_depth < m_depth ? p_depth : m_depth);

        for( z = 0; z < minz; z++ )
        {
            t1 = z * p_width * p_height;
            t2 = z * m_width * m_height;
            for( y = 0; y < miny; y++ )
            {
                t3 = y * p_width;
                t4 = y * m_width;
                for( x = 0; x < minx; x++ )
                {
                    newarray[ t1 + t3 + x ] = m_array[ t2 + t4 + x ];
                }
            }
        }

        if( m_array != 0 )
            delete[] m_array;

        m_array = newarray;
        m_width = p_width;
        m_height = p_height;
        m_depth = p_depth;
	}

	int Size()
	{
		return m_width * m_height * m_depth;
	}

	int Width()
	{
		return m_width;
	}

	int Height()
	{
		return m_height;
	}

	int Depth()
	{
		return m_depth;
	}

private:
	Datatype *m_array;
	int m_width;
	int m_height;
	int m_depth;
};

#endif