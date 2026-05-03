#include<iostream>
#include<vector>
#include<cmath>
#include<iomanip>
#include<utility>
using namespace std;

constexpr double EPSILON = 1e-12; //tolerance for floating point comparisons

//prints system with precision upto three decimal places
void print_sys(vector<vector<double>> &arr, vector<double> &rhs)
{
    int m = arr.size();
    int n = arr[0].size();
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            double val = arr[i][j];
            if(abs(val) < EPSILON)  val = 0;
            cout << left << setw(10) << round(val * 1000.0f) / 1000.0f;
        }
        if(abs(rhs[i]) > EPSILON) cout << "| " << round(rhs[i] * 1000.0f) / 1000.0f << "\n";
        else cout << "| 0\n";
    }
    cout << "\n\n";
    return;
}

//prints matrix with precision upto three decimal places
void print_mat(vector<vector<double>> &arr)
{
    cout << "same operation for inverse : \n\n";
    int m = arr.size();
    int n = arr[0].size();
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            double val = arr[i][j];
            if(abs(val) < EPSILON)  val = 0;
            cout << left << setw(10) << round(val * 1000.0f) / 1000.0f;
        }
        cout << "\n";
    }
    cout << "\n\n";
    return;
}

//swaps rows x and y
void row_op1(int &steps, string &soln, vector<vector<double>> &inv, vector<vector<double>> &arr, vector<double> &rhs, int x, int y, int n, char show_steps)
{
    swap(arr[x], arr[y]);
    swap(rhs[x], rhs[y]);
    if(show_steps == 'y')
    {
        cout << "STEP " << steps << " : Swap rows " << x + 1 << " and " << y + 1 << "\n\n";
        print_sys(arr, rhs);
    }

    if(soln == "inverse")
    {
        swap(inv[x], inv[y]);
        if(show_steps == 'y') print_mat(inv);
    }

    steps++;
    return;
}

//row x becomes row x + k times row y
void row_op2(int &steps, string &soln, vector<vector<double>> &inv, vector<vector<double>> &arr, vector<double> &rhs, int x, int y, int n, double k, char show_steps)
{
    for(int i = 0; i < n; i++)
    {
        arr[x][i] += k * arr[y][i];
    }
    rhs[x] += k * rhs[y];
    if(show_steps == 'y')
    {
        cout << "STEP " << steps << " : Turn row " << x + 1 << " into row " << x + 1 << " + " << k << " times row " << y + 1 << "\n\n";
        print_sys(arr, rhs);
    }

    if(soln == "inverse")
    {
        for(int i = 0; i < n; i++)
        {
            inv[x][i] += k * inv[y][i];
        }
        if(show_steps == 'y') print_mat(inv);
    }

    steps++;
    return;
}

//row x becomes k times itself
void row_op3(int &steps, string &soln, vector<vector<double>> &inv, vector<vector<double>> &arr, vector<double> &rhs, int x, int n, double k, char show_steps)
{
    for(auto &elem : arr[x])
    {
        elem *= k;
    }
    rhs[x] *= k;
    if(show_steps == 'y')
    {
        cout << "STEP " << steps << " : Multiply row " << x + 1 << " by " << k << "\n\n";
        print_sys(arr, rhs);
    }

    if(soln == "inverse")
    {
        for(auto &elem : inv[x])
        {
            elem *= k;
        }
        if(show_steps == 'y') print_mat(inv);
    }

    steps++;
    return;
}

//returns 0 if row x is zero and 1 otherwise
int zero_check(vector<vector<double>> &arr, int x, int n)
{
    for(const auto &elem : arr[x])
    {
        if(abs(elem) > EPSILON) return 1;
    }
    return 0;
}

