#include <bits/stdc++.h>

using namespace std;

int enemybreath(bool[][5], short[][5], int, pair<int, int>, vector<pair<int, int> > &);
int capture(short[][5], int, pair<int, int>);
int allybreath(bool[][5], short[][5], int, pair<int, int>);
int get_legal(short[][5], short[][5], int, vector<pair<int, int> >&);
int diffboard(short board1[][5], short board2[][5]);
int move(pair<int, int>);
int eval(short[][5]);

int main()
{
	short previous[5][5] = {}; 
	short board[5][5] = {};//yx
	int player;
	bool passed = false;
	fstream in;
	while(!in.is_open())
		in.open("input.txt", fstream::in);
	in >> player;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			char c = 0;
			while(c < '0' || c > '9')
				in >> c;
			previous[i][j] = c - '0';
		}
	}
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			char c = 0;
			while(c < '0' || c > '9')
				in >> c;
			board[i][j] = c - '0';
		}
	}
	in.close();
	if(!diffboard(board, previous) && eval(board) == player)
	{
		move(pair<int, int>(-1, -1));
		return 0;
	}
	vector<pair<int, int> >legal_moves;
	get_legal(board, previous, player, legal_moves);
	if(legal_moves.size())
	{
		srand(time(NULL));
		int i = rand() % legal_moves.size();
		move(legal_moves[i]);
	}
	else
		move(pair<int, int>(-1, -1));
}


int capture(short board[][5], int phase, pair<int, int>pos)
{
	phase = (phase == 1? 2: 1);
	bool counted[4][5][5] = {};
	vector<pair<int, int> >cluster;
	int captured = 0;
	if(pos.first > 0 && board[pos.first - 1][pos.second] == phase)
	{
		if(enemybreath(counted[0], board, phase, pair<int, int>(pos.first - 1, pos.second), cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
		}
		cluster.clear();
	}
	if(pos.first < 4 && board[pos.first + 1][pos.second] == phase)
	{
		if(enemybreath(counted[1], board, phase, pair<int, int>(pos.first + 1, pos.second), cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
		}
		cluster.clear();
	}
	if(pos.second > 0 && board[pos.first][pos.second - 1] == phase)
	{
		if(enemybreath(counted[2], board, phase, pair<int, int>(pos.first, pos.second - 1), cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
		}
		cluster.clear();
	}
	if(pos.second < 4 && board[pos.first][pos.second + 1] == phase)
	{
		if(enemybreath(counted[3], board, phase, pair<int, int>(pos.first, pos.second + 1), cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
		}
	}
	return captured;
}

int enemybreath(bool counted[][5], short board[][5], int phase, pair<int, int>pos, vector<pair<int, int> > &cluster)//phase of enemy
{
	counted[pos.first][pos.second] = true;
	cluster.push_back(pos);
	int sum = 0;
	if(pos.first > 0 && !counted[pos.first - 1][pos.second])//allow up
	{
		if(!board[pos.first - 1][pos.second])
		{
			counted[pos.first - 1][pos.second] = true;
			sum += 1;
		}
		else if(board[pos.first - 1][pos.second] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first - 1, pos.second), cluster);
	}
	if(pos.first < 4 && !counted[pos.first + 1][pos.second])//allow down
	{
		if(!board[pos.first + 1][pos.second])
		{
			counted[pos.first + 1][pos.second] = true;
			sum += 1;
		}
		else if(board[pos.first + 1][pos.second] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first + 1, pos.second), cluster);
	}
	if(pos.second > 0 && !counted[pos.first][pos.second - 1])//allow left
	{
		if(!board[pos.first][pos.second - 1])
		{
			counted[pos.first][pos.second - 1] = true;
			sum += 1;
		}
		else if(board[pos.first][pos.second - 1] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first, pos.second - 1), cluster);
	}
	if(pos.second < 4 && !counted[pos.first][pos.second + 1])//allow right
	{
		if(!board[pos.first][pos.second + 1])
		{
			counted[pos.first][pos.second + 1] = true;
			sum += 1;
		}
		else if(board[pos.first][pos.second + 1] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first, pos.second + 1), cluster);
	}
	return sum;
}

int allybreath(bool counted[][5], short board[][5], int phase, pair<int, int>pos)//1: up, 2: down, 3: left, 4: right
{
	counted[pos.first][pos.second] = true;
	int sum = 0;
	if(pos.first > 0 && !counted[pos.first - 1][pos.second])//allow up
	{
		if(!board[pos.first - 1][pos.second])
		{
			counted[pos.first - 1][pos.second] = true;
			sum += 1;
		}
		else if(board[pos.first - 1][pos.second] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first - 1, pos.second));
	}
	if(pos.first < 4 && !counted[pos.first + 1][pos.second])//allow down
	{
		if(!board[pos.first + 1][pos.second])
		{
			counted[pos.first + 1][pos.second] = true;
			sum += 1;
		}
		else if(board[pos.first + 1][pos.second] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first + 1, pos.second));
	}
	if(pos.second > 0 && !counted[pos.first][pos.second - 1])//allow left
	{
		if(!board[pos.first][pos.second - 1])
		{
			counted[pos.first][pos.second - 1] = true;
			sum += 1;
		}
		else if(board[pos.first][pos.second - 1] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first, pos.second - 1));
	}
	if(pos.second < 4 && !counted[pos.first][pos.second + 1])//allow right
	{
		if(!board[pos.first][pos.second + 1])
		{
			counted[pos.first][pos.second + 1] = true;
			sum += 1;
		}
		else if(board[pos.first][pos.second + 1] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first, pos.second + 1));
	}
	return sum;
}

int get_legal(short board[][5], short previous[][5], int phase, vector<pair<int, int> > &legal)
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(!board[i][j])
			{
				bool counted[5][5] = {};
				short tmpboard[5][5];
				pair<int, int> dummy;
				memcpy(tmpboard, board, sizeof(short) * 25);
				int diff = true;
				if(capture(tmpboard, phase, pair<int, int>(i, j)) == 1)
				{
					tmpboard[i][j] = phase;
					diff = diffboard(tmpboard, previous);
				}
				if(!diff)
					continue;
				if(!allybreath(counted, tmpboard, phase, pair<int, int>(i, j)))
					continue;
				legal.push_back(pair<int, int>(i, j));
			}
		}
	}
	return 0;
}

int diffboard(short board1[][5], short board2[][5])
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(board1[i][j] != board2[i][j])
				return true;
		}
	}
	return false;
}

int eval(short board[][5])
{
	int black = 0, white = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			if(board[i][j] == 1)
				black++;
			else if(board[i][j] == 2)
				white++;
	}
	if((double)white + 2.5 > black)
		return 2;
	else 
		return 1;
}

int move(pair<int, int> m)
{
	fstream out;
	out.open("output.txt", fstream::out);
	if(m.first == -1 || m.second == -1)
		out << "PASS";
	else
		out << m.first << "," << m.second;
	out.close();
	return 0;
}
