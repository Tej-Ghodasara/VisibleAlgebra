#include<bits/stdc++.h>
using namespace std;

//swaps rows x and y
void row_op1(vector<vector<float>> &arr, vector<float> &rhs, int x, int y, int n)
{
    float temp;
    for(int i = 0; i < n; i++)
    {
        temp = arr[x][i];
        arr[x][i] = arr[y][i];
        arr[y][i] = temp;
    }
    temp = rhs[x];
    rhs[x] = rhs[y];
    rhs[y] = temp;
    return;
}

//row x becomes row x + k times row y
void row_op2(vector<vector<float>> &arr, vector<float> &rhs, int x, int y, int n, float k)
{
    for(int i = 0; i < n; i++) arr[x][i] += k * arr[y][i];
    rhs[x] += k * rhs[y];
    return;
}

//row x becomes k times itself
void row_op3(vector<vector<float>> &arr, vector<float> &rhs, int x, int n, float k)
{
    for(int i = 0; i < n; i++) arr[x][i] *= k;
    rhs[x] *= k;
    return;
}

//returns 0 if row x is zero and 1 otherwise
int zero_check(vector<vector<float>> &arr, int x, int n)
{
    int flag = 0;
    for(int i = 0; i < n; i++)
    {
        if(arr[x][i] != 0)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main()
{
    while(1)
    {
        int m, n;
        cout << "Enter positive integers m and n for size of coefficient matrix\n";
        cout << "Enter 0 0 if you want to terminate the program\n";
        cin >> m >> n;
        if(m == 0 && n == 0) break;
        else if(m < 0 || n < 0) cout << "Enter valid input\n";
        else
        {
            //input in matrix form
            vector<vector<float>> arr(m, vector<float>(n));
            vector<float> rhs(m);
            cout << "Enter the coefficient matrix\n";
            for(int i = 0; i < m; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    cin >> arr[i][j];
                }
            }
            cout << "Enter the RHS of equations\n";
            for(int i = 0; i < m; i++)
            {
                cin >> rhs[i];
            }

            //transforming into RREF form
            for(int i = 0; i < min(m, n); i++)
            {
                //checking diagonal elements to create a leading 1
                if(abs(arr[i][i]) > 1e-6)
                {
                    row_op3(arr, rhs, i, n, (1 / arr[i][i]));
                    for(int j = 0; j < m; j++)
                    {
                        if(j != i)
                        {
                            row_op2(arr, rhs, j, i, n, -arr[j][i]);
                        }
                    }
                }

                //if diagonal element is 0 then swap with another row where it is non-zero
                else
                {
                    int flagz = 0;
                    for(int j = (i + 1); j < m; j++)
                    {
                        if(abs(arr[j][i]) > 1e-6)
                        {
                            row_op1(arr, rhs, i, j, n);
                            flagz = 1;
                            break;
                        }
                    }
                    if(flagz == 1)
                    {
                        row_op3(arr, rhs, i, n, (1 / arr[i][i]));
                        for(int j = 0; j < m; j++)
                        {
                            if(j != i)
                            {
                                row_op2(arr, rhs, j, i, n, -arr[j][i]);
                            }
                        }
                    }
                }
            }
            
            //checking if there is a zero row stuck in between and pull it down
            int zeroflag1 = 0;
            for(int i = 0; i < m; i++)
            {
                zeroflag1 = zero_check(arr, i, n);
                if(zeroflag1 == 0)
                {
                    int zeroflag2 = 0;
                    for(int j = m - 1; j > i; j--)
                    {
                        zeroflag2 = zero_check(arr, j , n);
                        if(zeroflag2 == 1)
                        {
                            row_op1(arr, rhs, i, j, n);
                            break;
                        }
                    }
                }
            }

            //output in matrix form with precision upto 3 decimal places
            for(int i = 0; i < m; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    if(abs(arr[i][j]) > 1e-6) cout << floor(arr[i][j] * 1000) / 1000 << " ";
                    else cout << "0 ";
                }
                cout << "| " << rhs[i] << "\n";
            }
        }
    }
    return 0;
}