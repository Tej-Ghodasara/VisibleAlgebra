#include<iostream>
#include<vector>
#include<cmath>
#include<iomanip>
using namespace std;

constexpr float EPSILON = 1e-6; //tolerance for floating point comparisons

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

            //creating the leading ones
            int last_leading1 = -1;
            for(int i = 0; i < min(m, n); i++)
            {
                int flag = 0; //flag to check for a leading one appearance
                for(int k = last_leading1 + 1; k < n; k++)
                {
                    //break if leading one already created
                    if(flag == 1) break;

                    //checking elements to create a leading one and making all other column entries zero
                    if(abs(arr[i][k]) > EPSILON)
                    {
                        flag = 1;
                        last_leading1 = k;
                        row_op3(arr, rhs, i, n, (1 / arr[i][k]));
                        for(int j = 0; j < m; j++)
                        {
                            if(j != i)
                            {
                                row_op2(arr, rhs, j, i, n, -arr[j][k]);
                            }
                        }
                        continue;
                    }

                    //if element is zero then swap with another row where it is non-zero
                    //if no non-zero element in column there then continue loop to next column
                    else
                    {
                        int leading1_found = 0; //leading1_found to check for a candidate in another row for a leading one
                        for(int j = (i + 1); j < m; j++)
                        {
                            if(abs(arr[j][k]) > EPSILON)
                            {
                                row_op1(arr, rhs, i, j, n);
                                leading1_found = 1;
                                break;
                            }
                        }
                        if(leading1_found == 1)
                        {
                            flag = 1;
                            last_leading1 = k;
                            row_op3(arr, rhs, i, n, (1 / arr[i][k]));
                            for(int j = 0; j < m; j++)
                            {
                                if(j != i)
                                {
                                    row_op2(arr, rhs, j, i, n, -arr[j][k]);
                                }
                            }
                        }
                    }
                }
            }
            
            //checking if there is a zero row stuck in between and pull it down
            int zeroflag = 0; //flag to check if a row in the middle is zero
            for(int i = 0; i < m; i++)
            {
                zeroflag = zero_check(arr, i, n);
                if(zeroflag == 0)
                {
                    int rowswap = 0; //flag to keep track of a candidate row for swapping with zero row
                    for(int j = m - 1; j > i; j--)
                    {
                        rowswap = zero_check(arr, j , n);
                        if(rowswap == 1)
                        {
                            row_op1(arr, rhs, i, j, n);
                            break;
                        }
                    }
                }
            }

            //output in matrix form with precision upto three decimal places
            for(int i = 0; i < m; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    if(abs(arr[i][j]) > EPSILON) cout << round(arr[i][j] * 1000.0f) / 1000.0f << " ";
                    else cout << "0 ";
                }
                cout << "| " << round(rhs[i] * 1000.0f) / 1000.0f << "\n";
            }
        }
    }
    return 0;
}