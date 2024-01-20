#include "OPS_input_t.hpp"

#include <set>
#include "matrix.hpp"

namespace EMIR
{

/*OPS_input_t::OPS_input_t(const OPS_instance_t& instance, const vector<int>& nJ, const vector<int>& nK):
    instance_(instance),
    succ_(),
    pred_(),
    succ_inx_(),
    pred_inx_(),
    A_succ_(),
    inv_succ_(),
    t_cost_()
{
    const int K = nK.size();
    const int n = nJ.size();

    nodes_k_.resize(K);

    succ_.resize(K,n);
    pred_.resize(K,n);

    succ_inx_.resize(K,n);
    pred_inx_.resize(K,n);

    inv_succ_.resize(K);

    arcs_k_.resize(K);

    t_cost_.resize(n,n);

    int l = 0;

    for(int ki = 0; ki < K; ki++) {

        const int k = nK[ki];

        const vector<int>& Jk = instance.get_Jk(k);
        const int nJ = Jk.size();

        matrix<int>& A_inv  = inv_succ_[ki];
        A_inv.resize(n,n);
        A_inv.init(-1);

        vector<int>& arcs_k = arcs_k_[ki];

        vector<int> nodes(n,0);

        for(auto j: Jk) {

            succ_(ki + 1, 1).push_back(j);
            succ_inx_(ki + 1, 1).push_back(l);
            A_succ_.push_back(get_pos(k, 0, j));
            A_inv(0 + 1,j + 1) = l;
            arcs_k.push_back(l);

            l++;

            nodes[0]++;
            nodes[j]++;
        }



        succ_(k + 1, 1).push_back(n - 1);
        succ_inx_(k + 1, 1).push_back(l);
        A_succ_.push_back(get_pos(k, 0, n - 1));
        A_inv(0 + 1, n) = l;
        arcs_k.push_back(l);

        l++;

        nodes[0]++;
        nodes[n - 1]++;

        for(int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];

            for(int j = 0; j < nJ; j++)
                if (i != j) {

                    const int Jj = Jk[j];

                    succ_(k + 1,Ji + 1).push_back(Jj);
                    succ_inx_(k + 1,Ji + 1).push_back(l);
                    A_succ_.push_back(get_pos(k, Ji, Jj));

                    A_inv(Ji + 1,Jj + 1) = l;

                    arcs_k.push_back(l);

                    l++;

                    nodes[Ji]++;
                    nodes[Jj]++;
                }

            succ_(k + 1,Ji + 1).push_back(n - 1);
            succ_inx_(k + 1,Ji + 1).push_back(l);
            A_succ_.push_back(get_pos(k, Ji, n - 1));

            A_inv(Ji + 1, n) = l;

            arcs_k.push_back(l);

            l++;

            nodes[Ji]++;
            nodes[n - 1]++;
        }

        vector<int>& nodes_k = nodes_k_[k];
        for(int i = 1; i < n - 1; i++)
            if (nodes[i] > 0)
                nodes_k.push_back(i);
    }

} */

void OPS_input_t::resize_structures(void)
{
    const int m = get_m();
    const int n = get_n();
	
	//cout << "CREANDO ESTRUCTURAS: " << m << " " << n << endl;
    
    nodes_k_.resize(m);

    succ_.resize(m,n);
    pred_.resize(m,n);

    succ_inx_.resize(m,n);
    pred_inx_.resize(m,n);

    inv_succ_.resize(m);

    arcs_k_.resize(m);

    t_cost_.resize(n,n);
}


void OPS_input_t::update_structures(int k, int Ji, int Jj, int& l, GOMA::matrix<int>& A_inv, vector<int>& arcs_k, vector<int>& nodes)
{
	//cout <<"Arco: " << Ji << ", " << Jj << endl;
	
    succ_(k + 1, Ji + 1).push_back(Jj);
    succ_inx_(k + 1, Ji + 1).push_back(l);
    A_succ_.push_back(get_pos(k, Ji, Jj));

    A_inv(Ji + 1, Jj + 1) = l;

    arcs_k.push_back(l);

    nodes[Ji]++;
    nodes[Jj]++;

    l++;
}

void OPS_input_t::update_pred(int k, int Ji, int Jj, int& l)
{
    pred_(k + 1, Jj + 1).push_back(Ji);
    pred_inx_(k + 1, Jj + 1).push_back(l);

    l++;
}

void OPS_input_t::init_t_cost(void)
{
    const int n = get_n();
    
    for(int i = 1; i <= n - 1; i++) {

        //shortest_path(aux, i - 1, dist, prev);
        for(int j = 2; j <= n; j++)
            if (i != j) {

                //cout << dist[j - 1] - instance.get_T()(i,j) << endl;
                //assert(dist[j - 1] <= instance.get_T()(i,j));

                t_cost_(i, j) = get_T(i,j);
            } else
                t_cost_(i, j) = INF_SP;

    }

    t_cost_(1, n) = 0;

    for(int j = 1; j <= n; j++)
        t_cost_(n, j) = INF_SP;

    for(int i = 1; i <= n; i++)
        t_cost_(i, 1) = INF_SP;
        
    //t_cost_.write_raw(cout);

}

void OPS_input_t::get_L(vector<int>& L) const
{
    const int n = get_n();
    
    L.resize(n);
    
    for(int i = 0; i < n - 1; i++)
        L[i] = 0;
        
    L[n - 1] = get_L();
}

void OPS_input_t::make_structures(void) // AQUÍ!!!!!!!!!
{
    int l = 0;
    
    const int n = get_n();
    const int m = get_m();

    for(int k = 0; k < m; k++) {

        const vector<int>& Jk = get_Jk(k);

        const int nJ = Jk.size();

        GOMA::matrix<int>& A_inv  = inv_succ_[k];
        A_inv.resize(n, n);
        A_inv.init(-1);

        vector<int>& arcs_k = arcs_k_[k];

        vector<int> nodes(n, 0);

        for(auto j: Jk) {
			if (t_cost_(1, j + 1) < INF_SP)
				update_structures(k, 0, j, l, A_inv, arcs_k,  nodes);			
		}

        update_structures(k, 0, n - 1, l, A_inv, arcs_k, nodes);

        for(int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];

            for(int j = 0; j < nJ; j++)
                if (i != j) {

                    const int Jj = Jk[j];

					if (t_cost_(Ji + 1, Jj + 1) < INF_SP)
						update_structures(k, Ji, Jj, l, A_inv, arcs_k, nodes);
                }

            update_structures(k, Ji, n - 1, l, A_inv, arcs_k, nodes);
        }
		
		//cout << "Concluido" << endl;

        vector<int>& nodes_k = nodes_k_[k];
        for(int i = 1; i < n - 1; i++)
            if (nodes[i] > 0)
                nodes_k.push_back(i);

    }
}

