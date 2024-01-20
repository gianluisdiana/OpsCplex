#pragma once

#include <cstring>
#include <cassert>

using namespace std;

#define N_BITS_WORD (8 * sizeof(T))


namespace EMIR {

template <class T, size_t N>
class bitset_t
{
	public:
		T* block_;
		int sz_;
	
	public:
		bitset_t(void):
			block_(NULL),
			sz_(N/N_BITS_WORD){

			if ((N % N_BITS_WORD) != 0)
				sz_ ++;
		
			block_ = new T[sz_];

            clear();
		};

		bitset_t(const bitset_t& bs):
		block_(NULL),
		sz_(bs.sz_)
		{
			block_ = new T[bs.sz_];	

			for(int i = 0; i < sz_; i++)
				block_[i] = bs.block_[i];		
		}

		~bitset_t(void) {
			if (block_) delete [] block_;
		}


		const bitset_t& operator=(const bitset_t& bs)
		{
			for(int i = 0; i < sz_; i++)
				block_[i] = bs.block_[i];		
                
            return bs;
		}
        
        void clear(void) {
            
 			for(int i = 0; i < sz_; i++)
				block_[i] = 0;                       
        }

		void insert(unsigned int i){

			assert(i <= N);
			assert(i >= 1);

			i--;

			const int pos    = i/N_BITS_WORD;
			const int offset = i%N_BITS_WORD;

			block_[pos] |= (T(0x1) << offset);
		}

		void remove(unsigned int i){

			assert(i <= N);
			assert(i >= 1);

			i--;

			const int pos    = i/N_BITS_WORD;
			const int offset = i%N_BITS_WORD;

			block_[pos] &= ~(T(0x1) << offset);
		}

		bool contains(unsigned int i) const
		{

			assert(i <= N);
			assert(i >= 1);

			i--;

			const int pos    = i/N_BITS_WORD;
			const int offset = i%N_BITS_WORD;

			return (block_[pos] & (T(0x1) << offset)) != T(0x0);  
		}

		void insert(const bitset_t& bs)
		{
			for(int i = 0; i < sz_; i++)
				block_[i] |= bs.block_[i];
		}

		void remove(const bitset_t& bs)
		{
			for(int i = 0; i < sz_; i++)
				block_[i] &= ~bs.block_[i];
		}

		bool contains(const bitset_t& bs) const
		{	
			bool contains_set = true;

			int i = 0;

			while ((i < sz_) && (contains_set)){
				contains_set = contains_set && ((block_[i] & bs.block_[i]) == bs.block_[i]);
				i++;
			}

			return contains_set;		
		}

		int first_item(void) const
		{			
			int sm = -1;

			for(int i = 0; (i < sz_) && (sm == -1);i++)
			{
				if (block_[i] != 0)
					sm = first_item(block_[i]) + N_BITS_WORD * i;
			}

			return sm;
			
		}

		int last_item(void) const
		{		
			int bg = -1;

			for(int i = sz_ - 1; (i >= 0) && (bg == -1);i--)
			{
				if (block_[i] != 0)
					bg = last_item(block_[i]) + N_BITS_WORD * i;
			}

			return bg;
			
		}

		int cardinality(void) const
		{
			int card = 0;

			for(int i = 0; i < sz_; i++)
				card += cardinality(block_[i]);

			return card;
		}

		ostream& write(ostream& os) const 
		{

			string s;
			to_string(s);
			os << s;

			return os;
		}


		void union_set(const bitset_t&  B, bitset_t& C) const
		{
			for(int j = 0; j < sz_; j++)				
				C.block_[j] = block_[j] | B.block_[j];						
		}	

		void intersec_set(const bitset_t&  B, bitset_t& C) const
		{
			for(int j = 0; j < sz_; j++)				
				C.block_[j] = block_[j] & B.block_[j];						
		}

		void diff_set(const bitset_t&  B, bitset_t& C) const
		{
			for(int j = 0; j < sz_; j++)				
				C.block_[j] = block_[j] & ~B.block_[j];						
		}

	private:

		int first_item(T block) const
		{
            return __builtin_ffsl(block);
		}

		int last_item(T block) const
		{
            return N_BITS_WORD - __builtin_clzl(block);
		}

		int cardinality(T block) const
		{
            return __builtin_popcount(block);
		}
	
		 void to_string(string& s) const 
		 {
			for(int j = 0; j < sz_; j++){

				const int sz = min(N_BITS_WORD, N - j * N_BITS_WORD);

				T block = block_[j];		

				for(int i = 0; i < sz; i++)
				{
					const char c = '0' + (block & T(0x1));
					s.insert(s.begin(),c); 
					block >>= 1;
				}
			}
		 }
};




}

