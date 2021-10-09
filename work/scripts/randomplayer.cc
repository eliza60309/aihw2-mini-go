#include <bits/stdc++.h>

using namespace std;

int enemybreath(bool[][5], short[][5], int, pair<int, int>, vector<pair<int, int> > &);
int capture(short[][5], int, pair<int, int>);
int allybreath(bool[][5], short[][5], int, pair<int, int>);
int get_legal(short[][5], short[][5], int, vector<pair<int, int> >&);

int main()
{
	short previous[5][5] = {}; 
	short board[5][5] = {};//yx
	int phase = 1;//1: black 2: white
	int step = 0;
	int player;
	fstream in, log;
	//log.open("log.txt", fstream::out | fstream::ate | fstream::app);
	//log << "GAME START" << endl;
	//log.close();
	while(!in.is_open())
		in.open("input.txt", fstream::in);
	in >> player;
	//log.open("log.txt", fstream::out | fstream::ate | fstream::app);
	//log << "I AM " << player << endl;
	//log.close();
	in.close();
	if(player == 2)
		step = 2;
	else
		step = 1;
	//while(1)
	{
		while(!in.is_open())
			in.open("input.txt", fstream::in);
		in >> player;
		//log.open("log.txt", fstream::out | fstream::ate | fstream::app);
		int x, y;
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
		remove("input.txt");
		/*if(step - 1 > 0)
		{
			log << "----- Step " << step - 1 << " -----" << endl;
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 5; j++)
					log << previous[i][j] << " ";
				log << endl;
			}
		}
		log << "----- Step " << step << " -----" << endl;
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
				log << board[i][j] << " ";
			log << endl;
		}
		log.close();*/
		phase = player;
		vector<pair<int, int> >legal_moves;
		get_legal(board, previous, phase, legal_moves);
		if(legal_moves.size())
		{
			srand(time(NULL));
			int move = rand() % legal_moves.size();
			fstream out;
			out.open("output.txt", fstream::out);
			out << legal_moves[move].first << "," << legal_moves[move].second;
			step += 2;
			out.close();
		}
		else
		{
			fstream out;
			out.open("output.txt", fstream::out);
			out << "PASS";
			step += 2;
			out.close();
		}
	}
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
					diff = false;
					for(int k = 0; k < 5; k++)
					{
						for(int l = 0; l < 5; l++)
						{
							if(tmpboard[k][l] != previous[k][l])
							{
								diff = true;
								break;
							}
						}
						if(diff)
							break;
					}
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
