//算法1
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<unordered_map>
#include<string>
#include<time.h>
//#define RED     "\033[31m"      /* Red */
//#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
using namespace std;


bool equal(string a, string b)
{//判断a和b是否相等
	if (a.size() != b.size())
		return 0;
	for (int i = 0; i < a.size(); i++)
		if (a[i] != b[i])
			return false;
	return true;
}

vector<int> Identity;//A分块
int identified = 0;

vector<int> search(string s, string subs)
{//返回s中未匹配的子串subs的所以起始位置
	vector<int> ans;
	int loc = 0;
	while ((loc = s.find(subs, loc)) != string::npos)	
	{
		ans.push_back(loc);
		loc++;
	}
	return ans;
}


void dfs_(int s, vector<string> M, string A,vector<int> A_identity)
{//根据块匹配M对A_identity进行遍历填色,以找到一个正确的块对应关系
	if (identified)
		return;
	string subs = M[s];
	vector<int> loc = search(A, subs);//所有可能的位置
	int l, r;
	for (int i = 0; i < loc.size(); i++)
	{
		if (identified)
			return;
		l = loc[i];
		r = l + subs.size() - 1;
		int k;
		for (k = l; k <= r; k++)
			if (A_identity[k])
				break;
		if (k == r + 1)
		{
			for (k = l; k <= r; k++)
				A_identity[k] = s;
		}
		else
			continue;
		if (s+1 == M.size())
		{//当前填色可行,将对应关系保存在Identity中
			identified = 1;
			Identity = A_identity;
			return;
		}
		//处理下一个块
		dfs_(s + 1, M, A,A_identity);
		for (k = l; k <= r; k++)//取消填色以回溯
			A_identity[k] = 0;
	}

}

int get_duo(vector<string>list)
{//计算块list的duo-preservation数
	int ans = 0;
	for (auto x : list)
		ans += (x.size() - 1);
	return ans;
}

void SimpleInsert1(string A, string &B, vector<string> &M)
{//简单版本的SF-MIDP算法
	//预处理
	int m = M.size() - 2;
	vector<int> pi(m + 2);
	vector<int> b_l;
	for (int i = 0; i < A.size(); i++)
		Identity.push_back(0);
	//对A分块
	dfs_(1, M, A,Identity);
	vector<string> C;
	C.push_back("");//识别C[1~m+1]
	for (int i = 2; i < Identity.size(); i++)
	{
		if (Identity[i] == 0)
		{
			int j = i ;
			while(j < Identity.size() && Identity[j] == 0)
				j++;
			C.push_back(A.substr(i, j - i));
			while (j+1 < Identity.size() && Identity[j] == Identity[j + 1])
				j++;
			i = j;
		}
		else
		{
			C.push_back("");
			while (i+1< Identity.size()&& Identity[i + 1] == Identity[i])
				i++;
		}
	}
	//认别pi[0~m+1]
	pi[0] = 0;
	int pi_id = 1;
	for (int i = 2; i < Identity.size(); i++)
	{
		if (Identity[i])
		{

			pi[pi_id] = Identity[i];
			pi_id++;
			while (i + 1 < Identity.size() && Identity[i + 1] == Identity[i])
				i++;

		}
	}
	vector<string> P(m + 2);
	for (int i = 0; i < P.size(); i++)
		P[i] = "";
	vector<string> M_ans = M;
	int delta = get_duo(M);
	//迭代更新
	for (int i = 1; i <= m + 1; i++)
	{
		if (C[i] == "")
			continue;
		int loc = B.find(M[pi[i - 1]]);
		string t = B.substr(0, loc + M[pi[i - 1]].size()) + C[i];
		t = t + B.substr(loc + M[pi[i - 1]].size(), B.size() - (loc + M[pi[i - 1]].size()));
		B = t;

		if (pi[i - 1] + 1 == pi[i])
		{
			M_ans[pi[i-1]] = "";
			P[i] = P[i - 1] + M[pi[i - 1]] + C[i];
			M_ans[pi[i]] = P[i] + M[pi[i]];

			delta += (C[i].size() + 1);
		}
		else
		{
			M_ans[pi[i - 1]] = M_ans[pi[i - 1]] + C[i];
			delta += C[i].size();
		}
		for (int j = 0; j < M_ans.size(); j++)
		{
			if (M_ans[j] != "")
			{
				cout << M_ans[j] << " ";
			}
		}
		cout << endl << endl;
	}
	if (equal(A, B))
	{
		M.clear();
		M.push_back(B);
		return;
	}
	M.clear();
	for (auto i : M_ans)
	{//更新M
		if (i != "")
		{
			M.push_back(i);
		}
	}
	//由两种方法得到最终的duo-preservation数相等知算法实现正确
	//cout << "M' duo?: " << delta << endl;
	//cout << "M'_duo: " << get_duo(M) << endl;
}