void OPS_input_t::make_prev(void)
{
    const int m = get_m();
    const int n = get_n();
    
    int l = 0;

    for(int k = 0; k < m; k++) {

        const vector<int>& Jk = get_Jk(k);
        const int nJ = Jk.size();

        for(auto j: Jk)
            update_pred(k, 0, j, l);

        update_pred(k, 0, n - 1, l);

        for(int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];

            for(int j = 0; j < nJ; j++)
                if (i != j) {

                    const int Jj = Jk[j];

                    update_pred(k, Ji, Jj, l);
                }

            update_pred(k, Ji, n - 1, l);
        }
    }    
}

void OPS_input_t::build_input(void)
{

    resize_structures();

    make_structures();

    make_prev();

    init_t_cost();

#ifndef NDEBUG
    test_succ();
    test_pred();
    test_A_succ();
#endif
}


void OPS_input_t::write_arc_inx(ostream& os, int inx) const
{
    const int pos = get_A_succ(inx);
    int s, t, k;
    get_pos(pos, k, s, t);                
    
    os << "(" << setw(3) << s << ", " << setw(3) << t << ")[" << setw(2) << k << " ]";    
}

OPS_input_t::OPS_input_t(const OPS_instance_t& instance, bool build):
    instance_(instance),
    succ_(),
    pred_(),
    succ_inx_(),
    pred_inx_(),
    A_succ_(),
    inv_succ_(),
    t_cost_()
{
    if (build) 
        build_input();        
}

int OPS_input_t::get_max_arc(void) const
{
    int max = 0;

	const int sz = get_A_succ_sz();

	for(int l = 0; l < sz; l++){
  
		const int pos = get_A_succ(l);
  
		int i, j, k;          
		get_pos(pos, k, i, j);   

		if (max < t_cost_(i + 1,j + 1))
			max = t_cost_(i + 1,j + 1 );	
	}	
 	
	return max;
}

int OPS_input_t::get_max_nodes(void) const
{
    int m = -1;
    const int K = get_m();

    for(int k = 0; k < K; k++) {

        const int csz = nodes_k_[k].size();

        m = m < csz? csz: m;
    }

    return m;
}

