#include <iostream>

static int i = 0;

template <typename T>
void	pr(T arg)
{
	std::cout << "---->>\t" << i << "\t" << arg << "\t" << std::endl;
	i++;
}

template <typename T1, typename T2>
void	pr(T1 arg1, T2 arg2)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3>
void	pr(T1 arg1, T2 arg2, T3 arg3)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3, typename T4>
void	pr(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << "\t" << arg4 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void	pr(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << "\t" << arg4 << "\t" << arg5 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void	pr(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << "\t" << arg4 << "\t" << arg5 << "\t" << arg6 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void	pr(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << "\t" << arg4 << "\t" << arg5 << "\t" << arg6 << "\t" << arg7 << std::endl;
	i++;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void	pr(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
{
	std::cout << "---->>\t" << i << "\t" << arg1 << "\t" << arg2 << "\t" << arg3 << "\t" << arg4 << "\t" << arg5 << "\t" << arg6 << "\t" << arg7 << "\t" << arg8 << std::endl;
	i++;
}