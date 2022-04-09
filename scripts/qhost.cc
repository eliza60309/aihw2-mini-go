//#include <iostream>
//#include <utility>
//#include <vector>
//#include <cstring>
//#include <fstream>
#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

int enemybreath(bool[][5], short[][5], int, pair<int, int>, vector<pair<int, int> > &);
int capture(short[][5], int, pair<int, int>);
int allybreath(bool[][5], short[][5], int, pair<int, int>);
int get_legal(short[][5], short[][5], int, vector<pair<int, int> >&);
int show_board(short[][5]);
int call(int, short[][5], short[][5], pair<int, int> &, string, string, string);
string encode(short[][5]);
bool exists(const string &);

#define ALPHA 0.7
#define GAMMA 0.9
#define WIN_REWARD 1000


class qmove
{
	public:
	pair<int, int>move;
	double val;
};

class qstate
{
	public:
	qstate(){moves = vector<qmove>();}
	string state;
	vector<qmove> moves;
};

vector<qstate>::iterator get_qstate(vector<qstate> &, string &);
vector<qmove>::iterator get_qmove(vector<qmove> &, pair<int, int> &);
int load(qstate &myqstate, string &dir);
int save(qstate &myqstate, string &dir);

int main(int argc, char *argv[])
{
	short previous[5][5] = {}; 
	short board[5][5] = {};//yx
	int phase = 1;//1: black 2: white
	int step = 0;
	int passed = false;
	int endgame = 0;
	int victory = 0;
	int black = 0, white = 0;
	vector<string> states[2];
	vector<pair<int, int> >moves[2];
	int movecnt[2] = {};
	//fstream p1q, p2q;
	//if(p1q.open("p1q", fstream::in))
	while(!victory)
	{
		if(step >= 24 || endgame)
		{
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 5; j++)
					if(board[i][j] == 1)
						black++;
					else if(board[i][j] == 2)
						white++;
			}
			if((double)white + 2.5 > black)
				victory = 2;
			else 
				victory = 1;
			endgame = true;
			continue;
		}
		//cout << "----- Step " << step << ", Player " << phase << " -----" << endl;
		//show_board(board);
		pair<int, int> move;
		if(phase == 1)
			call(1, board, previous, move, "output.txt", "input.txt", argv[1]);
		else 
			call(2, board, previous, move, "output.txt", "input.txt", argv[2]);
		int x = move.second;
		int y = move.first;
		vector<pair<int, int> >legal_moves;
		get_legal(board, previous, phase, legal_moves);
		bool legal = false;
		if(y == -1)
		{
			if(passed)
				endgame = true;
			else
			{
				states[phase - 1].push_back(encode(board));
				moves[phase - 1].push_back(pair<int, int>(-1, -1));
				passed = true;
				movecnt[phase - 1]++;
				step++;
				phase = (phase == 1? 2: 1);
			}
			continue;
		}
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
			states[phase - 1].push_back(encode(board));
			moves[phase - 1].push_back(pair<int, int>(y, x));
			for(int i = 0; i < 5; i++)
				for(int j = 0; j < 5; j++)
					previous[i][j] = board[i][j];
			int captured = capture(board, phase, pair<int, int>(y, x));
			board[y][x] = phase;
			movecnt[phase - 1]++;
			phase = (phase == 1? 2: 1);
			passed = false;
			step++;
		}
		else
		{
			//cout << ">>>>> ILLEGAL MOVE!! >>>>>" << endl;
			victory = (phase == 1? 2: 1);
			endgame = true;
		}
	}
	string datadir = (argc <= 3? "data": argv[3]);
	if(datadir[datadir.length() - 1] != '/')
		datadir += '/';
	string winnerdir = (victory == 1? "black/": "white/");
	double max_q;
	for(int i = moves[victory - 1].size() - 1; i >= 0; i--)
	{
		qstate cur_qstate, tmpstate;
		cur_qstate.state = states[victory - 1][i];
		tmpstate.state = states[victory - 1][i];
		//get qstate
		//vector<qstate>::iterator i_qstate = get_qstate(qstates, states[victory - 1][i]);
		string filedir = datadir + winnerdir + "step" + to_string(i + 1) + "/" + states[victory - 1][i] + ".txt";
		//load file
		if(exists(filedir))
			load(cur_qstate, filedir);
		//get qmove
		vector<qmove>::iterator i_qmove = get_qmove(cur_qstate.moves, moves[victory - 1][i]);
		//calc reward
		if(i == moves[victory - 1].size() - 1)
			i_qmove->val = WIN_REWARD;
		else
			i_qmove->val = i_qmove->val * (1.0 - ALPHA) + ALPHA * GAMMA * max_q;
		max_q = 0;
		for(int j = 0; j < cur_qstate.moves.size(); j++)
		{
			if(cur_qstate.moves[j].val > max_q)
				max_q = cur_qstate.moves[j].val;
		}
		save(cur_qstate, filedir);
		load(tmpstate, filedir);
		cout << tmpstate.state << ":" << endl;
		for(vector<qmove>::iterator j = tmpstate.moves.begin(); j != tmpstate.moves.end(); j++)
			cout << "(" << j->move.first << "," << j->move.second << "): " << j->val << endl;
		
	}
	
	//display the qstate table
	/*for(vector<qstate>::iterator i = qstates.begin(); i != qstates.end(); i++)
	{
		cout << i->state << ":" << endl;
		for(vector<qmove>::iterator j = i->moves.begin(); j != i->moves.end(); j++)
		{
			cout << "(" << j->move.first << "," << j->move.second << "): " << j->val << endl;
		}
	}*/
	cout << "PLAYER " << victory << " WON with " << step << " steps" << endl;
	//cout << "Black: " << black << " White: " << white << endl;
	//cout << "BLACK: " << movecnt[0] << " WHITE: " << movecnt[1] << endl;
}

