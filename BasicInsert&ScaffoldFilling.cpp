//算法2、3
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<unordered_map>
#include<string>
#include<time.h>
using namespace std;

vector<int> Identity;
int identified = 0;

int cnt1;
int cnt2;
bool equal(string a, string b)
{//判断a和b是否相等
	if (a.size() != b.size())
		return 0;
	for (int i = 0; i < a.size(); i++)
		if (a[i] != b[i])
			return false;
	return true;
}

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

int get_duo(vector<string>list)
{//计算块list的duo-preservation数
	int ans = 0;
	for (auto x : list)
		ans += (x.size() - 1);
	return ans;
}

void dfs(int s, vector<string> M, string A, vector<int> identity)
{//根据块匹配M对A_identity进行遍历填色,以找到一个正确的块对应关系
	if (identified)
		return;
	string subs = M[s];
	vector<int> loc = search(A, subs);
	int l, r;
	for (int i = 0; i < loc.size(); i++)
	{
		if (identified)
			return;
		l = loc[i];
		r = l + subs.size() - 1;
		int k;
		for (k = l; k <= r; k++)
			if (identity[k])
				break;
		if (k == r + 1)
		{
			for (k = l; k <= r; k++)
				identity[k] = s;
		}
		else
			continue;
		if (s + 1 == M.size())
		{
			identified = 1;
			Identity = identity;
			return;
		}

		dfs(s + 1, M, A, identity);
		for (k = l; k <= r; k++)
			identity[k] = 0;
	}

}

bool no_unmatched(string B, char c,vector<int> Identity_B)
{//判断B中是否不存在未匹配的c
	int i = 0;
	for (; i < B.size(); i++)
		if (B[i] == c && Identity_B[i] == 0)
			break;
	return i == B.size();
}

bool ishead(int curr, vector<int> Identity_B)
{//判断当前位置是否为某块的头部
	return Identity_B[curr - 1] != Identity_B[curr] && Identity_B[curr] != 0 && curr != Identity_B.size() - 1 && Identity_B[curr] == Identity_B[curr + 1];
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
	for (int i = 0; i < n; ++i)
		swap(a[i], a[randint(i, n - 1)]);
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
	int b_len = 40;//定义B的长度
	B = "";
	for (int i = 0; i < b_len; i++)
	{
		char t = 'a' + rand() % 26;
		B = B + t;
	}
	int m = b_len / 10;//定义块数为B的长度/10
	vector<int> tail;
	vector<int> vis(b_len);
	for (int i = 0; i < b_len; i++)
	{
		vis[i] = 0;
	}
	for (int i = 0; i < m; i++)
	{
		int t = randint(1, b_len - 3);
		while (vis[t] || vis[t - 1] || vis[t + 1])
		{
			t = randint(1, b_len - 3);
		}
		vis[t] = 1;
		tail.push_back(t);
	}
	sort(tail.begin(), tail.end());
	tail.push_back(b_len - 1);

	int start = 0;
	M.clear();
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

	vector<string> b_C;
	for (int i = 0; i < M.size(); i++)
	{
		int clen = randint(1, 5);
		string ci = randstr(clen);
		//		cout << ci << " ";
		A += ci;
		A += M[pi[i]];
		//B
		int bloc = B.find(M[i]);
		int randomlen = randint(1, max(clen - 1, 1));
		int randomleft = randint(0, ci.size() - randomlen);
		string b_ci = ci.substr(randomleft, randomlen);
		string t = B.substr(0, bloc) + b_ci;
		t += B.substr(bloc, B.size() - bloc);
		B = t;
		b_C.push_back(b_ci);

	}
	//	cout << endl;
	int clen = randint(1, 5);
	string ci = randstr(clen);
	A += ci;
	int randomlen = randint(1, max(clen - 1, 1));
	int randomleft = randint(0, ci.size() - randomlen);
	string b_ci = ci.substr(randomleft, randomlen);
	B += b_ci;
	b_C.push_back(b_ci);

	A += "$$";
	B = "##" + B + "$$";

	/*cout << "M_:" << endl;
	for (int i = 0; i < M.size(); i++)
		cout << M[pi[i]] << " ";
	cout << endl;*/
	M.insert(M.begin(), "##");
	M.push_back("$$");
	//cout << "B:" << endl;
	//cout << B << endl;

}

