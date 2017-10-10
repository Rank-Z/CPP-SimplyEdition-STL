#include<initializer_list>//ֵ�б���
#include<memory>//allocator��	
#include<stdexcept>//���ж����˼��ֳ������쳣��
#include<utility>//std::move() std::forward()

class vector
{

	using reference = int & ;
	using const_reference = const int &;
	using iterator = int * ;
	using const_iterator = const int *;
	using size_type = unsigned ;//���� size_t
	using difference_type = int;
	using value_type = int;
	using allocator_type = std::allocator<int>;
	using pointer = int *;
	using const_pointer=const int *;

private:
	iterator elem = nullptr;//�׵�ַ				==beign()
	iterator space = nullptr;//δʹ�ÿռ��׵�ַ	==end()
	iterator last = nullptr;//�ܹ���ַβ���ַ
	allocator_type alloc;//�ڴ�������

public:
	iterator& _first() { return elem; }
	iterator& _end() { return space; }
	iterator& _last() { return last; }

public:
	vector()
	{
		elem=alloc.allocate(4);
		space = elem;
		last = elem + 4;
	}

	vector(int s) :vector(s,0)//ί�и� vector(int s,int value)�������Ϊ0
	{ }

	vector(int s, int value) //���Ϊvalue
	{
		elem = alloc.allocate(s);
		std::uninitialized_fill_n(elem, s, value);
		space = last = elem + s;
	}
	//TODO:��������Ƿ��Ч����ȫ������
	vector(const int *b,const int *c) //why const ? 
	{
		difference_type p_diff = c - b;
		elem = alloc.allocate(p_diff);
		space = last = elem + p_diff;
		for (int i = 0; elem + i != last; ++i)
		{
			*(elem + i) = *(b + i);
		}
	}

	vector(std::initializer_list<int> &il) :vector(il.begin(),il.end())//ֵ�б� ֻ����constָ��
	{ }

	vector(vector& v):vector(v.cbegin(),v.cend())
	{ }

