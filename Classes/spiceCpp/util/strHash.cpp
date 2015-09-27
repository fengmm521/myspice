//
//  strHash.c
//  game1
//
//  Created by 俊盟科技1 on 8/26/14.
//
//


#include "strHash.h"
//typedef std::uint64_t hash_t;
//
//constexpr hash_t prime = 0x100000001B3ull;
//constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t hash_(char const* str)
{
	hash_t ret{basis};
    
	while(*str){
		ret ^= *str;
		ret *= prime;
		str++;
	}
    
	return ret;
}