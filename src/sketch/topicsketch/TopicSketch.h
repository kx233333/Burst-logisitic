#ifndef TopicSketch_H
#define TopicSketch_H
#include <bits/stdc++.h>
#include "../../util/BOBHash32.h"
#include <cmath>
class Velocity
{
	public:
	uint32_t time;
	double v;
	uint32_t deltaT;
	Velocity(uint32_t _deltaT)
	{
		deltaT = _deltaT;
		time = 0;
		v = 0.0;
	}
	void insert(uint32_t t)
	{
		v = v * exp(1.0 * ((int) time - (int)t) / deltaT) + 1.0 / deltaT;
		time = t;
	}
	void update(uint32_t t)
	{
		v = v * exp(1.0 * ((int) time - (int) t) / deltaT);
		time = t;
	}
	double query(uint32_t t)
	{
		update(t);
		return v;
	}
};

class TopicSketch
{
	public:
	int d, size;
	uint32_t deltaT1, deltaT2;
	Velocity*** c1;
	Velocity*** c2;
	BOBHash32* bobhash[10];
	TopicSketch(int _d, int _size, uint32_t _deltaT1, uint32_t _deltaT2)
	{
		d = _d;
		size = _size;
		for(int i = 0; i < d; i++)
			bobhash[i] = new BOBHash32(i + 103);
		deltaT1 = _deltaT1;
		deltaT2 = _deltaT2;
		c1 = (Velocity ***)new Velocity**[d];
		c2 = (Velocity ***)new Velocity**[d];
		for(int i = 0; i < d; i++)
		{
			c1[i] = new Velocity*[size];
			c2[i] = new Velocity*[size];
		}
		for(int i = 0; i < d; i++)
			for(int j = 0; j < size; j++)
			{
				c1[i][j] = new Velocity(deltaT1);
				c2[i][j] = new Velocity(deltaT2);
			}
	}
	void insert(uint64_t id, uint32_t time)
	{
		for(int i = 0; i < d; i++)
		{
			int z = bobhash[i]->run((char *)&id, 8) % size;
			c1[i][z]->insert(time);
			c2[i][z]->insert(time);
		}
	}
	double query(uint64_t id, uint32_t time)
	{
		double v1 = 1000000.0, v2 = 1000000.0;
		for(int i = 0; i < d; i++)
		{
			int z = bobhash[i]->run((char *)&id, 8) % size;
			v1 = min(v1, c1[i][z]->query(time));
			v2 = min(v2, c2[i][z]->query(time));
		}
		return (v2 - v1) / (deltaT1 - deltaT2);
	}
};

#endif
