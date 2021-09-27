#include <iostream>
#include <utility>
#include <vector>

using namespace std;

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
			board[y][x] = phase;
			phase = (phase == 1? 2: 1);
		}
		else
			cout << ">>>>> ILLEGAL MOVE!! >>>>>" << endl;
	}
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
			if(ko && i == ko_pos.first && j == ko_pos.second)
				continue;
			if(!board[i][j])
			{
				bool counted[5][5] = {};
				if(!allybreath(counted, board, phase, pair<int, int>(i, j)))
					continue;
				legal.push_back(pair<int, int>(i, j));
			}
		}
	}
	return legal;
}