const vector<int> OPS_input_t::get_inv_succ(int i, int j)  const
{
	vector<int> aux;
	const int K = get_m();

    for(int k = 0; k < K; k++) {
		
		const int inx = get_inv_succ(k, i, j);
		
		if (inx >= 0)
			aux.push_back(inx);			
	}
	
	return aux;
}


void OPS_input_t::get_sync_stat(int& nsync, int& maxgsync, int& mingsync, double& avggsync) const
{
    const int K = get_m();
    const int n = get_n();

    vector<int> V(n);

    for(int& v: V)
        v = 0;

    for(int k = 0; k < K; k++) {

        const vector<int>& nk =  nodes_k_[k];

        for(auto v: nk)
            V[v] ++;
    }

    nsync = 0;
    maxgsync = 1;
    mingsync = n;
    avggsync = 0;

    int cnt = 0;

    for(int i = 1; i < n - 1; i++) {

        assert(V[i] > 0);

        if (V[i] > 1) {

            cnt ++;

            nsync++;

            if (V[i] > maxgsync)
                maxgsync = V[i];

            if (V[i] < mingsync)
                mingsync = V[i];

            avggsync += V[i];
        }
    }

    if (cnt > 0)
        avggsync /= (double)(cnt);
    else {
        maxgsync = 0;
        mingsync = 0;
        avggsync = 0;
    }
}

double OPS_input_t::get_avg_nodes(void) const
{
    double m = 0;
    const int K = get_m();

    for(int k = 0; k < K; k++) {

        const int csz = nodes_k_[k].size();
        m += csz;
    }

    return m/(double)(K);
}

OPS_input_t::~OPS_input_t(void)
{}


int OPS_input_t::get_msucc(int k) const
{
    int m = 0;

    for(int i = 0; i < get_n(); i++)
        m += get_nsucc(k, i);

    return m;
}


int OPS_input_t::get_mm(void) const
{
    const int K = get_m();
    int m = 0;

    for( int k = 0; k < K; k++)
        m += get_msucc(k);

    return m;
}

void OPS_input_t::get_pos(int pos, int& k, int& i, int& j) const
{
    const int n = get_n();
    const int m = n * n;

    k = pos / m;
    int res = pos % m;

    i = res / n;
    j = res % n;
}


bool OPS_input_t::check_path (const vector<int>& p) const
{
    int k, i, j;

    const int first = get_A_succ(p[0]);

    get_pos(first, k, i,j);

    const bool depot1 = (i == 0);

    const int last  = get_A_succ(p[p.size() - 1]);

    get_pos(last, k, i,j);

    const bool depot2 = (j == (get_n() - 1));

    return (depot1 && depot2);
}

int  OPS_input_t::length_path(const vector<int>& p) const
{
    const int sz = p.size();

    int len = 0;

    for(int l = 0; l < sz; l++) {

        const int inx = get_A_succ(p[l]);

        int k, i, j;

        get_pos(inx, k, i,j);

        cout << get_t(i, j) << " - ";

        len += get_t(i, j);
    }

    cout << endl;
    cout << len << endl;



    return len;

}

void OPS_input_t::get_path(const vector<int>& v, int k, vector<int>& arcs, vector<bool>& visited) const
{
    const int sz = v.size() - 1;
    assert(sz > 0);

    for(int l = 0; l < sz; l++) {

        visited[v[l]] = true;
        visited[v[l+1]] = true;

        const int p = get_inv_succ(k, v[l], v[l + 1]);
        arcs.push_back(p);
    }
}

void OPS_input_t::get_path(const vector<int>& v, int k, vector<int>& arcs) const
{
    const int sz = v.size() - 1;
    assert(sz > 0);

    for(int l = 0; l < sz; l++) {

        const int p = get_inv_succ(k, v[l], v[l + 1]);
        arcs.push_back(p);
    }
}

void OPS_input_t::get_mpath(const vector<int>& v, int k, vector<int>& arcs, vector<bool>& visited) const
{
    if (v.size() == 0) return;

    get_path(v, k, arcs, visited);

    const int sz = v.size();

    for(int i = 0; i < sz - 2; i++)
        for(int  j = i + 2; j < sz; j++) {

            const int p = get_inv_succ(k, v[i], v[j]);
            arcs.push_back(p);
        }


    for(int i = sz - 2; i >= 2; i--)
        for(int j = i - 1; j >= 1; j--) {

            const int p = get_inv_succ(k, v[i], v[j]);
            arcs.push_back(p);
        }


}