int load(qstate &myqstate, string &dir)
{
	fstream in;
	in.open(dir.c_str(), fstream::in);
	int n;
	in >> n;
	pair<int, int> move;
	double value;
	qmove q;
	for(int i = 0; i < n; i++)
	{
		in >> move.first >> move.second >> q.val;
		q.move = move;
		myqstate.moves.push_back(q);
	}
	in.close();
	return 0;
}

int save(qstate &myqstate, string &dir)
{
	fstream out;
	out.open(dir.c_str(), fstream::out);
	out << myqstate.moves.size() << endl;
	for(int i = 0; i < myqstate.moves.size(); i++)
	{
		out << myqstate.moves[i].move.first << " ";
		out << myqstate.moves[i].move.second << " ";
		out << myqstate.moves[i].val << endl;
	}
	out.close();
	return 0;
}


bool exists(const string &s)
{
	struct stat buf;
	return !stat(s.c_str(), &buf);
}

string encode(short board[][5])
{
	string s;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			s.push_back(board[i][j] + '0');
	}
	return s;
}

vector<qmove>::iterator get_qmove(vector<qmove> &qmoves, pair<int, int> &move)
{
	vector<qmove>::iterator i;
	for(i = qmoves.begin(); i != qmoves.end(); i++)
	{
		if(i->move.first == move.first && i->move.second == move.second)
			break;
	}
	if(i == qmoves.end())
	{
		qmove newqmove;
		newqmove.move = move;
		newqmove.val = 0;
		qmoves.push_back(newqmove);
		i = qmoves.end();
		i--;
	}
	return i;
}

vector<qstate>::iterator get_qstate(vector<qstate> &qstates, string &state)
{
	int qstate_id = -1;
	vector<qstate>::iterator i;
	for(i = qstates.begin(); i != qstates.end(); i++)
	{
		if(i->state == state)
			break;
	}
	if(i == qstates.end())
	{
		qstate newqstate;
		newqstate.state = state;
		qstates.push_back(newqstate);
		i = qstates.end();
		i--;
	}
	return i;
}

int call(int player, short board[][5], short previous[][5], pair<int, int> &ans, string input, string output, string cmd)
{
	fstream in;
	fstream out;
	out.open(output.c_str(), fstream::out);
	out << player << endl;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			out << previous[i][j];
		out << endl;
	}
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			out << board[i][j];
		out << endl;
	}
	out.close();
	system(cmd.c_str());
	while(!in.is_open())
		in.open(input.c_str(), fstream::in);
	char c[3];
	in >> c[0] >> c[1] >> c[2];
	if(c[1] != ',')
		ans.first = -1;
	else
	{
		ans.first = c[0] - '0';
		ans.second = c[2] - '0';
	}
	in.close();
	remove(input.c_str());
	return 0;
}

int show_board(short board[][5])
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			cout << board[i][j] << " ";
		cout << endl;
	}
	return 0;
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
