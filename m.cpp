#include <bits/stdc++.h>
using namespace std;

#define int long long
#define _ ios_base::sync_with_stdio(0);cin.tie(0);


int32_t main(){
	_

	int k; cin >> k;
	int n, m; cin >> n >> m;
	vector g(n, vector<int>());
	vector<int> inc(n);
	vector<pair<int,int>> edg(m);
	for(int i=0;i<m;i++){
		int u, v; cin >> u >> v; u--,v--;
		g[u].push_back(v);
		g[v].push_back(u);
		inc[u] |= (1LL<<i);
		inc[v] |= (1LL<<i);
		edg[i] = {u, v};
	}

	// Quero: grafo em que os vertices correspondem a matchings
	// conexos com pelo menos k arestas.

	// Vamos assumir m <= 20 e construir um vetor que diz se a
	// bitmask eh valida ou nao.

	vector<bool> val(1LL<<m, 1);
	vector<int> ind(1LL<<m);
	
	// Vou comecar com O(n*2^m)
	for(int v=0;v<n;v++){
		for(int i=0;i<(1LL<<m);i++){
			if(__builtin_popcount(inc[v] & i) >= 2) val[i] = 0;
			if(__builtin_popcount(inc[v] & i) >= 1) ind[i] |= (1<<v);
		}
	}

	
	// Agora O((n+m)*2^n) para conectividade

	vector<bool> con(1LL<<n);
	for(int i=0;i<(1LL<<n);i++){
		int vis = 0;
		auto dfs2 = [&] (auto&& self, int u) -> void {
			vis |= (1LL<<u);
			for(auto v : g[u]){
				if(((i>>v)&1) && !((vis>>v)&1)) self(self, v);
			}
		};
		int cc = 0;
		for(int v=0;v<n;v++){
			if(((i>>v)&1) && !((vis>>v)&1)){
				cc++;
				dfs2(dfs2, v);
			}
		}
		if(cc == 1) con[i] = 1;
	}

	//cout << "Conjuntos conexos: " << endl;
	//for(int i=0;i<(1LL<<n);i++){
	//	if(con[i]){
	//		for(int j=0;j<n;j++) if((i>>j)&1) cout << j+1 << " ";
	//		cout << endl;
	//	}
	//}
	//cout << endl;

	for(int i=0;i<(1LL<<m);i++){
		val[i] = val[i] & con[ind[i]];
		if(__builtin_popcount(i) < k) val[i] = 0;
	}

	//int tot = 0;
	//for(auto x : val) tot += x;

	//vector<int> stat(m+1);
	//for(int i=0;i<(1LL<<m);i++){
	//	stat[__builtin_popcount(i)] += val[i];
	//}

	//for(int i=0;i<m;i++){
	//	cout << i << ": " << stat[i] << endl;
	//}
	//cout << "total: " << tot << endl; 

	vector<int> marc(1LL<<m);
	int cur_cc;
	auto dfs = [&] (auto&& self, int u) -> void {
		marc[u] = 1; cur_cc++;
		for(int j=0;j<m;j++) if((u>>j)&1) cout << "(" << edg[j].first+1 << "," << edg[j].second+1 << ") ";
		cout << endl;
		// adicionar aresta
		for(int i=0;i<m;i++){
			int v = u | (1LL<<i);
			if(val[v] && !marc[v]) self(self, v);
		}
		// remover aresta
		for(int i=0;i<m;i++){
			int v = u & ~(1LL<<i);
			if(val[v] && !marc[v]) self(self, v);
		}
	};

	int q_cc = 0;
	for(int i=0;i<(1LL<<m);i++){
		if(!val[i] || marc[i]) continue;
		cur_cc = 0;
		q_cc++;
		dfs(dfs, i);
		cout << endl << "Total: " << cur_cc << endl;
		cout << "------------------------------------" << endl;
		//if(cur_cc == 1){
		//	cout << "Isolado! arestas: ";
		//	for(int j=0;j<m;j++) if((i>>j)&1) cout << "(" << edg[j].first << "," << edg[j].second << ") ";
		//	cout << endl;
		//}
	}
	
	cout << "Total de componentes: " << q_cc << endl;
	


}