void OPS_input_t::shortest_path(const GOMA::matrix<int>& M, int source, int* dist, int* prev )
{

    //M.write_raw(cout);

    set<int> Q;
    std::set<int>::iterator it;

    const int n = M.get_m();


    dist[source] = 0;
    prev[source] = -1;

    for ( int i = 0; i < n; i++ ) {

        if ( i != source ) {

            dist[i] = INF_SP;
            prev[i] = -1;
        }
        Q.insert( i );
    }


    while (!Q.empty()) {


        int u = -1;
        int best_val = INF_SP + 1;

        for ( int i = 0; i < n; i++ )
            if ( (Q.find( i ) != Q.end()) && ( dist[i] < best_val ) ) {

                best_val = dist[i];
                u = i;
            }

        assert(u >= 0);

        Q.erase( Q.find (u) );

        for ( int i = 0; i < n; i++ ) {

            const int t = M(u + 1, i + 1);

            //if (M(u + 1, i + 1) > 0)
            {

                const int alt = best_val + t;
                const int v = i;

                if ( alt < dist[v] ) {

                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }

        /* for(int i = 0; i < n; i++)
            cout << dist[i] << " ";
        cout << endl; */
    }
}


#ifndef NDEBUG

void OPS_input_t::test_succ(void)
{
    const int K = get_m();

    for(int k = 0; k < K; k++) {

        set<int> SJk;
        const vector<int>& Jk = get_Jk(k);
        const int nJ = Jk.size();

		//cout << "Grupo: " << k << endl;

		//cout << "     ";

        for(auto i: Jk) {
            SJk.insert(i);
			
			//cout << setw(3) << i << " ";
			
		}
		//cout << endl;

        for(int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];
            const vector<int>& v = succ_(k + 1, Ji + 1);
            const int sz = v.size();

			//cout << "    Sucesores de : " << Ji << endl;
			
			//for(int j: v)
			//	cout << setw(3) << j << " ";
			//cout << endl;

            for(int l = 0; l < sz - 1; l++) {
				
                const int j = v[l];
				
				//cout << j << "      Checking: ";
				
				//if (SJk.find(j)== SJk.end())
					//exit(1);
				
                assert(SJk.find(j)!= SJk.end());
				
				//cout << "OK" << endl;
            }
        }
    }
}

void OPS_input_t::test_pred(void)
{
    const int K = get_m();

    for(int k = 0; k < K; k++) {

        set<int> SJk;
        const vector<int>& Jk = get_Jk(k);
        const int nJ = Jk.size();

        for(auto i: Jk)
            SJk.insert(i);

        for(int i = 0; i < nJ; i++) {

            const int Ji = Jk[i];
            vector<int>& v = pred_(k + 1,i + 1);
            const int sz = v.size();

            for(int l = 1; l < sz; l++) {

                const int j = v[l];
                assert((SJk.find(Ji) != SJk.end()) && (SJk.find(j)!= SJk.end()));
            }
        }
    }
}


void OPS_input_t::test_A_succ(void)
{
    const int sz = A_succ_.size();
    const int n  = get_n();

    for(int l = 0; l < sz; l++) {

        const int pos = A_succ_[l];

        int i, j, k;
        get_pos(pos, k, i, j);

        if ((i != 0) && (j != n - 1)) {

            set<int> SJk;
            const vector<int>& Jk = get_Jk(k);

            for(auto Ji: Jk)
                SJk.insert(Ji);

            assert((SJk.find(i) != SJk.end()) && (SJk.find(j)!= SJk.end()));
        }
    }
}


#endif

void OPS_input_t::write_statistics_hdr(ostream& os) const
{
    instance_.write_statistics_hdr(os);

    os << "$n$" << "\t";
    os << "$m$" << "\t";

    os << "$\\max|J_k|$" << "\t";
    os << "$\\bar{|J_k|}$" << "\t";

    os << "nsync" << "\t";
    os << "$\\max\\partial\\mbox{sync}$" << "\t";
    os << "$\\bar{\\partial\\mbox{sync}$" << "\t";

    os << "$L$" << "\t";
}

void OPS_input_t::write_statistics(ostream& os) const
{
    instance_.write_statistics(os);

    int nsync, maxgsync, mingsync;
    double avggsync;

    get_sync_stat(nsync, maxgsync, mingsync, avggsync);

    os << setw(4)  << get_n() << "\t";
    os << setw(4)  << get_m() << "\t";

    os << setw(4)  << get_max_nodes() << "\t";
    os << setw(6)  << fixed << setprecision(1) << get_avg_nodes() << "\t";

    os << setw(4)  << nsync << "\t";
    os << setw(4)  << maxgsync << "\t";
    os << setw(6)  << fixed << setprecision(1) << avggsync << "\t";

    os << setw(6)  << get_L() << "\t";
    os << "\t";
}

}
