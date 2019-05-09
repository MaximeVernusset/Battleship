#include "console.h"


Console* Console::m_instance = NULL;

Console::Console()
{
    m_instance = NULL;
}

Console::~Console()
{

}

void Console::deleteInstance()
{
    delete m_instance;
    m_instance = NULL;
}

Console* Console::getInstance()
{
    if (!Console::m_instance)
    {
        m_instance = new Console();
    }

    return m_instance;
}

void Console::write(string s)
{
    cout << s;
}

void Console::writeLine(string s)
{
    cout << s << endl;
}

void Console::clear()
{
    system("cls");
}

void Console::effacerZoneActions()
{
    int ligne = POS_Y_ACTIONS+1;
    this->gotoLigCol(ligne, 0);
    this->setColor(GRIS_CLAIR, NOIR);
    for(unsigned short i=0; i<7; i++) //7 arbitraire : efface 2 lignes après le début du cin demandant les coordoonées
    {
        for(unsigned short j=0; j<LARGEUR_CONSOLE; j++)
            this->write(" ");
        this->writeLine("");
    }
}

void Console::gotoLigCol(int lig, int col)
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

int Console::getInputKey()
{
    return getch();
}

bool Console::isKeyboardPressed()
{
    return kbhit();
}


void Console::setColor(Color front, Color back)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,back*16+front);
}


void Console::wait(int t)
{
    Sleep(t);
}


void Console::resize(int lig, int col)
{
    stringstream stream;
    stream << "MODE CON LINES=" << lig << " COLS=" << col;
    system(stream.str().c_str());
}