int main()
{
    while(1)
    {
        cout << "RREF, inverse or linear equation solution? Enter r for RREF, i for inverse and s otherwise\n";
        string soln;
        cin >> soln;
        if(soln == "r") soln = "rref";
        else if(soln == "i") soln = "inverse";
        else soln = "solution";
        int m, n;
        cout << "Enter positive integers m and n for size of coefficient matrix.\n";
        cout << "Enter 0 0 if you want to terminate the program.\n";
        cin >> m >> n;
        if(m == 0 && n == 0) break;
        if(m <= 0 || n <= 0) cout << "Enter valid input\n";
        else if(soln == "inverse" && m != n) cout << "Enter dimensions for a square matrix\n";
        else
        {
            char show_steps = 'n';
            cout << "Enter the character 'y' if you wish to see the steps for\n";
            cout << "RREF transformations. Enter 'n' otherwise.\n";
            cin >> show_steps;
            if(show_steps != 'y') show_steps = 'n';

            //input in matrix form
            vector<vector<double>> arr(m, vector<double>(n));
            vector<double> rhs(m);
            int steps = 1;
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
            cout << "\nSOLUTION :\n\n";

            vector<vector<double>> inv;
            if(soln == "inverse")
            {
                for(int i = 0; i < m; i++)
                {
                    inv.push_back(vector<double>(n, 0));
                }
                for(int i = 0; i < m; i++)
                {
                    inv[i][i] = 1;
                }
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
                        if(abs(arr[i][k] - 1.0f) > EPSILON) row_op3(steps, soln, inv, arr, rhs, i, n, (1 / arr[i][k]), show_steps);
                        for(int j = 0; j < m; j++)
                        {
                            if(j != i && abs(arr[j][k]) > EPSILON)
                            {
                                row_op2(steps, soln, inv, arr, rhs, j, i, n, -arr[j][k], show_steps);
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
                                row_op1(steps, soln, inv, arr, rhs, i, j, n, show_steps);
                                leading1_found = 1;
                                break;
                            }
                        }
                        if(leading1_found == 1)
                        {
                            flag = 1;
                            last_leading1 = k;
                            row_op3(steps, soln, inv, arr, rhs, i, n, (1 / arr[i][k]), show_steps);
                            for(int j = 0; j < m; j++)
                            {
                                if(j != i)
                                {
                                    row_op2(steps, soln, inv, arr, rhs, j, i, n, -arr[j][k], show_steps);
                                }
                            }
                        }
                    }
                }

                //if no leading 1, row is a zero row so stop printing inverse
                if(flag == 0 && soln == "inverse")
                {
                    cout << "matrix is not invertible, discontinuing operations for inverse\n\n";
                    soln = "rref";
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
                            row_op1(steps, soln, inv, arr, rhs, i, j, n, show_steps);
                            break;
                        }
                    }
                }
            }

            cout << "final augmented RREF matrix :\n\n";
            print_sys(arr, rhs);
            if(soln == "inverse")
            {
                cout << "final inverse matrix :\n\n";
                print_mat(inv);
            }

            if(soln == "solution")
            {
                int solutionflag = 0, zero_row_num = 0;
                for(int i = m - 1; i >= 0; i--)
                {
                    if(zero_check(arr, i, n) == 0 && rhs[i] != 0)
                    {
                        cout << "No solution exists\n\n";
                        solutionflag = 1;
                        break;
                    }
                    else if(zero_check(arr, i, n) == 0)
                    {
                        solutionflag = 2;
                        zero_row_num++;
                    }
                }
                if(solutionflag == 2) cout << "infinite solutions exist with the constraints :\n\n";
                else if(solutionflag == 0) cout << "unique solution exists with the constraints :\n\n";
                if(solutionflag != 1)
                {
                    for(int i = 0; i < m - zero_row_num; i++)
                    {
                        int j;
                        for(j = 0; j < n; j++)
                        {
                            if(abs(arr[i][j] - 1.0f) < EPSILON) break;
                        }
                        cout << "x" << j + 1 << " = " << round(rhs[i] * 1000.0f) / 1000.0f;
                        for(int k = j + 1; k < n; k++)
                        {
                            if(arr[i][k] > EPSILON) cout << " - " << round(arr[i][k] * 1000.0f) / 1000.0f  << "(x" << k + 1 << ")";
                            if(arr[i][k] < -EPSILON) cout << " + " << round(-arr[i][k] * 1000.0f) / 1000.0f  << "(x" << k + 1 << ")";
                        }
                        cout << "\n\n";
                    }
                }
            }
        }
    }
    return 0;
}