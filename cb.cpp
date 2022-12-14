// Jorb Dehmel, 2022, jdehmel@outlook.com

#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <string>
using namespace std;

////////////////////////////

#define DIR "~/.clipboard"
#define FILE "/data.txt"

////////////////////////////

bool cmpCharArrs(const char *a, const char b[])
{
    int i;
    for (i = 0; a[i] != '\0' && b[i] != '\0'; i++)
    {
        if (a[i] != b[i])
            return false;
    }
    return a[i] == b[i];
}

void safeSystem(char *what)
{
    if (system(what) != 0)
        throw runtime_error(what);
    return;
}

////////////////////////////

int main(const int argc, const char *argv[])
{
    try
    {

        if (argc < 2)
        {
            cout << "\033[1;31m"
                 << "Error: please enter a command. (see help for all commands)"
                 << "\033[0m\n";
            return 1;
        }

        ///////////////////////////

        if (system("mkdir -p " DIR) != 0)
        {
            cout << "\033[1;31m"
                 << "Error: could not make ~/.clipboard directory."
                 << "\033[0m\n";
            return 2;
        }
        else if (system("touch " DIR FILE) != 0)
        {
            cout << "\033[1;31m"
                 << "Error: could not make ~/.clipboard/data.txt file."
                 << "\033[0m\n";
            return 3;
        }

        ///////////////////////////

        if (cmpCharArrs(argv[1], "copy"))
        {
            if (argc < 3)
            {
                cout << "\033[1;31m"
                     << "Error:  requires filepath."
                     << "\033[0m\n";
                return 4;
            }
            else if (argc > 3)
            {
                cout << "\033[1;33m"
                     << "Note: Unused argument(s) following filepath."
                     << "\033[0m\n";
            }

            //////////////////

            safeSystem("mv " DIR FILE " ./temp.txt");

            ofstream fout("./temp.txt", ios::out);
            if (!fout.is_open())
            {
                cout << "\033[1;31m"
                     << "Error:  could not open clipboard files."
                     << "\033[0m\n";
                return 5;
            }

            safeSystem("pwd >tempcwd.txt");
            string cwd;
            ifstream fin("tempcwd.txt", ios::in);
            assert(fin.is_open());
            fin >> cwd;
            fin.close();
            safeSystem("rm tempcwd.txt");

            fout << "cp -r " << cwd << "/" << argv[2] << " .";
            fout.close();

            safeSystem("mv ./temp.txt " DIR FILE);

            //////////////////

            cout << "\033[1;32m"
                 << "Copied!"
                 << "\033[0m\n";
        }

        else if (cmpCharArrs(argv[1], "cut"))
        {
            if (argc < 3)
            {
                cout << "\033[1;31m"
                     << "Error:  requires filepath."
                     << "\033[0m\n";
                return 4;
            }
            else if (argc > 3)
            {
                cout << "\033[1;33m"
                     << "Note: Unused argument(s) following filepath."
                     << "\033[0m\n";
            }

            //////////////////

            safeSystem("mv " DIR FILE " ./temp.txt");

            ofstream fout("./temp.txt", ios::out);
            if (!fout.is_open())
            {
                cout << "\033[1;31m"
                     << "Error:  could not open clipboard files."
                     << "\033[0m\n";
                return 5;
            }

            safeSystem("pwd >tempcwd.txt");
            string cwd;
            ifstream fin("tempcwd.txt", ios::in);
            assert(fin.is_open());
            fin >> cwd;
            fin.close();
            safeSystem("rm tempcwd.txt");

            fout << "mv " << cwd << "/" << argv[2] << " .";
            fout.close();

            safeSystem("mv ./temp.txt " DIR FILE);

            //////////////////

            cout << "\033[1;32m"
                 << "Cut!"
                 << "\033[0m\n";
        }

        else if (cmpCharArrs(argv[1], "paste"))
        {
            if (argc > 2)
            {
                cout << "\033[1;33m"
                     << "Note: Unused argument(s)."
                     << "\033[0m\n";
            }

            //////////////////

            safeSystem("mv " DIR FILE " ./temp.txt");

            ifstream fin("./temp.txt", ios::in);
            if (!fin.is_open())
            {
                cout << "\033[1;31m"
                     << "Error:  could not open clipboard files."
                     << "\033[0m\n";
                return 5;
            }

            string command;
            getline(fin, command);
            fin.close();

            safeSystem("mv ./temp.txt " DIR FILE);

            if (system(command.c_str()) != 0)
            {
                cout << "\033[1;31m"
                     << "Error:  could not paste."
                     << "\033[0m\n";
                return 6;
            }

            //////////////////

            cout << "\033[1;32m"
                 << "Pasted!"
                 << "\033[0m\n";
        }

        else if (cmpCharArrs(argv[1], "help"))
        {
            cout << "\033[1;33m"
                 << "Command | Function\n"
                 << "-------------------\n"
                 << "  copy  |   Puts filepath on clipboard\n"
                 << "  cut   |   Copies, and deletes upon paste\n"
                 << "  paste |   Pastes whatever was in the clipboard to the cwd\n"
                 << "  help  |   Displays this page\n\n"
                 << "(Jorb Dehmel, 2022, jdehmel@outlook.com)\n"
                 << "\033[0m\n";
        }

        ///////////////////////////

        else
        {
            cout << "\033[1;31m"
                 << "Invalid command '" << argv[1] << "'. (see help for all commands)"
                 << "\033[0m\n";
            return 7;
        }
    }
    catch (const runtime_error &e)
    {
        cout << "\033[1;31m"
             << e.what()
             << "\033[0m\n";
        return 8;
    }
    catch (...)
    {
        cout << "\033[1;31m"
             << "An unknown fatal error occured."
             << "\033[0m\n";
        return 9;
    }

    return 0;
}

////////////////////////////
