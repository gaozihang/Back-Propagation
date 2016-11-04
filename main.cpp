#include <iostream>
#include <time.h>
using namespace std;

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a > b ? b : a)

#define MAX_LEVEL_COUNT 10 
#define MAX_PER_LEVEL_COUNT 10  
#define MAX_DATA_COUNT 100
#define PREC 0.1 
const double sv = 5; 
const double a = 1.0; 
int level_count; 
int per_level_count[MAX_LEVEL_COUNT];
double w[MAX_LEVEL_COUNT][MAX_PER_LEVEL_COUNT][MAX_PER_LEVEL_COUNT]; 
double y[MAX_LEVEL_COUNT][MAX_LEVEL_COUNT];
double v[MAX_LEVEL_COUNT][MAX_LEVEL_COUNT];
double d; 
double lg[MAX_LEVEL_COUNT][MAX_LEVEL_COUNT]; 

int data_count;
double input_data[MAX_DATA_COUNT][MAX_PER_LEVEL_COUNT];
double input_result[MAX_PER_LEVEL_COUNT];
double max_value[MAX_PER_LEVEL_COUNT];
double min_value[MAX_PER_LEVEL_COUNT];

double diff_func(double v);
void print_w();
void print_lg();
void print_output();
void input_value();

void init_paramter()
{
	cin >> level_count;

	for (int i = 0; i < level_count; i++)
		cin >> per_level_count[i];

	srand(time(0));
	for (int i = 0; i < level_count - 1; i++)
		for (int j = 0; j < per_level_count[i]; j++)
			for (int k = 0; k < per_level_count[i + 1]; k++)
			{
			//	cin >> w[i][j][k];
				w[i][j][k] = 2.0 * rand() / RAND_MAX - 1.0;	
			}
}

void forward_calc()
{
	for (int i = 1; i < level_count; i++)
		for (int j = 0; j < per_level_count[i]; j++)
		{
			v[i][j] = 0;
			for (int k = 0; k < per_level_count[i - 1]; k++)
				v[i][j] += y[i - 1][k] * w[i - 1][k][j];
			y[i][j] = 1 / (1 + exp(-1 * a * v[i][j]));
		}
}

void back_calc()
{
	double delt_w;
	for (int c = level_count - 1; c > 0; c--) 
	{
		if (c == level_count - 1)  
		{
			double e = d - y[c][0]; 
			// diff_func() 
			lg[c][0] = e * a * y[c][0] * (1 - y[c][0]);
			for (int j = 0; j < per_level_count[c - 1]; j++)
			{
				delt_w = sv * lg[c][0] * y[c - 1][j];
				w[c - 1][j][0] += delt_w;
			}
		}
		else 
		{
			for (int i = 0; i < per_level_count[c]; i++)
			{ 
				double added = 0;
				for (int j = 0; j < per_level_count[c + 1]; j++)
					added += lg[c + 1][j] * w[c][i][j];
				lg[c][i] = a * y[c][i] * (1 - y[c][i]) *added;
				for (int j = 0; j < per_level_count[c - 1]; j++)
				{
					delt_w = sv * lg[c][i] * y[c-1][j];
					w[c - 1][j][i] += delt_w;
				}
			}
		}
	}
}

int main()
{
	double err;
	init_paramter();
	print_w();
	print_lg();
	input_value();
	while (1)
	{
		err = 0;
		for (int i = 0; i < data_count; i++)
		{
			for (int j = 0; j < per_level_count[0]; j++)
				y[0][j] = input_data[0][j];
			d = input_result[i];
//			cout << i << " : " << endl;
			forward_calc();
//			print_output();
			back_calc();
			forward_calc();
//			print_output();
			err += pow(y[level_count - 1][0] - d,2);
		//	print_w();
		//	print_lg();
		}
		cout << "err : " <<err << endl;
		if (err < PREC)
			break;
	}

	int t, w, h;
	while (cin >> t >> w >> h)
	{
		forward_calc();
		print_output();
	}
	return 0;
}

void print_w()
{
	cout << "w value is :" << endl;
	for(int i = 0; i < level_count - 1; i++)
		for (int j = 0; j < per_level_count[i]; j++)
			for (int k = 0; k < per_level_count[i + 1]; k++)
				cout << "level" << i << " : " << j << "--" << k << " : " << w[i][j][k] << endl;
}

void print_lg()
{
	cout << "lg value is :" << endl;
	for (int i = 0; i < level_count; i++)
		for (int j = 0; j < per_level_count[i]; j++)
			cout << "[" << i << "," << j << "] : " << lg[i][j] << endl;
}

void print_output()
{
	for (int i = 0; i < per_level_count[level_count - 1]; i++)
		cout << "output " << i << " : " << y[level_count - 1][i] << endl;
}

void input_value()
{
	cin >> data_count;

	for (int j = 0; j < per_level_count[0]; j++)
	{
		max_value[j] = -99999999;
		min_value[j] =  99999999;
	}
	for (int i = 0; i < data_count; i++)
	{
		for (int j = 0; j < per_level_count[0]; j++)
		{
			cin >> input_data[i][j];
			max_value[j] = MAX(max_value[j], input_data[i][j]);
			min_value[j] = MIN(min_value[j], input_data[i][j]);
		}
		cin >> input_result[i];
	}

	for (int i = 0; i < data_count; i++)
		for (int j = 0; j < per_level_count[0]; j++)
			input_data[i][j] = (input_data[i][j] - min_value[j]) / (max_value[j] - min_value[j]);
}