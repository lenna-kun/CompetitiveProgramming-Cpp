template <class S, S (*op)(S, int), S (*merge)(S, S), S (*op2)(S, int), S (*e)()>
struct re_rooting {
public:
	re_rooting(int n) : _n(n), g(n), dp(n) {}
 
	void add_edge(int u, int v, int idx = 1) {
		u -= idx, v -= idx;
		g[u].emplace_back(v);
		g[v].emplace_back(u);
	}
 
	std::vector<S> solve(int root = 0) {
		build(root);
		std::vector<S> vec(_n);
		for(int k = 0; k < _n; k++) {
			S res = e();
			for(int i = 0; i < g[k].size(); i++) res = merge(res, op(dp[k][i], g[k][i]));
			vec[k] = op2(res, k);
		}
		return vec;
	}
 
private:
	int _n;
	std::vector<std::vector<int>> g;
	std::vector<std::vector<S>> dp;
 
	S dfs1(int now, int per = -1) {
		S res = e();
		int cnt = -1;
		for(int i = 0; i < (int)g[now].size(); i++) {
			if(g[now][i] == per) continue;
			dp[now][i] = dfs1(g[now][i], now);
			res = merge(res, op(dp[now][i], g[now][i]));
		}
		return op2(res, now);
	}
 
	void dfs2(int now, int per = -1, S add = e()) {
		for(int i = 0; i < (int)g[now].size(); i++) {
			if(g[now][i] == per) {
				dp[now][i] = add;
				break;
			}
		}
 
		std::vector<S> pL(g[now].size() + 1), pR(g[now].size() + 1);
 
		pL[0] = e();
		for(int i = 0; i < (int)g[now].size(); i++)
			pL[i + 1] = merge(pL[i], op(dp[now][i], g[now][i]));
 
		pR[g[now].size()] = e();
		for(int i = (int)g[now].size() - 1; i >= 0; i--)
			pR[i] = merge(pR[i + 1], op(dp[now][i], g[now][i]));
 
		for(int i = 0; i < g[now].size(); i++) {
			if(g[now][i] == per) continue;
			dfs2(g[now][i], now, op2(merge(pL[i], pR[i + 1]), now));
		}
	}
 
	void build(int root) {
		for(int i = 0; i < _n; i++)
			dp[i].resize(g[i].size());
		dfs1(root);
		dfs2(root);
	}
};