/*
 * chars sourced from:
 * https://patorjk.com/software/taag/#p=display&h=0&v=0&f=Big&t=%0A
 */

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <codecvt>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

#define CLS       "\x1B[2J"
#define HOME      "\x1B[H"
#define HIDE_CURS "\x1B[?25h"
#define SHOW_CURS "\x1B[?25l"

std::pair< int, int > get_winsize();
char                  getch(bool &);

std::vector< std::string >                   get_letter(std::string const);
std::map< char, std::vector< std::string > > get_letters();

void print_str(std::string const, unsigned short const);
void print_ascii_util(std::string const,
                      std::map< char, std::vector< std::string > > const);
void print_ascii(std::string const,
                 std::map< char, std::vector< std::string > > const,
                 unsigned short const);

int
main(void)
{
    std::pair< int, int >                        winsize       = get_winsize();
    int                                          og_cols       = winsize.first;
    std::map< char, std::vector< std::string > > ascii_letters = get_letters();
    bool                                         loop          = true;
    std::string                                  base_str;

    std::printf(HIDE_CURS);
    std::printf(CLS);
    std::printf(HOME);
    fflush(stdout);

    while ( loop )
    {
        char ch = getch(loop);

        if ( ch == 127 && base_str.size() > 0 )
        {
            base_str.pop_back();
        }
        else if ( ch != 127 )
        {
            base_str.push_back(ch);
        }

        std::printf(CLS);
        std::printf(HOME);
        fflush(stdout);

        print_ascii(base_str, ascii_letters, winsize.first);
        std::cout << "\r";

        winsize = get_winsize();

        if ( og_cols != winsize.first )
        {
            std::printf(CLS);
            std::printf(HOME);
            fflush(stdout);

            og_cols = winsize.first;
        }
    }

    return 0;
}

std::pair< int, int >
get_winsize()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    return std::make_pair(w.ws_col, w.ws_row);
}

char
getch(bool &loop)
{
    system("stty raw");
    char input = std::getchar();

    if ( input == 3 )
    {
        loop = false;
    }

    system("stty cooked");

    return input;
}

void
print_str(std::string const str, unsigned short const width)
{
    int length_left = str.size();
    int cursor      = 0;

    std::printf(HOME);
    fflush(stdout);

    do
    {
        std::cout << str.substr(cursor, width) << std::endl;

        cursor      += width - 1;
        length_left -= width - 1;
    } while ( length_left > 0 );
}

std::vector< std::string >
get_letter(std::string const path)
{
    std::ifstream              fin;
    std::vector< std::string > data;
    std::string                line;

    fin.open(path);

    while ( getline(fin, line) )
    {
        data.push_back(line);
    }

    fin.close();

    return data;
}

std::map< char, std::vector< std::string > >
get_letters()
{
    std::map< char, std::vector< std::string > > ascii_letters;

    for ( char ch = 'a'; ch <= 'z'; ch++ )
    {
        std::string path = "chars/";
        path.push_back(ch);
        path.append(".txt");
        ascii_letters[ch] = get_letter(path);
    }

    for ( char ch = '0'; ch <= '9'; ch++ )
    {
        std::string path = "chars/";
        path.push_back(ch);
        path.append(".txt");
        ascii_letters[ch] = get_letter(path);
    }

    ascii_letters['&'] = get_letter("chars/ampersand.txt");
    ascii_letters['<'] = get_letter("chars/angleleft.txt");
    ascii_letters['>'] = get_letter("chars/angleright.txt");
    ascii_letters['@'] = get_letter("chars/at.txt");
    ascii_letters['['] = get_letter("chars/bracketleft.txt");
    ascii_letters[']'] = get_letter("chars/bracketright.txt");
    ascii_letters['^'] = get_letter("chars/caret.txt");
    ascii_letters[':'] = get_letter("chars/colon.txt");
    ascii_letters[','] = get_letter("chars/comma.txt");
    ascii_letters['$'] = get_letter("chars/dollar.txt");
    ascii_letters['!'] = get_letter("chars/exclamation.txt");
    ascii_letters['#'] = get_letter("chars/hash.txt");
    ascii_letters['-'] = get_letter("chars/minus.txt");
    ascii_letters['('] = get_letter("chars/parenleft.txt");
    ascii_letters[')'] = get_letter("chars/parenright.txt");
    ascii_letters['%'] = get_letter("chars/percent.txt");
    ascii_letters['.'] = get_letter("chars/period.txt");
    ascii_letters['?'] = get_letter("chars/question.txt");
    ascii_letters[';'] = get_letter("chars/semicolon.txt");
    ascii_letters['/'] = get_letter("chars/slash.txt");
    ascii_letters[' '] = get_letter("chars/space.txt");
    ascii_letters['*'] = get_letter("chars/star.txt");
    ascii_letters['_'] = get_letter("chars/underscore.txt");

    return ascii_letters;
}

void
print_ascii_util(
    std::string const                                  str,
    std::map< char, std::vector< std::string > > const ascii_letters)
{
    for ( int i = 0; i < 6; i++ )
    {
        for ( char ch : str )
        {
            std::cout << ascii_letters.at(ch).at(i);
        }

        std::cout << std::endl;
    }
}

void
print_ascii(std::string const                                  str,
            std::map< char, std::vector< std::string > > const ascii_letters,
            unsigned short const                               cols)
{
    int                        ascii_length = 0;
    std::vector< std::string > lines;
    std::string                current_line;
    std::string                tmp;

    for ( char ch : str )
    {
        if ( ch == 13 )
        {
            lines.push_back(current_line);
            current_line.clear();

            ascii_length = 0;
        }
        else
        {

            current_line.push_back(ch);

            ascii_length += (ascii_letters.at(ch).at(0).length()) / 2;

            if ( ascii_length > cols )
            {
                current_line.pop_back();
                lines.push_back(current_line);
                current_line.clear();
                current_line.push_back(ch);

                ascii_length = (ascii_letters.at(ch).at(0).length()) / 2;
            }
        }
    }

    if ( current_line.length() > 0 )
    {
        lines.push_back(current_line);
    }

    for ( std::string line : lines )
    {
        print_ascii_util(line, ascii_letters);
    }
}
