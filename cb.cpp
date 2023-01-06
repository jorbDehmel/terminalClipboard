// Jorb Dehmel, 2022, jdehmel@outlook.com

#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <string>

#include "tags.hpp"

using namespace std;

////////////////////////////

#define DIR "~/.clipboard/"
#define CURRENT_CLIPBOARD_FILE ".currentCB.txt"

////////////////////////////

void safeSystem(string what)
{
    if (system(what.c_str()) != 0)
        throw runtime_error(what);
    return;
}

void safeSystemBasic(char *what)
{
    if (system(what) != 0)
        throw runtime_error(what);
    return;
}

int system(string what)
{
    return system(what.c_str());
}

////////////////////////////

int main(const int argc, const char *argv[])
{
    try
    {
        string formattedArgs[argc];
        for (int i = 0; i < argc; i++)
        {
            string current = argv[i];
            if (current.find(' ') != string::npos)
                formattedArgs[i] = '"' + current + '"';
            else
                formattedArgs[i] = current;
        }

        if (system("mkdir -p " DIR) != 0)
        {
            cout << tags::red_bold
                 << "Error: could not make " << DIR << " directory.\n"
                 << tags::reset;
            return 2;
        }

        string file = "";

        // Load current clipboard
        safeSystem(string("touch ") + DIR + CURRENT_CLIPBOARD_FILE);
        safeSystem(string("cp ") + DIR + CURRENT_CLIPBOARD_FILE + " ./temp.txt");

        ifstream currentCB("temp.txt", ios::in);

        if (!currentCB.is_open())
        {
            cout << "Could not find " << DIR CURRENT_CLIPBOARD_FILE << ", creating...\n";

            system("echo clipboard >" DIR CURRENT_CLIPBOARD_FILE);
            file = "clipboard";
        }
        else
        {
            getline(currentCB, file);
        }
        currentCB.close();

        system("rm -rf temp.txt");

        if (file == "")
            file = "clipboard";

        if (argc < 2)
        {
            cout << tags::red_bold
                 << "Error: please enter a command. (see help for all commands)\n"
                 << tags::reset;
            return 1;
        }

        ///////////////////////////

        if (system(string("touch ") + DIR + file) != 0)
        {
            cout << tags::red_bold
                 << "Error: could not make " << DIR << file << " file.\n"
                 << tags::reset;
            return 3;
        }

        ///////////////////////////

        if (formattedArgs[1] == "copy")
        {
            if (argc < 3)
            {
                cout << tags::red_bold
                     << "Error:  requires filepath.\n"
                     << tags::reset;
                return 4;
            }
            else if (argc > 3)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s) following filepath.\n"
                     << tags::reset;
            }

            //////////////////

            safeSystem(string("mv ") + DIR + file + " ./temp.txt");

            ofstream fout("./temp.txt", ios::out);
            if (!fout.is_open())
            {
                cout << tags::red_bold
                     << "Error:  could not open clipboard files.\n"
                     << tags::reset;
                return 5;
            }

            safeSystem("pwd >tempcwd.txt");
            string cwd;
            ifstream fin("tempcwd.txt", ios::in);
            assert(fin.is_open());
            fin >> cwd;
            fin.close();
            safeSystem("rm tempcwd.txt");

            fout << "cp -r " << cwd << "/" << formattedArgs[2] << " .";
            fout.close();

            safeSystem(string("mv ./temp.txt ") + DIR + file);

            //////////////////

            cout << tags::green_bold
                 << "Copied!\n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "cut")
        {
            if (argc < 3)
            {
                cout << tags::red_bold
                     << "Error:  requires filepath.\n"
                     << tags::reset;
                return 4;
            }
            else if (argc > 3)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s) following filepath.\n"
                     << tags::reset;
            }

            //////////////////

            safeSystem(string("mv ") + DIR + file + " ./temp.txt");

            ofstream fout("./temp.txt", ios::out);
            if (!fout.is_open())
            {
                cout << tags::red_bold
                     << "Error:  could not open clipboard files.\n"
                     << tags::reset;
                return 5;
            }

            safeSystem("pwd >tempcwd.txt");
            string cwd;
            ifstream fin("tempcwd.txt", ios::in);
            assert(fin.is_open());
            fin >> cwd;
            fin.close();
            safeSystem("rm tempcwd.txt");

            fout << "mv " << cwd << "/" << formattedArgs[2] << " .";
            fout.close();

            safeSystem(string("mv ./temp.txt ") + DIR + file);

            //////////////////

            cout << tags::green_bold
                 << "Cut!\n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "paste")
        {
            if (argc > 2)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s).\n"
                     << tags::reset;
            }

            //////////////////

            cout << "Pasting...";

            safeSystem(string("mv ") + DIR + file + " ./temp.txt");

            ifstream fin("./temp.txt", ios::in);
            if (!fin.is_open())
            {
                cout << tags::red_bold
                     << "Error:  could not open clipboard files.\n"
                     << tags::reset;
                return 5;
            }

            string command;
            getline(fin, command);
            fin.close();

            safeSystem(string("mv ./temp.txt ") + DIR + file);

            if (system(command.c_str()) != 0)
            {
                cout << "\r"
                     << tags::red_bold
                     << "Error:  could not paste.\n"
                     << tags::reset;
                return 6;
            }

            //////////////////

            cout << "\r"
                 << tags::green_bold
                 << "Pasted!   \n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "help")
        {
            cout << tags::yellow_bold
                 << "Command  |   Function\n"
                 << "---------------------\n"
                 << "  copy   |   Puts filepath on clipboard\n"
                 << "  cut    |   Copies, and deletes upon paste\n"
                 << "  paste  |   Pastes whatever was in the clipboard to the cwd\n"
                 << "  help   |   Displays this page\n"
                 << "  switch |   Switches clipboards\n"
                 << "  list   |   Lists all clipboards\n"
                 << "  clear  |   Clears a clipboard\n"
                 << "  remove |   Removes a clipboard\n"

                 << "\n(Jorb Dehmel, 2022, jdehmel@outlook.com)\n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "switch")
        {
            if (argc < 3)
            {
                cout << tags::red_bold
                     << "Error:  Requires destination clipboard.\n"
                     << tags::reset;
                return 10;
            }
            else if (argc > 3)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s).\n"
                     << tags::reset;
            }

            safeSystem(string("echo ") + formattedArgs[2] + " >" + DIR + CURRENT_CLIPBOARD_FILE);

            cout << tags::green_bold
                 << "Changed current clipboard to \"" << formattedArgs[2] << "\"\n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "list")
        {
            if (argc > 2)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s).\n"
                     << tags::reset;
            }

            cout << tags::green_bold
                 << "Current clipboard: " << tags::reset << "\""
                 << file << "\""
                 << tags::green_bold
                 << "\nOthers: "
                 << tags::reset << flush;

            system("ls " DIR);
        }

        else if (formattedArgs[1] == "clear")
        {
            if (argc < 3)
            {
                cout << tags::red_bold
                     << "Error:  Requires clipboard to clear.\n"
                     << tags::reset;
                return 10;
            }
            else if (argc > 3)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s).\n"
                     << tags::reset;
            }

            safeSystem(string("echo >") + DIR + file);

            cout << tags::green_bold
                 << "Emptied clipboard \"" << formattedArgs[2] << "\"\n"
                 << tags::reset;
        }

        else if (formattedArgs[1] == "remove")
        {
            if (argc < 3)
            {
                cout << tags::red_bold
                     << "Error:  Requires clipboard to remove.\n"
                     << tags::reset;
                return 10;
            }
            else if (argc > 3)
            {
                cout << tags::yellow_bold
                     << "Note: Unused argument(s).\n"
                     << tags::reset;
            }

            safeSystem(string("rm -rf ") + DIR + formattedArgs[2]);

            cout << tags::green_bold
                 << "Removed clipboard \"" << formattedArgs[2] << "\"\n"
                 << tags::reset;
        }

        ///////////////////////////

        else
        {
            cout << tags::red_bold
                 << "Invalid command '" << formattedArgs[1] << "'. (see help for all commands)\n"
                 << tags::reset;
            return 7;
        }
    }
    catch (const runtime_error &e)
    {
        cout << tags::red_bold
             << e.what()
             << tags::reset;
        return 8;
    }
    catch (...)
    {
        cout << tags::red_bold
             << "An unknown fatal error occured.\n"
             << tags::reset;
        return 9;
    }

    return 0;
}

////////////////////////////
