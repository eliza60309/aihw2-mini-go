#include <iostream>
#include <utility>
#include <vector>
#include <cstring>

using namespace std;

int enemybreath(bool[][5], short[][5], int, pair<int, int>, vector<pair<int, int> > *);
int capture(short[][5], int, pair<int, int>, pair<int, int> *);
int allybreath(bool[][5], short[][5], int, pair<int, int>);
vector<pair<int, int> > get_legal(short[][5], int, bool, pair<int, int>);

int main()
{
	short board[5][5] = {};//yx
	int phase = 1;//1: black 2: white
	int step = 0;
	bool ko = false;
	pair<int, int>ko_pos;
	while(1)
	{
		cout << "----- Step " << step << " -----" << endl;
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
				cout << board[i][j] << " ";
			cout << endl;
		}
		int x, y;
		if(phase == 1)
			cout << "Black's Turn: ";
		else
			cout << "White's Turn: ";
		cin >> x >> y;

		vector<pair<int, int> >legal_moves = get_legal(board, phase, ko, ko_pos);
		bool legal = false;
		for(int i = 0; i < legal_moves.size(); i++)
		{
			if(legal_moves[i].first == y && legal_moves[i].second == x)
			{
				legal = true;
				break;
			}
		}
		if(legal)
		{
			pair<int, int> new_ko_pos;
			int captured = capture(board, phase, pair<int, int>(y, x), &new_ko_pos);
			if(captured == 1)
			{
				ko_pos = new_ko_pos;
				ko = true;
			}
			else 
				ko = false;
			board[y][x] = phase;
			phase = (phase == 1? 2: 1);
		}
		else
			cout << ">>>>> ILLEGAL MOVE!! >>>>>" << endl;
	}
}


int capture(short board[][5], int phase, pair<int, int>pos, pair<int, int> *ko_pos)
{
	phase = (phase == 1? 2: 1);
	bool counted[5][5] = {};
	vector<pair<int, int> >cluster;
	int captured = 0;
	if(pos.first > 0 && board[pos.first - 1][pos.second] == phase)
		if(enemybreath(counted, board, phase, pair<int, int>(pos.first - 1, pos.second), &cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
			if(captured == 1)
				*ko_pos = cluster[0];
			//memset(counted, 0, sizeof(bool) * 25);
			cluster.clear();
		}
	if(pos.first < 4 && board[pos.first + 1][pos.second] == phase && !counted[pos.first + 1][pos.second])
		if(enemybreath(counted, board, phase, pair<int, int>(pos.first + 1, pos.second), &cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
			if(captured == 1)
				*ko_pos = cluster[0];
			//memset(counted, 0, sizeof(bool) * 25);
			cluster.clear();
		}
	if(pos.second > 0 && board[pos.first][pos.second - 1] == phase && !counted[pos.first][pos.second - 1])
		if(enemybreath(counted, board, phase, pair<int, int>(pos.first, pos.second - 1), &cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
			if(captured == 1)
				*ko_pos = cluster[0];
			//memset(counted, 0, sizeof(bool) * 25);
			cluster.clear();
		}
	if(pos.second < 4 && board[pos.first][pos.second + 1] == phase && !counted[pos.first][pos.second + 1])
		if(enemybreath(counted, board, phase, pair<int, int>(pos.first, pos.second + 1), &cluster) == 1)
		{
			for(int i = 0; i < cluster.size(); i++)
				board[cluster[i].first][cluster[i].second] = 0;
			captured += cluster.size();
			if(captured == 1)
				*ko_pos = cluster[0];
		}
	return captured;
}

int enemybreath(bool counted[][5], short board[][5], int phase, pair<int, int>pos, vector<pair<int, int> > *cluster)//phase of enemy
{
	counted[pos.first][pos.second] = true;
	(*cluster).push_back(pos);
	int sum = 0;
	if(pos.first > 0 && !counted[pos.first - 1][pos.second])//allow up
	{
		if(!board[pos.first - 1][pos.second])
			sum += 1;
		else if(board[pos.first - 1][pos.second] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first - 1, pos.second), cluster);
	}
	if(pos.first < 4 && !counted[pos.first + 1][pos.second])//allow down
	{
		if(!board[pos.first + 1][pos.second])
			sum += 1;
		else if(board[pos.first + 1][pos.second] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first + 1, pos.second), cluster);
	}
	if(pos.second > 0 && !counted[pos.first][pos.second - 1])//allow left
	{
		if(!board[pos.first][pos.second - 1])
			sum += 1;
		else if(board[pos.first][pos.second - 1] == phase)
			sum += enemybreath(counted, board, phase, pair<int, int>(pos.first, pos.second - 1), cluster);
	}
	if(pos.second < 4 && !counted[pos.first][pos.second + 1])//allow right
	{
		if(!board[pos.first][pos.second + 1])
			sum += 1;
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
			sum += 1;
		else if(board[pos.first - 1][pos.second] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first - 1, pos.second));
	}
	if(pos.first < 4 && !counted[pos.first + 1][pos.second])//allow down
	{
		if(!board[pos.first + 1][pos.second])
			sum += 1;
		else if(board[pos.first + 1][pos.second] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first + 1, pos.second));
	}
	if(pos.second > 0 && !counted[pos.first][pos.second - 1])//allow left
	{
		if(!board[pos.first][pos.second - 1])
			sum += 1;
		else if(board[pos.first][pos.second - 1] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first, pos.second - 1));
	}
	if(pos.second < 4 && !counted[pos.first][pos.second + 1])//allow right
	{
		if(!board[pos.first][pos.second + 1])
			sum += 1;
		else if(board[pos.first][pos.second + 1] == phase)
			sum += allybreath(counted, board, phase, pair<int, int>(pos.first, pos.second + 1));
	}
	return sum;
}

vector<pair<int, int> > get_legal(short board[][5], int phase, bool ko, pair<int, int> ko_pos)
{
	vector<pair<int, int> >legal;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(!board[i][j])
			{
				if(ko && i == ko_pos.first && j == ko_pos.second)
					continue;
				bool counted[5][5] = {};
				short tmpboard[5][5];
				pair<int, int> dummy;
				memcpy(tmpboard, board, sizeof(short) * 25);
				capture(tmpboard, phase, pair<int, int>(i, j), &dummy);
				if(!allybreath(counted, tmpboard, phase, pair<int, int>(i, j)))
					continue;
				legal.push_back(pair<int, int>(i, j));
			}
		}
	}
	return legal;
}
