#ifndef BurstyEvent_H
#define BurstyEvent_H
#include <bits/stdc++.h>
#include "../../util/BOBHash32.h"
#include <cmath>
#include <algorithm> 
class BurstyCounter
{
	public:
	vector<int> V;
	vector<pair<int, int> > newV;
	BurstyCounter()
	{
		V.clear();
	}
	void insert(uint32_t time)
	{
		V.push_back((int) time);
	}
	void cal(int b, int end)
	{
		newV.clear();
		if(b > (int)V.size())
			b = V.size();
		V.push_back(end);
		int *s = new int[V.size()];
		int **f = (int **) new int*[b + 1];
		for(int i = 0; i <= b; i++)
			f[i] = new int[V.size()];
		int **prv = (int **) new int*[b + 1];
		for(int i = 0; i <= b; i++)
			prv[i] = new int[V.size()];
		
		s[0] = 0;
		for(int i = 1; i < (int)V.size(); i++)
			s[i] = s[i - 1] + i * (V[i] - V[i - 1]);
		for(int i = 0; i < (int)V.size(); i++)
			f[0][i] = s[i];
		printf("%d\n", (int)V.size());
		for(int i = 1; i <= b; i++)
			for(int j = 1; j < (int)V.size(); j++)
			{
				f[i][j] = 1000000000;
				for(int k = i; k <= j; k++)
				{
					int tmp = f[i - 1][k - 1] + s[j] - s[k] - k * (V[j] - V[k]);
					if(tmp < f[i][j])
					{
						f[i][j] = tmp;
						prv[i][j] = k;
					}
				}
				//cout<<i<<" "<<j<<" "<<f[i][j]<<endl;
			}
		int cnt = b, now = V.size() - 1;
		while(cnt)
		{
			newV.push_back(make_pair(V[prv[cnt][now] - 1], prv[cnt][now]));
			now = prv[cnt][now] - 1;
			cnt--;
		}
		V.clear();
		delete s;
		delete f;
		delete prv;
		
		//cout<<"BurstyCounter"<<endl;
		//for(int i = 0; i < newV.size(); i++)
		//	cout<<newV[i].first<<" "<<newV[i].second<<endl;
	}
	int query(int time)
	{
		for(int i = 0; i < (int)newV.size(); i++)
		{
			if(newV[i].first <= time)
				return newV[i].second;
		}
		return 0;
	}
	void print()
	{
		cout<<"BurstyCounter"<<endl;
		for(int i = 0; i < (int)newV.size(); i++)
			cout<<newV[i].first<<" "<<newV[i].second<<endl;
	}
};

class BurstySketch
{
	public:
	BurstyCounter*** c;
	int d, size;
	BOBHash32* bobhash[10];
	BurstySketch(int _d, int _size)
	{
		d = _d;
		size = _size;
		for(int i = 0; i < d; i++)
			bobhash[i] = new BOBHash32(i + 103);
		c = (BurstyCounter ***)new BurstyCounter**[d];
		for(int i = 0; i < d; i++)
			c[i] = new BurstyCounter*[size];
		for(int i = 0; i < d; i++)
			for(int j = 0; j < size; j++)
				c[i][j] = new BurstyCounter();
	}
	void insert(uint64_t id, uint32_t time)
	{
		for(int i = 0; i < d; i++)
		{
			int z = bobhash[i]->run((char *)&id, 8) % size;
			c[i][z]->insert(time);
		}
	}
	void cal(int b, int end)
	{
		printf("%d %d\n", d, size);
		for(int i = 0; i < d; i++)
			for(int j = 0; j < size; j++)
				c[i][j]->cal(b, end);
	}
	int query(uint64_t id, int time)
	{
		if(time < 0)
			return 0;
		int* D;
		D = new int[d];
		for(int i = 0; i < d; i++)
		{
			int z = bobhash[i]->run((char *)&id, 8) % size;
			D[i] = c[i][z]->query(time);
		}
		sort(D, D + d);
		int tmp = D[d / 2];
		delete []D;
		return tmp;
	}
};
#endif