void BasicInsert(string &A, string &B, vector<string> &M)
{//2近似的SF-MIDP算法
	int m = M.size() - 2;
	vector<int> pi(m + 2);
	vector<int> b_l;
	vector<int> Identity_A(A.size()), Identity_B(A.size());//
	//对A、B分块
	Identity.clear();
	identified = 0;
	dfs(1, M, A, Identity_A);//识别A的分块
	Identity_A = Identity;
	Identity.clear();
	identified = 0;
	dfs(1, M, B, Identity_B);//识别B的分块
	Identity_B = Identity;
	if (Identity_B.empty())
	{//当前随机数据不可行,需重新生成
		cout << "identity_B is empty" << endl;
		return;
	}
	vector<char>X;
	int vis[26];
	memset(vis, 0, sizeof(vis));
	for (auto i : A)
	{//生成缺失基因的多重集X
		if (vis[i - 'a'])
			continue;
		vis[i - 'a'] = 1;
		int n = count(A.begin(), A.end(), i) - count(B.begin(), B.end(), i);
		for (int k=0;k<n;k++)
		{
			X.push_back(i);
		}
	}
	//for (auto x : A)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl;
	//for (auto x : Identity_A)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl << endl;
	//for (auto x : B)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl;
	//for (auto x : Identity_B)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl << endl;
	int colorid = M.size() ;
	cnt1 = get_duo(M);
	//迭代更新
	while (!X.empty())
	{
		char x = X[0];
		string S(1,x);
		X.erase(X.begin());
		int i = 0;
		for (; i < A.size(); i++)
		{
			if (A[i] == x && Identity_A[i] == 0)
				break;
		}
		if (i == A.size()) {
			cout << "wrong"; return ;
		}
		while (i + 1 < A.size() && Identity_A[i + 1] == 0 && no_unmatched(B, A[i + 1],Identity_B))
		{
			S += A[i + 1];

			vector<char>::iterator s = find(X.begin(), X.end(), A[i+1]);
			if (s == X.end())
			{
				cout << "erase wrong"; 
				return;
			}
			X.erase(s);
			i++;
		}
		//int j = B.find(A[i + 1]);
		int j = 0;
		for (; j < Identity_B.size(); j++)
			if (B[j] == A[i + 1] && Identity_B[j] == Identity_A[i + 1])
				break;

		string t = B.substr(0, j) + S;
		t += B.substr(j, B.size() - j);
		B = t;

		cnt1 += S.size();
		if (ishead(i+1, Identity_A))
		{
			int left = j;
			int right = j+S.size();
			while (right + 1 < B.size() && Identity_B[right + 1] == Identity_B[right])
				right++;
			int color = Identity_B[j];//
			if (color == 0)
			{
				color = colorid;
				colorid++;
			}
			//插入串之前后移后面的部分
			for (int step2 = B.size() - 1; step2 > right; step2--)
				Identity_B[step2] = Identity_B[step2 - S.size()];

			for (int step = left; step <= right && step < Identity_B.size(); step++)//
				Identity_B[step] = color;

			int l2 = i + 1-S.size() ;
			int r2 = l2 + right - left;
			for (int step = l2; step <= r2&&step<Identity_A.size(); step++)//
				Identity_A[step] = color;
			cout << "color: " << color<<" S: "<<S << endl;
		}
		else
		{
			int left = j;
			int right = j+S.size();
			for (int step2 = B.size() - 1; step2 > right; step2--)
				Identity_B[step2] = Identity_B[step2 - S.size()];

			for (int step = left; step <= right; step++)
				Identity_B[step] = colorid;

			int l2 = i + 1 - S.size();
			int r2 = i+1;
			for (int step = l2; step <= r2; step++)
				Identity_A[step] = colorid;
			cout << "colorid: " << colorid << " S: " << S << endl;
			colorid++;
		}
		//for (auto x : A)
		//{
		//	cout.width(2);
		//	cout << x;
		//}
		//cout.width(2);
		//cout << endl;
		//for (auto x : Identity_A)
		//{
		//	cout.width(2);
		//	cout << x;
		//}
		//cout.width(2);
		//cout << endl<<endl;
		//for (auto x : B)
		//{
		//	cout.width(2);
		//	cout << x;
		//}
		//cout.width(2);
		//cout << endl;
		//for (auto x : Identity_B)
		//{
		//	cout.width(2);
		//	cout<< x;
		//}
		//cout.width(2);
		//cout << endl << endl;
	}
	M.clear();
	M.push_back("##");
	for (int i = 2; i < B.size(); i++)
	{//更新M
		if (Identity_B[i] == 0) continue;
		int j = i;
		while (j + 1 < B.size() && Identity_B[j + 1] == Identity_B[j])
		{
			j++;
		}
		if (j == i)continue;
		M.push_back(B.substr(i, j - i + 1));
		i = j;
	}
	//cout << A << endl;
	//cout << B << endl;
	//for (auto x : M)
	//	cout << x << " ";
	//cout << endl;
	cnt2 = get_duo(M);
	//由两种方法得到最终的duo-preservation数相等知算法实现正确
	cout << "duo:" << cnt1 << " " << cnt2;
}