int randint(int i, int j)
{//返回[i,j]内的随机数
	if (i > j)
		swap(i, j);
	return i + rand() % (j - i + 1);
}

void shuffle(vector<int> &a)
{//对a混洗打乱顺序
	int n = a.size();
	for (int i = 0; i <n; ++i)
		swap(a[i], a[randint(i,  n- 1)]);
}

string randstr(int len)
{//返回一个长度为len的随机字符串
	string ans = "";
	for (int i = 0; i < len; i++)
	{
		char t = 'a' + randint(0, 25);
		ans += t;
	}
	return ans;
}

void getA_B_M(string &A, string &B, vector<string> &M)
{//生成初始数据
	int b_len = 20;//定义B的长度
	B = "";
	for (int i = 0; i < b_len; i++)
	{
		char t = 'a' +rand() % 26;
		B = B + t;
	}
	int m = b_len / 5;//定义块数为B的长度/5
	vector<int> tail;
	vector<int> vis(b_len);
	for (int i = 0; i < b_len; i++)
	{
		vis[i] = 0;
	}
	for (int i = 0; i < m; i++)
	{
		int t = randint(1, b_len - 3);
		while (vis[t]||vis[t-1]||vis[t+1])
		{
			t = randint(1, b_len - 3);
		}
		vis[t] = 1;
		tail.push_back(t);
	}
	sort(tail.begin(), tail.end());
	tail.push_back(b_len - 1);
	
	int start = 0;
	for (int i = 0; i < tail.size(); i++)
	{
		string t = B.substr(start, tail[i] - start + 1);
		M.push_back(t);
		start = tail[i] + 1;
	}
	vector<int> pi(M.size());
	for (int i = 0; i < pi.size(); i++)
	{
		pi[i] = i;
	}
	shuffle(pi);
	A = "##";
//	cout << "C[i]: " << endl;
	for (int i = 0; i < M.size(); i++)
	{
		int clen = randint(1, 5);
		string ci = randstr(clen);
//		cout << ci << " ";
		A += ci;
		A += M[pi[i]];
	}
	int clen = randint(1, 5);
	string ci = randstr(clen);
	A += ci;
	A += "$$";
	B = "##" + B + "$$";
	/*cout << "M_:" << endl;
	for (int i = 0; i < M.size(); i++)
		cout << M[pi[i]] << " ";
	cout << endl;*/
	M.insert(M.begin(),"##");
	M.push_back("$$");

}

int main()
{
	//string B = "##abbcac$$";
	//string A = "##bacaabcbc$$";
	//vector<string> M = { "##","ab","bc","ac","$$" };
	//SimpleInsert1(A, B, M);
	//cout << time(NULL) << endl;
	//srand(time(NULL));
	//srand(1619247300);
	string a, b;
	vector<string> m;
	getA_B_M(a, b, m);
	SimpleInsert1(a, b, m);
	return 0;
}
