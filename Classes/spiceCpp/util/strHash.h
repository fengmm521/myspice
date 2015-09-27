//
//  strHash.h
//  game1
//  使用结构体来进行游戏属性存储会比较方便快捷
//
//  Created by 俊盟科技1 on 8/26/14.
//
//
//为了使用字符串作为switch的case分支，这里加了些方法

#ifndef game1_strHash_h
#define game1_strHash_h
#include <iostream>
#include <string>
using namespace std;

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t hash_(char const* str);
//{
//	hash_t ret{basis};
//    
//	while(*str){
//		ret ^= *str;
//		ret *= prime;
//		str++;
//	}
//    
//	return ret;
//}

constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
	return *str ? hash_compile_time(str+1, (*str ^ last_value) * prime) : last_value;
}

constexpr unsigned long long operator "" _hash(char const* p, size_t)
{
	return hash_compile_time(p);
}

/*字符串作为case例子
void simple_switch(char const* str)
{
	using namespace std;
	switch(hash_(str)){
        case "first"_hash:
            cout << "1st one" << endl;
            break;
        case "second"_hash:
            cout << "2nd one" << endl;
            break;
        case "third"_hash:
            cout << "3rd one" << endl;
            break;
        default:
            cout << "Default..." << endl;
	}
}
 */


#endif