bool is_good(char x, vector<int> Identity_A, vector<int> Identity_B,string A,string B,int &ia,int &ib)
{//判断当前位置的x是否是好的1-子串
	string S(1, x);
	vector<int> loc = search(A, S);
	int i = 0;
	for (; i < loc.size(); i++)
	{
		if (Identity_A[loc[i]] != 0)
			continue;
		string l_r = "";
		l_r=l_r+A[loc[i] - 1] + A[loc[i] + 1];
		vector<int> b_loc = search(B, l_r);
		int j = 0;
		for (; j < b_loc.size(); j++)
		{
			if (Identity_B[b_loc[j]] == Identity_A[loc[i] - 1] && Identity_B[b_loc[j] + 1] == Identity_A[loc[i] + 1])
				break;
		}
		if (b_loc.size() != 0 && j != b_loc.size())
		{
			ia = loc[i];
			ib = b_loc[j];
			return true;
		}
	}
	return false;
}

void ScaffoldFilling(string &A, string &B, vector<string> &M)
{//改进的SF-MIDP近似算法
	int m = M.size() - 2;
	vector<int> pi(m + 2);
	vector<int> b_l;
	vector<int> Identity_A(A.size()), Identity_B(A.size());//
	//对A、B分块
	Identity.clear();
	identified = 0;
	dfs(1, M, A, Identity_A);
	Identity_A = Identity;
	Identity.clear();
	identified = 0;
	dfs(1, M, B, Identity_B);
	Identity_B = Identity;
	if (Identity_A.empty())
	{//当前随机数据不可行,需重新生成
		cout << "identity_A is empty ,retry other seed" << endl;
		return;
	}
	if (Identity_B.empty())
	{//当前随机数据不可行,需重新生成
		cout << "identity_B is empty ,retry other seed" << endl;
		return;
	}
	Identity_A[0] = M.size();
	Identity_A[1] = M.size();
	Identity_B[0] = M.size();
	Identity_B[1] = M.size();

	vector<char>X;
	int vis[26];
	memset(vis, 0, sizeof(vis));
	for (auto i : A)
	{//生成缺失基因的多重集X
		if (vis[i - 'a'])
			continue;
		vis[i - 'a'] = 1;
		int n = count(A.begin(), A.end(), i) - count(B.begin(), B.end(), i);
		for (int k = 0; k < n; k++)
		{
			X.push_back(i);
		}
	}
	//cout << A << endl;
	//for (auto x : Identity_A)
	//	cout << x;
	//cout << endl;
	//cout << B << endl;
	//for (auto x : Identity_B)
	//	cout << x;
	//cout << endl << endl;
	int colorid = M.size()+1;
	int i = 0;
	//cout << "start is：" << endl;
	//for (auto x : A)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl;
	//for (auto x : Identity_A)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl << endl;
	//for (auto x : B)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl;
	//for (auto x : Identity_B)
	//{
	//	cout.width(2);
	//	cout << x;
	//}
	//cout.width(2);
	//cout << endl << endl;
	int cnt1, cnt2;
	cnt1 = get_duo(M);
	while (i < X.size())
	{//尽可能寻找好的1-子串
		char x = X[i];
		int ia, ib;//ia为A中a_i的下标， ib 为B中a_i-1的下标
		if (is_good(x,Identity_A, Identity_B,A,B,ia,ib))
		{
			string S(1, x);
			X.erase(X.begin() + i);
			string temp = B.substr(0, ib + 1) + S + B.substr(ib + 1,B.size()-ib-1);
			B = temp;

			int left = ib;
			int right = ib + 1;
			if (Identity_B[ib]!=0)//ai-1=t(I)
			{
				while (left - 1 >= 0 && Identity_B[left - 1] == Identity_B[ib])
					left--;
			}
			else;
			if (Identity_B[ib + 1] != 0)//ai+1=h(I)
			{
				while (right + 1 < Identity_B.size() && Identity_B[right + 1] == Identity_B[right])
					right++;
			}
			else;
			for (int step = Identity_B.size() - 1; step > ib; step--)//后移1个单位
				Identity_B[step] = Identity_B[step - 1];
			for (int step = left; step <= right+1; step++)//对B
				Identity_B[step] = colorid;
			//对A
			left = ia - 1;
			right = ia + 1;
			if(Identity_A[left]!=0)
				while (left - 1 >= 0 && Identity_A[left - 1] == Identity_A[ia-1])
					left--;
			if (Identity_A[right] != 0)
				while (right + 1 < Identity_A.size() && Identity_A[right + 1] == Identity_A[right])
					right++;
			for (int step = left; step <= right; step++)//对B
				Identity_A[step] = colorid;
			colorid++;
			cnt1+=2;

			cout << "x: " << x << " color : " << colorid - 1 << endl;
			for (auto x : A)
			{
				cout.width(2);
				cout << x;
			}
			cout.width(2);
			cout << endl;
			for (auto x : Identity_A)
			{
				cout.width(2);
				cout << x;
			}
			cout.width(2);
			cout << endl << endl;
			for (auto x : B)
			{
				cout.width(2);
				cout << x;
			}
			cout.width(2);
			cout << endl;
			for (auto x : Identity_B)
			{
				cout.width(2);
				cout << x;
			}
			cout.width(2);
			cout << endl << endl;

		}
		else i++;
	}

	M.clear();
	M.push_back("##");
	for (int i = 2; i < B.size(); i++)
	{//更新M
		if (Identity_B[i] == 0) continue;
		int j = i;
		while (j + 1 < B.size() && Identity_B[j + 1] == Identity_B[j])
		{
			j++;
		}
		if (j == i)continue;
		M.push_back(B.substr(i, j - i + 1));
		i = j;
	}

	cnt2 = get_duo(M);
	/*cout << "duo: " << cnt1 << " " << cnt2 << endl;
	cout << "________________________________________________________________" << endl<< "________________________________________________________________" << endl;*/
	BasicInsert(A, B, M);//算法剩余部分同算法2
}

int main()
{
	string A = "##ghefijabklmcd$$";
	string B = "##gabjcdlef$$";
	vector<string> M = { "##","ab","cd","ef","$$" };
	cout << time(NULL) << endl;
	srand(time(NULL));
	//eg:srand(1620626018);
	//1620630249 为算法3的一个实例
	getA_B_M(A, B, M);
	//cout << A << endl;
	//cout << B << endl;
	//for (auto x : M)
	//	cout << x << " ";
	//cout << endl << endl;

	//BasicInsert(A, B, M);算法2
	ScaffoldFilling(A, B, M);//算法3
}
