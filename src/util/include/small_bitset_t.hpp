#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>

using namespace std;

namespace EMIR {

typedef unsigned long block_type;

#define N_SMALL_BITS_WORD (8 * sizeof(block_type))

template <size_t N = 64>
class small_bitset_t
{
	public:
		block_type block_;
	
	public:
		small_bitset_t(void):
			block_(0x0){

            clear();
		};

		small_bitset_t(const small_bitset_t& bs):
		block_(bs.block_)
		{}
        
		small_bitset_t(const block_type& b):
		block_(b)
		{}        

        small_bitset_t(const vector<int>& b):
		block_(0x0)
		{
            for(int i: b)
                insert(i);
        } 

		~small_bitset_t(void) {}
        
        inline block_type get_block(void) const {return block_;}

		const small_bitset_t& operator=(const small_bitset_t& bs)
		{
			block_ = bs.block_;		
                
            return bs;
		}
        
        inline void clear(void) {            
				block_ = 0x0;                       
        }

		void insert(unsigned int i){

			assert(i <= N);
			assert(i >= 1);

			i--;

			block_ |= (block_type(0x1) << i);
		}
        
        inline bool empty(void) const
        {
            return block_ == 0x0;
        }

        inline bool equal(const small_bitset_t& bs)
        {
            return block_ == bs.block_;
        }

		void remove(unsigned int i){

			assert(i <= N);
			assert(i >= 1);

			i--;

			block_ &= ~(block_type(0x1) << i);
		}

		bool contains(unsigned int i) const
		{

			assert(i <= N);
			assert(i >= 1);
            
            i--;

			return (block_ & (block_type(0x1) << i)) != block_type(0x0);  
		}

		inline void insert(const small_bitset_t& bs)
		{
            block_ |= bs.block_;
		}

		inline void remove(const small_bitset_t& bs)
		{
            block_ &= ~bs.block_;
		}

		inline bool contains(const small_bitset_t& bs) const
		{	
			return ((block_ & bs.block_) == bs.block_);		
		}

		ostream& write(ostream& os) const 
		{

			string s;
			to_string(s);
			os << s;

			return os;
		}


		inline void union_set(const small_bitset_t&  B, small_bitset_t& C) const
		{				
				C.block_ = block_ | B.block_;						
		}	

		inline void intersec_set(const small_bitset_t&  B, small_bitset_t& C) const
		{				
				C.block_ = block_ & B.block_;						
		}

		inline void diff_set(const small_bitset_t&  B, small_bitset_t& C) const
		{				
				C.block_ = block_ & ~B.block_;						
		}


		inline int first_item(void) const
		{
            return __builtin_ffsl(block_);
		}

		inline size_t last_item(void) const
		{
            return N_SMALL_BITS_WORD - __builtin_clzl(block_);
		}

		inline size_t cardinality(void) const
		{
            return __builtin_popcountl(block_);
		}
	
		 void to_string(string& s) const 
		 {

            block_type block = block_;	

            for(size_t i = 0; i < N; i++)
            {
                const char c = '0' + (block & block_type(0x1));
                s.insert(s.begin(),c); 
                block >>= 1;
            }			
		 }
};


}