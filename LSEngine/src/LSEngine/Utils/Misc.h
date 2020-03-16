#pragma once

namespace LSE {

	template<typename type>
	static void Swap(type& a, type& b)
	{
		type c = a;
		a = b;
		b = c;
	}

}