	~vector()
	{
		for (auto p = elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
		alloc.deallocate(elem, last - elem);
	}

	//����Ϊ���ƺ���
	/***********************************************************************************************************************************************************/
	//���ܺ���


	//��������ָ��********************************************************************************

	iterator begin()
	{
		return elem;
	}

	const iterator cbegin()
	{
		return elem;
	}

	iterator end()
	{
		return space;
	}

	const iterator cend()
	{
		return space;
	}
	//TODO:���������

	//��С������*************************************************************************************

	size_type size()
	{
		return static_cast<unsigned>(space - elem);
	}

	bool empty()
	{
		return (size() == 0);
	}

	size_type capacity()
	{
		return static_cast<unsigned>(last - elem);
	}

	void reserve(int n)
	{
		auto p = alloc.allocate((last - elem) + n);
		for (int i = 0; elem + i != space; ++i)
		{
			*(p + i) = *(elem + i);
		}
		for (auto it = elem; it != space; ++it)
		{
			alloc.destroy(it);//����������ÿ��Ԫ��
		}
		alloc.deallocate(elem, (last - elem));
		space = p + (space - elem);
		last = p + (last - elem)+n;
		elem = p;
	}

	void resize(int n)
	{
		value_type v;
		resize(n, v);// !!! Some Complier don't allow to do this
	}

	void resize(int n, value_type value)
	{
		if (n <= (last - elem))//n��ԭ�ռ�С
		{
			for (int i = n; elem + i != last; ++i)
			{
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem + n, (last - elem) - n);// !!!
			(space - elem) <= n ? last = elem + n : last = space = elem + n;
			for ( ; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
		else//n��ԭ�ռ��
		{
			auto p = alloc.allocate(n);
			for (int i = 0; elem + i != space; ++i)
			{
				*(p + i) = *(elem + i);
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem, (last - elem));
			last = p + n;
			space = p + (space - elem);
			elem = p;
			for (; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
	}

	void shrink_to_fit()
	{
		if (space != last)
		{
			alloc.deallocate(space, last - space);
			last = space;
		}
	}

	void clear()
	{
		for ( iterator p=elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
	}

	//Ԫ�ط���**************************************************************************************

	value_type& front()
	{
		return *elem;
	}

	value_type& back()
	{
		return *(space - 1);
	}

	value_type& operator[](int n)//����û�н���������Ϊ�޷������������ṩ�ޱ߽���ķ���
	{
		return *(elem + n);
	}

	value_type& at(int n)
	{
		if (n<0 || n>(space - elem))
			throw std::out_of_range("vector out of range");
		else
			return *(elem + n);
	}

	//ջ����*****************************************************************************************

	void push_back(value_type v)
	{
		if (!(last - space))
		{
			reserve(last - elem);//����Ϊԭ����С��������C++��׼�в�û�жԴ�����ȷ�涨�����Ǵ󲿷�C++ʵ�ֶ������
		}
		*space = v;
		++space;
	}

	void pop_back()
	{
		alloc.destroy(space);
		--space;
	}

	template<typename ...Args>
	void emplace_back(Args&&...args)
	{
		if (last - space == 0)
		{
			reserve(last - elem);
		}
		alloc.construct(space++, std::forward<Args>(args)...);
	}

	//�б����*********************************************************************************************

	iterator insert(iterator position, value_type &&v)//����ʹ���ƶ�����
	{
		if (last - space)
		{
			auto p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + 1, *p);
				alloc.destroy(p);
			}
			alloc.construct(position, std::move(v));
			last++;
			return position;
		}
		else
		{
			auto p = alloc.allocate(2 * (last - elem));
			int dif = position - elem;
			for (int i = 0; i!=dif; ++i)
			{
				alloc.construct(p + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			alloc.construct(p + (position - elem), std::forward<value_type>(v));
			for (; position != space; ++position)
			{
				alloc.construct(p + 1 + (position - elem), *(position));
				alloc.destroy(position);
			}
			alloc.deallocate(elem, last - elem);
			last = p + 2 * (last - elem);
			space = p + 1 + (space - elem);
			elem = p;
			return p + dif;
		}
	}

	iterator insert(iterator position, int n, value_type v)//����ʹ���ƶ������ܶ�ͬһ�����ƶ����Σ�����Ҫ������������
	{
		if (last - space >= n)
		{
			auto p = space - 1;
			for ( ; p >=position; --p)
			{
				alloc.construct(p + n, *p);
				alloc.destroy(p);
			}
			for (int i = 0; i != n; ++n)
			{
				alloc.construct(position+i, v);
			}
			last += n;
			return position;
		}
		else
		{
			int rsize = 2*(last - elem);
			int dist = position - elem;
			while (rsize < n+(space-elem))
			{
				rsize *= 2;
			}
			auto tp = alloc.allocate(rsize);
			for (int i=0; elem + i != position; ++i)
			{
				alloc.construct(tp + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			for (int i = 0; i != n; ++i)
			{
				alloc.construct(tp + dist + i, v);
			}
			for ( ; position != space; ++position)
			{
				alloc.construct(tp + (position - elem) + n, *(position));
				alloc.destroy(position);
			}
			alloc.deallocate(elem, space - elem);
			last = tp + rsize;
			space = tp + n + (last - elem);
			elem = tp;
			return tp + dist;
		}
	}

	iterator insert(iterator position, iterator  begin, iterator  end)
	{
		int size = end-begin;
		if (last - space >= size)
		{
			auto p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + size, *p);
				alloc.destroy(p);
			}
			for (; begin != end; ++begin)
			{
				alloc.construct(position++, *begin);
			}
			return p + 1;
		}
		else
		{
			int dif = position - elem;
			int rsize = 2 * (last - elem);
			while (rsize < size + (space - elem))
			{
				rsize *= 2;
			}
			auto p = alloc.allocate(rsize);
			for (int i = 0; elem + i != position; ++i)
			{
				alloc.construct(p + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			for (int i = position - elem; begin != end; ++begin,++i)
			{
				alloc.construct(p + i, *(begin));
			}
			for (; position != space; ++position)
			{
				alloc.construct(p + (position - elem) + size);
				alloc.destroy(position);
			}
			alloc.deallocate(elem, last - elem);
			space = p + rsize;
			last = p + (last - elem) + size;
			elem = p;
			return elem + dif;
		}
	}

	iterator insert(iterator position, std::initializer_list<int> li)
	{
		return insert(position, (iterator)li.begin(), (iterator)li.end());
	}

	iterator earse(iterator p)
	{
		return earse(p, p+1);
	}

	iterator earse(iterator first, iterator last)
	{
		for (int i = 0; first + i != last; ++i)
		{
			alloc.destroy(first + i);
		}
		for (int i = 0; last + i != space; ++i)
		{
			alloc.construct(first + i, *(last + i));
			alloc.destroy(last + i);
		}
		return first;
	}

	//�Ƚ��뽻��*********************************************************************************

	bool operator==(vector& v)
	{
		if (size() != v.size())return false;

		for (int i = 0; elem + i != space; ++i)
		{
			if (*(elem + i) != v[i])
				return false;
		}
		return true;
	}

	bool operator!=(vector& v)
	{
		return !operator==(v);
	}

	bool operator<(vector& v)
	{
		int min_size = size() < v.size() ? size() : v.size();
		for (int i = 0; i != min_size; ++i)
		{
			if (*(elem + i) == v[i])continue;
			if (*(elem + i) < v[i])
				return true;
			else
				return false;
		}
		return size() < v.size();
	}

	bool operator<=(vector& v)
	{
		return !(v < *(this));
	}

	bool operator>(vector& v)
	{
		return v <*(this);
	}

	bool operator>=(vector& v)
	{
		return !(operator<(v));
	}

	void swap(vector& v)
	{
		iterator t = _first();
		_first() = v._first();
		v._first() = t;
		
		t = _end();
		_end() = v._end();
		v._end() = t;

		t = _last();
		_last() = v._last();
		v._last() = t;
	}

	friend void swap(vector&v1, vector&v2);

};

void swap(vector &v1, vector& v2)
{
	v1.swap(v2);
}