#pragma once 

#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "json.hpp"
using json = nlohmann::json;


#define WIDE 20

using namespace std;

namespace GOMA {

template <class T>
class matrix
{
private:
        int m_;
        int n_;
        
        vector<T> v_;
        
public:
        matrix(void):
        m_(0),
        n_(0),
        v_() {}
        
        matrix(int m, int n):
        m_(m),
        n_(n),
        v_() {
                
            build();

        }
        
        virtual ~matrix(void)
        {}
        
        void resize(int m, int n)
        {
            m_ = m; n_ = n;

            build();
        }
        
        void init(T data){
            
            const int sz = m_ * n_;
            
            for(int i = 0; i < sz; i++)
                v_[i] = data;
            
        }
        
        T& operator[] (int i) {
            
            assert(i >= 0);
            assert(i < m_ * n_);
            
            return v_[i];
        }
        
        const T& operator[] (int i) const {
            
            assert(i >= 0);
            assert(i < m_ * n_);
            
            return v_[i];
        }     
   
        
        T& operator() (int i, int j)
        {
            assert(i >= 1);
            assert(i <= m_);
            
            assert(j >= 1);
            assert(j <= n_);            
            
            return v_[pos(i,j)];
        }

        T& at(int i, int j)
        {
            assert(i >= 1);
            assert(i <= m_);
            
            assert(j >= 1);
            assert(j <= n_);            
            
            return v_[pos(i,j)];
        }

        const T& operator() (int i, int j) const
        {       
            assert(i >= 1);
            assert(i <= m_);
            
            assert(j >= 1);
            assert(j <= n_);
   
            return v_[pos(i,j)];
        }    

        const T& at(int i, int j) const
        {       
            assert(i >= 1);
            assert(i <= m_);
            
            assert(j >= 1);
            assert(j <= n_);
   
            return v_[pos(i,j)];
        }      
        
        int get_m(void) const 
        {
            return m_;
        }
        
        int get_n(void) const
        {
            return n_;
        }
   

        istream& read_raw(istream& is)
        {
            
             for(int i = 1; i <= m_; i++)
                 for(int j = 1; j <= n_; j++)
                        is >> v_[pos(i, j)];
            
            return is;
        } 

        ostream& write_raw(ostream& os) const
        {
            
             for(int i = 1; i <= m_; i++){
                 for(int j = 1; j <= n_; j++)
                        os << setw(WIDE) << scientific << setprecision(6) << v_[pos(i, j)] << " ";
                os << endl;
             }
            
            return os;
        }   
   
        int pos(int i, int j) const
        {
            return (i -1)* n_ + j - 1;
        }
    
       void get_json(json& Mat) const
        {
            vector<vector<T> > M;
            
            M.resize(m_);
            
            for(int i = 0; i < m_; i++){
                
                vector<T>& v = M[i];
                
                for(int j = 0; j < n_; j++)
                    v.push_back(v_[pos(i + 1, j + 1)]);                
            }

            Mat = M;
        }
        
        void set_json(const json& Mat)
        {
            v_.clear();
            
            vector<vector<T> > M = Mat.get<vector<vector<T> > >();
            
            m_ = M.size();            
            assert(m_ >= 1);            
            n_ = M[0].size();
            
            build();
            
            for(int i = 0; i < m_; i++){
                
                const vector<T>& v = M[i];
                
                for(int j = 0; j < n_; j ++)
                    v_[pos(i + 1, j + 1)] = v[j];
            }
        }
        

 
private:
        
        void build(void)
        {
			try {
				v_.resize(m_ * n_);
			}
			catch (const std::length_error& le) 
			{
				std::cerr << "Length error: " << le.what() << '\n';
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
			}
			catch (const std::logic_error& le) 
			{
				std::cerr << "Logic error: " << le.what() << '\n';
			}
        }
} ;


}


template <class T>
istream& operator>>(istream& is, GOMA::matrix<T>& M)
{
	M.read_raw(is);
	return is;
}

template <class T>
ostream& operator<<(ostream& os, const GOMA::matrix<T>& M)
{
	M.write_raw(os);
	return os;
}
