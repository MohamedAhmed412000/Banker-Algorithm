#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

bool compare(int *arr1, int *arr2, int m){
	for (int i = 0; i < m; i++){
		if (arr1[i] > arr2[i])
			return false;
	}
	return true;
}
bool Safe_State(string &s, bool *finish, int **need,  int **allocation, int *work, int n, int m, int k, string str){
	int num = 1;
	bool flg = false;
	s += "<";
	if (str == "req"){
		s += ("P"+to_string(k)+"-req, ");
	}
	for (int i = 0; i < n; i++){
		if (finish[i] == false && compare(need[i], work, m)){
			for (int j = 0; j < m; j++)
					work[j] += allocation[i][j];
			finish[i] = true;
			s += ("P"+to_string(i));
			if(num < 5){
				s += ", ";
				num++;
			}
			i = -1;
		}
	}
	s += ">";
	for (int i = 0; i < n; i++){
		if (finish[i] == false){
			flg = true;
			break;
		}
	}
	return flg;
}
void Check_Request(int *request, int **need, int *work, int *available, int **allocation, bool *finish, int n, int m){
	bool flg = false;
	string s = "", str = "";
	int **allocate, **needs;
	allocate = new int*[n];
	needs = new int*[n];
	for (int i = 0; i < n; i++){
		allocate[i] = new int[m];
		needs[i] = new int[m];
	}
	
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			allocate[i][j] = allocation[i][j];
			needs[i][j] = need[i][j];
		}
	}
	if (compare(request, need[request[m]], m)){
		if (compare(request, available, m)){
			for (int i = 0; i < m; i++){
				work[i] -= request[i];
				allocate[request[m]][i] += request[i];
				needs[request[m]][i] -= request[i];
			}
			flg = Safe_State(s, finish, needs, allocate, work, n, m, request[m], "req");
			if (flg == false)
				str += ("Yes, Request can be granted with safe state " + s);
			else
				str += "No, Request can't be granted with safe state";
			cout << str << endl;
		}
		else
			cout << "No, Request can't be granted with safe state\n";
	}
	else
		cout << "ERROR, Process has exceeded its maximum claim\n";
}
void Print_Need_Matrix(bool *finish, int **need, int **max, int **allocation, int n, int m){
	cout << "Need Matrix: \n------------\n\t";
	for (int i = 0; i < m; i++)
		cout << "R" << i << "\t";
	cout << endl;
	for (int i = 0; i < n; i++){
		finish[i] = false;
		cout << "P" << i << "\t";
		for (int j = 0; j < m; j++){
			need[i][j] = max[i][j] - allocation[i][j];
			cout << need[i][j] << "\t";
		}
		cout << endl;
	}
}
void Read_Data(int **allocation, int **max, int *available, int *work, int n, int m){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++)
			cin >> allocation[i][j];
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++)
			cin >> max[i][j];
	}
	for (int i = 0; i < m; i++){
		cin >> available[i];
		work[i] = available[i];
	}
}
void Reset(string&str, string&s, bool*finish, int*available, int*work, int n, int m){
	str = "";
	s = "";
	for (int i = 0; i < n; i++)
		finish[i] = false;
	for (int i = 0; i < m; i++)
		work[i] = available[i];
}

int main(){
	// To read data from file
	ifstream in("InputFile.txt");
	cin.rdbuf(in.rdbuf());

	// Initialize the variables
	int n, m, **allocation, **max, **need, *available, *work, *request, num = 0, num_req = 0;
	bool *finish, flg;
	char chr, exit, k, req;
	string str = "", s = "";
	// n: No of Processes, m: no of Resources
	cin >> n >> m;

	// Making Needed Arrays
	available = new int[m];
	work = new int[m];
	request = new int[m+1];
	finish = new bool[n];
	allocation = new int*[n];
	max = new int*[n];
	need = new int*[n];
	for (int i = 0; i < n; i++){
		allocation[i] = new int[m];
		max[i] = new int[m];
		need[i] = new int[m];
	}
	
	// Enter data of arrays
	Read_Data(allocation, max, available, work, n, m);

	do {
		system("cls"); // clear all screen

		if (num > 0){
			cout << "To read new data, Enter 'y' & Enter 'n' to complete\n";
			k = getch();
			if (k == 'y' || k == 'Y'){
				cin >> n >> m;
				Read_Data(allocation, max, available, work, n, m);
			}
		}
		num++;

		cout << "To print Need MAtrix, Enter 'y' & Enter 'n' to complete\n";
		k = getch();
		// Calculate Need array
		if (k == 'y' || k == 'Y')
			Print_Need_Matrix(finish, need, max, allocation, n, m);
		
		do{
			cout << "Enter 'c' to check if the system in safe state & Enter 'r' to check if request can be granted?\n";
			chr = getch();
			if(chr == 'c' || chr == 'C'){
				// Check Safe State
				flg = Safe_State(s, finish, need, allocation, work, n, m, 0, "");
				if (flg == false)
					str += ("Yes, Safe state " + s);
				else
					str += "No, Unsafe state";
				cout << str << endl;
			}
			else if(chr == 'r' || chr == 'R'){
				// Check Request
				if (num_req == 0){
					cin >> request[m];
					for (int i = 0; i < m; i++)
						cin >> request[i];
				}
				else{
					cout << "To read new Request, Enter 'y' & Enter 'n' to take the old request\n";
					req = getch();
					if (req == 'y' || req == 'Y'){
						cin >> request[m];
						for (int i = 0; i < m; i++)
							cin >> request[i];
					}
				}
				num_req++;
				Check_Request(request, need, work, available, allocation, finish, n, m);
			}
			else
				cout << "Please Enter right character\n";
		} while(!(chr == 'c' || chr == 'C' || chr == 'r' || chr == 'R'));

		cout << "Enter 'q' to exit & Enter 'c' to complete\n";
		exit = getch();
		Reset(str, s, finish, available, work, n, m);
	} while(exit != 'q' && exit != 'Q');

	system("pause");
	return 0;
}