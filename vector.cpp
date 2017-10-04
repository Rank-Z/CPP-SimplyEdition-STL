#include<initializer_list>//ֵ�б���
#include<memory>//allocator��	
#include<stdexcept>//���ж����˼��ֳ������쳣��


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

private:
	int *elem = nullptr;//ͷ		beign()
	int *space = nullptr;//δʹ�ÿռ��׵�ַ	end()
	int *last = nullptr;//�ܹ���ַβ���ַ
	allocator_type alloc;//�ڴ�������

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

	vector(std::initializer_list<int> &il) :vector(il.begin(),il.end())//ֵ�б� ����constָ��
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


	//��������ָ��
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

	//��С������
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
		space = p + (space - elem);
		last = p + (space - elem);
		elem = p;
	}

	void resize(int n)
	{
		value_type v;
		resize(n, v);// !!! Some Complier don't allow to do this
	}

	void resize(int n, int value)
	{
		if (n <= (last - elem))
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
		else
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

	//Ԫ�ط���
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

	//ջ����

	void push_back(value_type v)
	{
		
	}

	void pop_back()
	{

	}

	void emplace_back()//args
	{

	}

	//�б����

	iterator insert(iterator p, int x)
	{

	}

	iterator insert(iterator p, int n, int x)
	{

	}

	iterator insert(iterator p, void * first, void * last)
	{

	}

	iterator insert(iterator p, std::initializer_list<int> li)
	{

	}

	iterator earse(iterator p)
	{

	}

	iterator earse(iterator first, iterator last)
	{

	}

	void clear()
	{

	}

	//�Ƚ��뽻��

	bool operator==(vector& v);

	bool operator!=(vector& v);

	bool operator<(vector& v);

	bool operator<=(vector& v);

	bool operator>(vector& v);

	bool operator>=(vector& v);

	void swap(vector& v);

	friend void swap(vector&v1, vector&v2);


};
