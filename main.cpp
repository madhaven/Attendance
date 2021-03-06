#include <JAYcreations.h>

///initial variables
ifstream filein;
ofstream fileout;
fstream file;
char subfilename[20], logfilename[20], timefilename[20], pasfilename[20];
int records, randomvariable;
class Paclass
{
public:
    char word[100];
    bool allowed;
    void refresh(){strcpy(word, "\0"); allowed=NULL;}
}password;
class SUBJECT//just to save what all subjects there are
{public:
    char name[30];
    int code;
}subclass;
class LOG//saves subject name of the subjects in that day
{public:
    TIMEstruct time;
    char subject[7][30];
    bool present[7];
}logclass;
class TIMETABLE//saves a default template of the subjects of a day, this is needed coz' there could be subject changes
{public:
    int subjectcode[7];
}timclass;
///functions
int daysinmonth(int month, int year)
{
    switch(month)
    {
    case 1:case 3:case 5:case 7:case 8:case 10:case 12:
        return 31;
    case 4:case 6:case 9:case 11:
        return 30;
    case 2:
        if (year%4==0)
            return 29;
        else return 28;
    }
}
bool authorized()
{
    char word[100];
    password.allowed=true;
    file.open(pasfilename, ios::binary|ios::in);
    file.read((char*)&password, sizeof(password));
    file.close();
    //cout<<"Password cracked\npassword : "<<password.word<<"\nallowed : "; if(password.allowed) cout<<"true"; else cout<<"false";
    if (password.allowed)
        return true;
    gotoxy(40-strlen("password : "), 15);
    cout<<"Password : ";
    getpass(word);
    system("cls");
    if (strcmp(word, password.word)==0)
        return true;
    return false;
}
void initialize(char* Y)//creates a file if it isn't actually existing
{
    fstream X;
    X.open(Y, ios::binary);
    X.close();
}
int rcount(char filename[20])
{
    fstream X;
    int thecount=0;
    if (strcmp(filename, subfilename)==0)
    {
        X.open(subfilename, ios::binary);
        X.close();
        X.open(subfilename, ios::binary|ios::in);
        while (X.read((char*)&subclass, sizeof(subclass)))
            thecount++;
        X.close();
    }
    else if (strcmp(filename, logfilename)==0)
    {
        X.open(logfilename, ios::binary);
        X.close();
        X.open(logfilename, ios::binary|ios::in);
        while(X.read((char*)&logclass, sizeof(logclass)))
            thecount++;
        X.close();
    }
    else if (strcmp(filename, timefilename)==0)
    {
        X.open(timefilename, ios::binary);
        X.close();
        X.open(timefilename, ios::binary|ios::in);
        while(X.read((char*)&timclass, sizeof(timclass)))
            thecount++;
        X.close();
    }
    else if (strcmp(filename, pasfilename)==0)
    {
        X.open(pasfilename, ios::binary);
        X.close();
        X.open(pasfilename, ios::binary|ios::in);
        while (X.read((char*)&password, sizeof(password)))
            thecount++;
        X.close();
    }
    return thecount;
}
int mainmenu()
{
    createbox(19, 7, 59, 21, true);
    int choice=0, options=6;
    gotoxy(38, 10); cout<<"MENU";
    gotoxy(40-strlen("Schedule")/2, 13); cout<<"Schedule";
    gotoxy(40-strlen("Course")/2, 14); cout<<"Course";
    gotoxy(40-strlen("Attendance")/2, 15); cout<<"Attendance";
    gotoxy(40-strlen("Password")/2, 16); cout<<"Password";
    gotoxy(40-strlen("Delete")/2, 17); cout<<"Delete";
    gotoxy(40-strlen("Exit")/2, 18); cout<<"Exit";

    while (1)
    {
        gotoxy(29, 13+choice); cout<<"|"; gotoxy(49, 13+choice); cout<<"|";
        gotoxy(79, 29);
        if (getch()=='\r')
            return choice+1;
        gotoxy(29, 13+choice); cout<<" "; gotoxy(49, 13+choice); cout<<" ";
        if (GetAsyncKeyState(VK_DOWN)&&(choice<options-1))
        {
            getch();
            choice++;
        }
        else if (GetAsyncKeyState(VK_UP)&&(choice>0))
        {
            getch();
            choice--;
        }
    }
}
void newsubject()
{
    int therecords=rcount(subfilename);
    createbox(19, 12, 59, 17, true);
    gotoxy(40-strlen("add a course")/2, 14);
    coutput("Add a Course", 0.5);
    retry:
    gotoxy(24, 15);
    gets(subclass.name);
    if (strlen(subclass.name)<1)
    {
        gotoxy(24, 15);
        cout<<"Too few letters";
        getch();
        gotoxy(24, 15);
        cout<<"               ";
        goto retry;
    }
    SUBJECT x;
    for (int i=0; i<therecords; i++)
    {
        filein.open(subfilename, ios::binary);
        filein.seekg(i*sizeof(x), ios::beg);
        filein.read((char*)&x, sizeof(x));
        filein.close();
        if (strcmpi(subclass.name, x.name)==0)
        {
            gotoxy(24, 15);
            cout<<"Existence Error";
            getch();
            gotoxy(24, 15);
            cout<<"               ";
            goto retry;
        }
    }
    subclass.code=0;
    bleh:
    subclass.code++;
    for (int i=0; i<therecords; i++)
    {
        filein.open(subfilename, ios::binary);
        filein.seekg(i*sizeof(x), ios::beg);
        filein.read((char*)&x, sizeof(x));
        filein.close();
        if (x.code==subclass.code)
            goto bleh;
    }
    file.open(subfilename, ios::binary|ios::app|ios::out);
    //file.seekp(therecords*sizeof(subclass), ios::beg);
    file.write((char*)&subclass, sizeof(subclass));
    file.close();
    system("cls");
}
void addtimetable()
{
    int maxlength=2;
    char day[5][10]={"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    int ycord=3;
    system("cls");
    gotoxy(40-strlen("create a timetable")/2, ycord); coutput("Create a Timetable", 0.5);
    for (int i=0; i<5; i++)//loop through each day
    {
        gotoxy(40-strlen(day[i])/2, ycord+2);
        coutput(day[i], 0.5);
        for (int j=0; j<7; j++)//loop through the hours
        {
            gotoxy(40-8, ycord+4+j); coutput("Hour "); cout<<j+1<<" : ";
            for (int k=-1; ;)//loop for selection of a subject
            {
                if (k==-1)
                {   gotoxy(41, ycord+4+j); cout<<"--";               }
                else
                {
                    filein.open(subfilename, ios::binary);
                    filein.seekg(k*sizeof(subclass), ios::beg);
                    filein.read((char*)&subclass, sizeof(subclass));
                    filein.close();
                    if (maxlength<strlen(subclass.name)) maxlength=strlen(subclass.name);
                    gotoxy(41, ycord+4+j); cout<<subclass.name;
                }
                if (getch()=='\r')
                {
                    if (k==-1)
                        timclass.subjectcode[j]=0;
                    else timclass.subjectcode[j]=subclass.code;
                    break;
                }
                if (k==-1)//to clear the last unselected subject from screen
                {   gotoxy(41, ycord+4+j); cout<<"  ";  }
                else
                {
                    filein.open(subfilename, ios::binary);
                    filein.seekg(k*sizeof(subclass), ios::beg);
                    filein.read((char*)&subclass, sizeof(subclass));
                    filein.close();
                    gotoxy(41, ycord+4+j); for (int pling=0; pling<strlen(subclass.name); pling++) cout<<' ';
                }
                if (GetAsyncKeyState(VK_DOWN)&&(k<rcount(subfilename)-1))
                {
                    getch();
                    k++;
                }
                else if (GetAsyncKeyState(VK_UP)&&(k>=0))
                {
                    getch();
                    k--;
                }
            }
        }//loop through the hours
        file.open(timefilename, ios::binary|ios::app|ios::out);
        file.write((char*)&timclass, sizeof(timclass));
        file.close();
        for (int j=0; j<7; j++)
        {   gotoxy(40-8, ycord+4+j); cout<<"         "; for (int pling=0; pling<maxlength; pling++) cout<<' '; }
        gotoxy(40-strlen(day[i])/2, ycord+2); for (int k=0;k<strlen(day[i]); k++) cout<<" ";
    }//loop through the days
    system("cls");
}
void newlog(int y=9, bool check=true)
{
    LOG theclass;
    if (!check)
    {
        settime(theclass.time);
        goto DOINIT;
    }
    TIMEstruct x;
    settime(x);
    if (rcount(logfilename)==0)
    {
        theclass.time.year=x.year; theclass.time.month=x.month; theclass.time.date=x.date; theclass.time.hour=x.hour; theclass.time.minute=x.minute; theclass.time.second=x.second;
        if (returnday(theclass.time)>6)
            return;
        if (theclass.time.hour<16)
            return;
        goto DOINIT;
    }
    filein.open(logfilename, ios::binary);
    filein.seekg((rcount(logfilename)-1)*sizeof(theclass), ios::beg);
    filein.read((char*)&theclass, sizeof(theclass));
    filein.close();
    if (x.year==theclass.time.year)
        if (x.month==theclass.time.month)
            if (x.date==theclass.time.date)
                return;

    nextday:
    if (theclass.time.date!=daysinmonth(theclass.time.month, theclass.time.year))
        theclass.time.date++;
    else if (theclass.time.month!=12)
    {
        theclass.time.month++;
        theclass.time.date=1;
    }
    else
    {
        theclass.time.date=1;
        theclass.time.month=1;
        theclass.time.year++;
    }
    if (returnday(theclass.time)>6)
        goto nextday;
    else if (theclass.time.year<x.year)
            goto DOINIT;
        else if (theclass.time.year==x.year)
                if (theclass.time.month<x.month)
                    goto DOINIT;
                else if (theclass.time.month==x.month)
                        if (theclass.time.date<x.date)
                            goto DOINIT;
                        else if ((theclass.time.date==x.date)&&(theclass.time.hour>=16))
                            goto DOINIT;
                    else return;
            else return;
    DOINIT:
    gotoxy(5 ,6+y);
    cout<<theclass.time.year;
    if (!check) for (int x=theclass.time.year; getch()!='\r'; ){if (GetAsyncKeyState(VK_UP)){getch();if (theclass.time.year<x)theclass.time.year++;cout<<"\b\b\b\b";}else if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.year>0)){getch();theclass.time.year--;cout<<"\b\b\b\b";}else continue;cout<<theclass.time.year;}
    cout<<" "<<setw(2)<<theclass.time.month;
    if (!check) while (getch()!='\r'){if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.month>1)){getch();cout<<"\b\b";theclass.time.month--;}else if (GetAsyncKeyState(VK_UP)&&(theclass.time.month<12)){getch();cout<<"\b\b";theclass.time.month++;}else continue;cout<<setw(2)<<theclass.time.month;}
    cout<<" "<<setw(2)<<theclass.time.date;
    if (!check) while (getch()!='\r'){if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.date>1)){getch();cout<<"\b\b";theclass.time.date--;}else if (GetAsyncKeyState(VK_UP)&&(theclass.time.date<daysinmonth(theclass.time.month, theclass.time.year))){getch();cout<<"\b\b";theclass.time.date++;}else continue;cout<<setw(2)<<theclass.time.date;}
    for (int i=0, j; i<7; i++)
    {
        gotoxy(33, 6+i);
        cout<<"HOUR "<<i+1<<" - ";
        filein.open(timefilename, ios::binary);
        filein.seekg((returnday(theclass.time)-1)*sizeof(timclass), ios::beg);
        filein.read((char*)&timclass, sizeof(timclass));
        filein.close();
        for (j=0; j<rcount(subfilename); j++)
        {
            filein.open(subfilename, ios::binary);
            filein.seekg(j*sizeof(subclass), ios::beg);
            filein.read((char*)&subclass, sizeof(subclass));
            filein.close();
            //gotoxy(0,0); cout<<"timclass.subjectcode : "<<timclass.subjectcode[i]<<" subclass.code : "<<subclass.code;getch();
            if (timclass.subjectcode[i]==subclass.code)
                {cout<<subclass.name;break;}
        }
        while (1)
        {
            gotoxy(42, 6+i);
            cout<<"                              ";
            gotoxy(42, 6+i);
            filein.open(subfilename, ios::binary);
            filein.seekg(j*sizeof(subclass), ios::beg);
            filein.read((char*)&subclass, sizeof(subclass));
            filein.close();
            cout<<subclass.name;
            if (getch()=='\r')
            {
                strcpy(theclass.subject[i], subclass.name);
                break;
            }
            else if (GetAsyncKeyState(VK_DOWN)&&(j<rcount(subfilename)-1))
            {
                getch();
                j++;
            }
            else if (GetAsyncKeyState(VK_UP)&&(j>0))
            {
                getch();
                j--;
            }
        }
        theclass.present[i]=true;
        while (1)
        {
            gotoxy(30, 6+i); cout<<"          ";
            gotoxy(30, 6+i);
            if (theclass.present[i]==true) cout<<"Present"; else cout<<"Absent";
            if (getch()=='\r')
                break;
            else if (GetAsyncKeyState(VK_UP)||(GetAsyncKeyState(VK_DOWN)))
            {
                getch();
                theclass.present[i]=!theclass.present[i];
            }
        }
    }
    file.open(logfilename, ios::binary|ios::app|ios::out);
    file.write((char*)&theclass, sizeof(theclass));
    file.close();
    gotoxy(15-6 ,5); cout<<"           ";
    gotoxy(15-7, 6); cout<<"          ";
    gotoxy(15-6, 7); cout<<"         ";
    for (int i=0; i<7; i++) {gotoxy(30, 5+i); cout<<"                                         ";}
    randomvariable++;
}
void hide(char filename[20])
{
    char command[100];
    for (int i=0; i<100; i++)
        command[i]=' ';
    strcpy(command, "attrib +h +S ");
    strcat(command, filename);
    system(command);
    return;
}
int main()
{
    int choice, weekday, cursor[2], counted, total;
    float attendance;
    char day[5][12]={"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    char word[100];
    LOG theclass;

    restart:
    strcpy(subfilename, "sJAYcreation.dat");initialize(subfilename);  hide(subfilename);
    strcpy(logfilename, "lJAYcreation.dat");initialize(logfilename);  hide(logfilename);
    strcpy(timefilename, "tJAYcreation.dat");initialize(timefilename);hide(timefilename);
    strcpy(pasfilename, "pJAYcreation.dat");initialize(pasfilename);  hide(pasfilename);
    signature();
    if (!authorized())
        return 0;
    system("mode 80, 30");
    while (rcount(subfilename)<5)
        newsubject();
    if (rcount(timefilename)<5)
        addtimetable();
    newlog();
    system("cls");
    mainmenu:
    choice=mainmenu();
    system("Cls");
    switch(choice)
    {
    case 1:///SCHEDULE
        weekday=cursor[1]=0;
        gotoxy(36, 3); coutput("SCHEDULE", 0.5);
        for (bool newscreen=true; ;)
        {
            if (newscreen)
            {
                newscreen=false;
                gotoxy(30, 8); for (int i=0; i<20; i++) cout<<' ';
                for (int i=0; i<7; i++)
                {gotoxy(40-7, 10+i); for (int j=0; j<40; j++) cout<<' ';}

                gotoxy(40-strlen(day[weekday])/2-2, 8);
                if (weekday!=0) cout<<"< "; else cout<<"  ";
                cout<<day[weekday];
                if (weekday<4) cout<<" >";
                filein.open(timefilename, ios::binary);
                filein.seekg(weekday*sizeof(timclass), ios::beg);
                filein.read((char*)&timclass, sizeof(timclass));
                filein.close();
                for (int i=0; i<7; i++)
                {
                    gotoxy(40-7, 10+i); cout<<"Hour "<<i+1<<" : ";
                    for (int j=0; j<rcount(subfilename); j++)
                    {
                        filein.open(subfilename, ios::binary);
                        filein.seekg(j*sizeof(subclass), ios::beg);
                        filein.read((char*)&subclass, sizeof(subclass));
                        filein.close();
                        if (subclass.code==timclass.subjectcode[i])
                        {cout<<subclass.name; break; }
                    }
                }
            }

            gotoxy(30, 10+cursor[1]); cout<<"|";
            gotoxy( 79, 29);
            if (getch()=='\r')
            {
                gotoxy(27, 10+cursor[1]); cout<<"EDIT";
                randomvariable=timclass.subjectcode[cursor[1]];
                for (int i=0; i<rcount(subfilename); i++)
                {
                    filein.open(subfilename, ios::binary);
                    filein.seekg(i*sizeof(subclass), ios::beg);
                    filein.read((char*)&subclass, sizeof(subclass));
                    filein.close();
                    if (subclass.code==randomvariable)
                    {   randomvariable=i; break;    }
                }
                while (1)
                {
                    gotoxy(0,0); cout<<"randomvariable : "<<randomvariable;
                    gotoxy(42, 10+cursor[1]); for (int i=0; i<30; i++) cout<<' ';
                    gotoxy(42, 10+cursor[1]);
                    if (randomvariable==-1)
                        cout<<"--";
                    else
                    {
                        filein.open(subfilename, ios::binary);
                        filein.seekg(randomvariable*sizeof(subclass), ios::beg);
                        filein.read((char*)&subclass, sizeof(subclass));
                        filein.close();
                        cout<<subclass.name;
                    }
                    if (getch()=='\r')
                        break;
                    else if (GetAsyncKeyState(VK_UP)&&(randomvariable>=0))
                    {
                        getch();
                        randomvariable--;
                    }
                    else if (GetAsyncKeyState(VK_DOWN)&&(randomvariable<rcount(subfilename)-1))
                    {
                        getch();
                        randomvariable++;
                    }
                }
                gotoxy(27, 10+cursor[1]); cout<<"   |";
                timclass.subjectcode[cursor[1]]=subclass.code;
                TIMETABLE x;
                for (int i=0; i<5; i++)
                {
                    filein.open(timefilename, ios::binary);
                    filein.seekg(i*sizeof(timclass), ios::beg);
                    filein.read((char*)&x, sizeof(x));
                    filein.close();
                    file.open("theyoyotemp.dat", ios::binary|ios::app|ios::out);
                    if (i!=weekday)
                        file.write((char*)&x, sizeof(x));
                    else file.write((char*)&timclass, sizeof(timclass));
                    file.close();
                }
                remove(timefilename);
                rename("theyoyotemp.dat", timefilename);
            }
            gotoxy(30, 10+cursor[1]); cout<<" ";

            if (GetAsyncKeyState(VK_DOWN)&&(cursor[1]<6))
            {
                getch();
                cursor[1]++;
            }
            else if (GetAsyncKeyState(VK_UP)&&(cursor[1]>0))
            {
                getch();
                cursor[1]--;
            }
            else if (GetAsyncKeyState(VK_RIGHT)&&(weekday<4))
            {
                getch();
                weekday++;
                newscreen=true;
            }
            else if (GetAsyncKeyState(VK_LEFT)&&(weekday>0))
            {
                getch();
                weekday--;
                newscreen=true;
            }
            else if (GetAsyncKeyState(VK_ESCAPE))
                goto mainmenu;
        }
    case 2:course:///COURSE
        while (rcount(subfilename)<5)
            newsubject();
        gotoxy(37, 3); cout<<"COURSE";
        records=rcount(subfilename);
        //cout<<records;getch();
        cursor[0]=0; cursor[1]=records-1;
        if (cursor[1]-cursor[0]>25) cursor[0]=cursor[1]-25;
        for (int i=cursor[0]; i<=cursor[1]; i++)
        {
            filein.open(subfilename, ios::binary);
            filein.seekg(i*sizeof(subclass), ios::beg);
            filein.read((char*)&subclass, sizeof(subclass));
            filein.close();
            gotoxy(40-strlen(subclass.name)/2, 6+i-cursor[0]);
            cout<<subclass.name;
        }
        while (1)
        {
            if (cursor[1]!=records)
            {
                gotoxy(14, 6+cursor[1]); cout<<"edit  |";
                gotoxy(60, 6+cursor[1]); cout<<"|  delete";
            }
            else
            {
                gotoxy(40, 6+records);
                cout<<"+";
            }
            gotoxy(79, 29);
            if ((getch()=='\r')&&(cursor[1]==records)&&(records<15))
            {   newsubject(); goto course;}
            if (cursor[1]!=records)
            {
                gotoxy(14, 6+cursor[1]); cout<<"       ";
                gotoxy(60, 6+cursor[1]); cout<<"         ";
            }
            else
            {
                gotoxy(40, 6+records);
                cout<<" ";
            }
            if (GetAsyncKeyState(VK_UP)&&(cursor[1]>0))
            {
                getch();
                cursor[1]--;
            }
            else if (GetAsyncKeyState(VK_DOWN)&&(cursor[1]<records))
            {
                getch();
                cursor[1]++;
            }
            else if (GetAsyncKeyState(VK_LEFT))
            {
                getch();
                SUBJECT x, y;
                char aword[30];
                filein.open(subfilename, ios::binary);
                filein.seekg(cursor[1]*sizeof(x), ios::beg);
                filein.read((char*)&x, sizeof(x));
                filein.close();
                createbox(19, 12, 59, 17, true);
                gotoxy(40-strlen("edit course")/2, 14);
                coutput("Edit Course", 0.5);
                retry:
                gotoxy(24, 15);
                strcpy(aword, x.name);
                autogets(x.name, x.name);
                if (strlen(x.name)<1)
                {
                    gotoxy(24, 15);
                    cout<<"Too few letters";
                    getch();
                    gotoxy(24, 15);
                    cout<<"             ";
                    goto retry;
                }
                for (int i=0; i<records; i++)
                {
                    filein.open(subfilename, ios::binary);
                    filein.seekg(i*sizeof(y), ios::beg);
                    filein.read((char*)&y, sizeof(y));
                    filein.close();
                    if (strcmpi(x.name, y.name)==0)
                    {
                        gotoxy(24, 15);
                        cout<<"Existence Error";
                        getch();
                        gotoxy(24, 15);
                        cout<<"               ";
                        goto retry;
                    }
                }
                for (int i=0; i<rcount(logfilename); i++)
                {
                    filein.open(logfilename, ios::binary);
                    filein.seekg(i*sizeof(logclass), ios::beg);
                    filein.read((char*)&logclass, sizeof(logclass));
                    filein.close();
                    for (int j=0; j<7; j++)
                    {
                        if (strcmpi(logclass.subject[j], aword)==0)
                            strcpy(logclass.subject[j], x.name);
                    }
                    file.open("thetempfile.dat", ios::binary|ios::app|ios::out);
                    file.write((char*)&logclass, sizeof(logclass));
                    file.close();
                }
                remove(logfilename);
                rename("thetempfile.dat", logfilename);
                for (int i=0; i<records; i++)
                {
                    filein.open(subfilename, ios::binary);
                    filein.seekg(i*sizeof(y), ios::beg);
                    filein.read((char*)&y, sizeof(y));
                    filein.close();
                    file.open("thetempfile.dat", ios::binary|ios::app|ios::out);
                    if (x.code==y.code)
                        file.write((char*)&x, sizeof(x));
                    else file.write((char*)&y, sizeof(y));
                    file.close();
                }
                remove(subfilename);
                rename("thetempfile.dat", subfilename);
                system("cls");
                goto course;
            }
            else if (GetAsyncKeyState(VK_RIGHT))
            {
                getch();
                createbox(19, 12, 59, 16, true);
                gotoxy(40-strlen("confirm deletion")/2, 14);
                coutput("Confirm Deletion");
                if (getch()=='\r')
                if (authorized())
                {
                    SUBJECT x; int bleh;
                    filein.open(subfilename, ios::binary);
                    filein.seekg(cursor[1]*sizeof(x), ios::beg);
                    filein.read((char*)&x, sizeof(x));
                    filein.close();
                    bleh=x.code;
                    for (int i=0; i<records; i++)
                    {
                        filein.open(subfilename, ios::binary);
                        filein.seekg(i*sizeof(x), ios::beg);
                        filein.read((char*)&x, sizeof(x));
                        filein.close();
                        if (bleh!=x.code)
                        {
                            file.open("thexyztemp.dat", ios::binary|ios::app|ios::out);
                            file.write((char*)&x, sizeof(x));
                            file.close();
                        }
                    }
                    remove(subfilename);
                    rename("thexyztemp.dat", subfilename);

                    for (int i=0; i<5; i++)
                    {
                        filein.open(timefilename, ios::binary);
                        filein.seekg(i*sizeof(timclass), ios::beg);
                        filein.read((char*)&timclass, sizeof(timclass));
                        filein.close();
                        for (int j=0; j<7; j++)
                        {
                            if (timclass.subjectcode[j]==bleh)
                                timclass.subjectcode[j]=0;
                        }
                        file.open("theblehblehtemfile.dat", ios::binary|ios::app|ios::out);
                        file.write((char*)&timclass, sizeof(timclass));
                        file.close();
                    }
                    remove(timefilename);
                    rename("theblehblehtemfile.dat", timefilename);
                    system("cls");
                    goto course;
                }
                goto course;
            }
            else if (GetAsyncKeyState(VK_ESCAPE))
                goto mainmenu;
        }
        getch();
        break;
    case 3:///ATTENDANCE
        gotoxy(36, 5); cout<<"SEARCH";
        gotoxy(37, 6); cout<<"LOGS";
        randomvariable=0;

        ///attendance preview
        for (int i=0; i<rcount(subfilename); i++)
        {
            counted=total=0;
            filein.open(subfilename, ios::binary);
            filein.seekg(i*sizeof(subclass), ios::beg);
            filein.read((char*)&subclass, sizeof(subclass));
            filein.close();
            for (int j=0; j<rcount(logfilename); j++)
            {
                filein.open(logfilename, ios::binary);
                filein.seekg(j*sizeof(logclass), ios::beg);
                filein.read((char*)&logclass, sizeof(logclass));
                filein.close();
                for (int k=0; k<7; k++)
                {
                    if (strcmp(logclass.subject[k], subclass.name)==0)
                    {
                        total++;
                        if (logclass.present[k]==true)
                            counted++;
                    }
                }
            }
            gotoxy(38-strlen(subclass.name), 10+i);
            cout<<subclass.name<<" : ";
            //gotoxy(0, 0); cout<<"counted : "<<counted<<" total : "<<total;getch();
            if (total>=1) attendance=(100*((float)counted/total)); else attendance=0;
            cout<<setw(3)<<(int)attendance;
            if ((int)attendance<attendance)
                cout<<"."<<(int)((attendance-(int)attendance)*100);
            else cout<<".00";
            gotoxy(47, 10+i); cout<<"%";
        }

        while (1)
        {
            gotoxy(32, 5+randomvariable); cout<<"|";
            gotoxy(45, 5+randomvariable); cout<<"|";
            gotoxy(79, 29);
            if (getch()=='\r')
                break;
            gotoxy(32, 5+randomvariable); cout<<" ";
            gotoxy(45, 5+randomvariable); cout<<" ";
            if (GetAsyncKeyState(VK_ESCAPE))
                goto mainmenu;
            else if (GetAsyncKeyState(VK_UP)&&(randomvariable==1))
            {
                getch();
                randomvariable=0;
            }
            else if (GetAsyncKeyState(VK_DOWN)&&(randomvariable==0))
            {
                getch();
                randomvariable=1;
            }
        }
        system("Cls");
        if (randomvariable==0)
        {
            settime(theclass.time);
            gotoxy(40-8, 15);
            cout<<"SEARCH : ";
            gotoxy(41, 15);
            cout<<theclass.time.year;
            for (int x=theclass.time.year; getch()!='\r'; )
            {
                if (GetAsyncKeyState(VK_UP))
                {
                    getch();
                    if (theclass.time.year<x)
                        theclass.time.year++;
                    cout<<"\b\b\b\b";
                }
                else if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.year>0))
                {
                    getch();
                    theclass.time.year--;
                    cout<<"\b\b\b\b";
                }
                else continue;
                cout<<theclass.time.year;
            }
            cout<<" "<<setw(2)<<theclass.time.month;
            while (getch()!='\r')
            {
                if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.month>1))
                {
                    getch();
                    cout<<"\b\b";
                    theclass.time.month--;
                }
                else if (GetAsyncKeyState(VK_UP)&&(theclass.time.month<12))
                {
                    getch();
                    cout<<"\b\b";
                    theclass.time.month++;
                }
                else continue;
                cout<<setw(2)<<theclass.time.month;
            }
            cout<<" "<<setw(2)<<theclass.time.date;
            while (getch()!='\r')
            {
                if (GetAsyncKeyState(VK_DOWN)&&(theclass.time.date>1))
                {
                    getch();
                    cout<<"\b\b";
                    theclass.time.date--;
                }
                else if (GetAsyncKeyState(VK_UP)&&(theclass.time.date<daysinmonth(theclass.time.month, theclass.time.year)))
                {
                    getch();
                    cout<<"\b\b";
                    theclass.time.date++;
                }
                else continue;
                cout<<setw(2)<<theclass.time.date;
            }
            for (int i=0; i<rcount(logfilename); i++)
            {
                filein.open(logfilename, ios::binary);
                filein.seekg(i*sizeof(logclass), ios::beg);
                filein.read((char*)&logclass, sizeof(logclass));
                filein.close();
                if (logclass.time.year==theclass.time.year)
                    if (logclass.time.month==theclass.time.month)
                        if (logclass.time.date==theclass.time.date)
                        {
                            randomvariable=rcount(logfilename);
                            cursor[1]=i;
                            if (cursor[1]>20) cursor[0]=cursor[1]-19; else cursor[0]=0;
                            system("cls");
                            goto pling;
                        }
            }
            gotoxy(40-strlen("log not found")/2, 16);
            coutput("Log not found", 2); _sleep(1000);
        }
        logs:
        gotoxy(37, 2);
        coutput("LOGS");
        randomvariable=rcount(logfilename);
        if (randomvariable==0)cursor[1]=0; else cursor[1]=randomvariable-1;
        if (cursor[1]>20) cursor[0]=cursor[1]-19; else cursor[0]=0;
        pling:
        for (bool newscreen=true; ;)
        {
        //gotoxy(0, 0); cout<<"randomvariable="<<randomvariable<<" cursor[0]="<<cursor[0]<<" cursor[1]="<<cursor[1];
            if (newscreen)
            {
                newscreen=false;
                for (int i=cursor[0]; i<=cursor[0]+25; i++)
                {   gotoxy(5, 4+i-cursor[0]); cout<<"                          ";  };
                for (int i=cursor[0]; ((i<=cursor[0]+20)&&(i<randomvariable)); i++)
                {
                    filein.open(logfilename, ios::binary);
                    filein.seekg(i*sizeof(logclass), ios::beg);
                    filein.read((char*)&logclass, sizeof(logclass));
                    filein.close();
                    gotoxy(5, 6+i-cursor[0]);
                    cout<<logclass.time.year<<" "<<setw(2)<<logclass.time.month<<" "<<setw(2)<<logclass.time.date;
                }
            }

            if (cursor[1]<randomvariable)
            {
                filein.open(logfilename, ios::binary);
                filein.seekg(cursor[1]*sizeof(logclass), ios::beg);
                filein.read((char*)&logclass, sizeof(logclass));
                filein.close();
                for (int i=0; i<7; i++)
                {
                    gotoxy(30, 6+i);
                    if (logclass.present[i])
                        cout<<"Present";
                    else cout<<"Absent";
                    gotoxy(40, 6+i);
                    cout<<"- "<<logclass.subject[i];
                }
            }
            if (cursor[1]==randomvariable)
                {gotoxy(10, 6+cursor[1]-cursor[0]); cout<<"+";}
            else {gotoxy(3, 6+cursor[1]-cursor[0]); cout<<"|";}
            gotoxy(79, 29);
            if ((getch()=='\r')&&(cursor[1]==randomvariable)) goto jump;
            for (int i=0; i<7; i++)
            {
                gotoxy(30, 6+i);
                if (logclass.present[i])
                    cout<<"       ";
                else cout<<"      ";
                gotoxy(40, 6+i);
                cout<<"- "; for (int x=0; x<strlen(logclass.subject[i]); x++) cout<<" ";
            }
            if (cursor[1]==randomvariable)
            {   gotoxy(10, 6+cursor[1]-cursor[0]); cout<<" ";}
            else gotoxy(3, 6+cursor[1]-cursor[0]); cout<<" ";

            if (GetAsyncKeyState(VK_UP)&&(cursor[1]>0))
            {
                getch();
                cursor[1]--;
                if (cursor[0]>cursor[1])
                {   cursor[0]--;newscreen=true; }
            }
            else if (GetAsyncKeyState(VK_DOWN)&&(cursor[1]<randomvariable))
            {
                getch();
                cursor[1]++;
                if (cursor[0]+20<cursor[1])
                {   cursor[0]++; newscreen=true;    }
            }
            else if (GetAsyncKeyState(VK_ESCAPE))
                goto mainmenu;
            continue;
            jump:
                gotoxy(10, 6+cursor[1]-cursor[0]); cout<<" ";
                newlog(cursor[1]-cursor[0], false);
                newscreen=true;
                continue;
            return 0;
        }
    case 4:///PASSWORD
        if (!authorized())
        {
            system("cls");
            gotoxy(40-strlen("unauthorized access")/2, 17);
            _sleep(300);
            cout<<("UNAUTHORIZED ACCESS");
            _sleep(800);
            gotoxy(40-strlen("terminating ...")/2, 18);
            coutput("TERMINATING ...", 3);
            return 0;
        }
        repass:
        gotoxy(40-strlen("new password : "), 13);
        cout<<"New Password : ";
        getpass(password.word);
        gotoxy(40-strlen("confirm password : "), 14);
        cout<<"Confirm Password : ";
        getpass(word);
        if (strcmp(word, password.word)!=0)
        {   gotoxy(40-strlen("mismatch")/2, 20); cout<<"MISMATCH"; _sleep(400); system("cls"); goto repass;      }
        if (strlen(word)<1)
        {   gotoxy(40-strlen("passwrod removed")/2, 20); cout<<"Password Removed"; getch(); password.allowed=true; }
        else
        {   gotoxy(40-strlen("password changed")/2, 20); cout<<"Password Changed"; getch(); password.allowed=false;}
        file.open(pasfilename, ios::out);
        file.write((char*)&password, sizeof(password));
        file.close();
        goto mainmenu;
    case 5:///DELETE
        system("cls");
        gotoxy(40-strlen("are you sure you want to erase all existing data ?")/2, 15);coutput("Are you sure you want to erase all existing data ?", 3);
        gotoxy(38, 16); cout<<": ";
        gets(word);
        if (no(word))
            goto mainmenu;
        _sleep(200);
        gotoxy(40-strlen("reset protocol initiated")/2, 11); coutput("Reset Protocol Initiated", 1);
        _sleep(200);
        remove(subfilename);
        gotoxy(0, 0); coutput("FILE 1 removed\n", 0.5); _sleep(50);
        remove(logfilename);
        coutput("FILE 2 removed\n", 0.5); _sleep(50);
        remove(timefilename);
        coutput("FILE 3 removed\n", 0.5); _sleep(50);
        remove(pasfilename);
        coutput("FILE 4 removed\n", 0.5); _sleep(50);
        gotoxy(40-strlen("RESTARTING PROGRAM")/2, 17);
        coutput("RESTARTING PROGRAM", 3);
        _sleep(500);
        system("cls");
        _sleep(500);
        goto restart;
    case 6:///EXIT
        system("cls");
        gotoxy(39, 14);
        coutput("BYE", 7);
        _sleep(700);
        return 0;
    }
    coutput("PLING", 3); _sleep(300); cout<<"\b\b\b\b\b"; coutput("     ", 3);
}
