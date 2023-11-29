#include "TXLib.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <stdio.h>

int knop = 0;
int t = 0;
using namespace std;

struct picture
{
    int x;
    int y;
    int w_scr;
    int h_scr;
    string adress;
    HDC pic;
    int w;
    int h;
    bool visible;
    string category;

    void draw()
    {
       if(visible) Win32::TransparentBlt(txDC(), x, y, w_scr, h_scr, pic, 0, 0, w, h, TX_WHITE);
    }

    bool click()
    {
        return ( txMouseButtons() == 1 &&
         txMouseX() > x && txMouseX() < x + w_scr &&
         txMouseY() > y && txMouseY() < y + h_scr);
    }


};

struct Button
{
    int x;
    const char* name;
    string category;
    int t;

    void draw()
    {

  /*  bool contact()
    {
        return ( txMouseButtons() == 1 && txMouseX() > x && txMouseX() < x + 138 && txMouseY() > 14 && txMouseY() < 40);
    }                                                                                                           */
    if(txMouseButtons() == 1 && txMouseX() > x && txMouseX() < x + 138 && txMouseY() > 14 && txMouseY() < 40 or t == knop)
    {
        txSetColor (TX_GRAY, 2);
        txSetFillColor (TX_GRAY);
        Win32::RoundRect(txDC(), x + 3, 17, x + 136, 43, 20, 20);
        txSetColor (TX_BLACK, 2);
        if(knop == t) txSetFillColor(RGB ( 0, 200, 10)); else txSetFillColor (TX_WHITE);
        Win32::RoundRect(txDC(), x + 2, 16, x + 135, 42, 20, 20);
        txSetColor(TX_BLACK);
        txSelectFont("Times New Roman", 25);
        txDrawText(x + 2, 16, x + 135, 42, name);

    }
    else
    {
        txSetColor (TX_GRAY, 2);
        txSetFillColor (TX_GRAY);
        Win32::RoundRect(txDC(), x + 3, 17, x + 136, 43, 20, 20);
        txSetColor (TX_BLACK, 2);
        txSetFillColor (TX_WHITE);
        Win32::RoundRect(txDC(), x, 14, x + 133, 40, 20, 20);
        txSetColor(TX_BLACK);
        txSelectFont("Times New Roman", 25);
        txDrawText(x, 14, x + 133, 40, name);
    }
}

    bool click()
    {
        return ( txMouseButtons() == 1 && txMouseX() > x && txMouseX() < x + 138 && txMouseY() > 14 && txMouseY() < 40);
    }

};


void Shablon()
{
    txSetColor(TX_BLACK);
    txSetFillColor(TX_WHITE);
    txRectangle(900, 3, 1247, 647);
    Win32::RoundRect(txDC(), 902, 5, 1244, 31, 10, 10);
    txSetFillColor(TX_GRAY);
    txRectangle(903, 33, 1242, 642);
    txSetColor(TX_BLACK);
    txSetFillColor(TX_WHITE);
    txRectangle(50, 80, 850, 610);


}

int getw(string adress)
{
    char header[54];
    ifstream bmp;

    bmp.open(adress, ios::in | ios::binary);
    bmp.read(header, 54);

    int w = *(int *)&header[18];

    return w;
}

int geth(string adress)
{
    char header[54];

    ifstream bmp;

    bmp.open(adress, ios::in | ios::binary);
    bmp.read(header, 54);

    int h = *(int *)&header[22];

    return h;
}

int ReadFolders(string adressDir, picture menu_pic[], int count_pic)
{
    DIR *dir;
    struct dirent *ent;
    int X = 940;
    int Y = 50;
    if ((dir = opendir (adressDir.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if((string)ent->d_name != "." && (string)ent->d_name != "..")
            {
                if(Y>=500)
                    menu_pic[count_pic].y = Y;
                    menu_pic[count_pic].x = X;
                }
                else
                {
                    X +=100;
                    Y = 50;
                    menu_pic[count_pic].y = Y;
                    menu_pic[count_pic].x = X;
                }
                menu_pic[count_pic].adress = adressDir + (string)ent->d_name;
                count_pic++;
                Y+= 150;
            }
      closedir (dir);
      }


    return count_pic;
}


int main()
{
    txTextCursor (false);
    txDisableAutoPause();

    int count_btn = 3;
    int count_pic = 0;
    int quest = -1;
    bool mousefree = false;

    int ncentr_pic = 0;

    txCreateWindow (1250, 650);

    Button btn[count_btn];
    btn[0] = {10, "��� ������", "T-Shirt", 1};
    btn[1] = {158, "�������", "drinks", 2};
    btn[2] = {158 + 148, "�������� �����", "head_wear", 3};

    picture menu_pic[50];

    count_pic = ReadFolders("photo/T-Shirt/", menu_pic, count_pic);
    count_pic = ReadFolders("photo/drinks/", menu_pic, count_pic);
    count_pic = ReadFolders("photo/head_wear/", menu_pic, count_pic);

    for(int i=0; i<count_pic; i++)
    {
        menu_pic[i].pic = txLoadImage(menu_pic[i].adress.c_str());

        menu_pic[i].w = getw(menu_pic[i].adress);
        menu_pic[i].h = geth(menu_pic[i].adress);
        menu_pic[i].visible = false;


        string str = menu_pic[i].adress;
        int pos1 = str.find("/");
        int pos2 = str.find("/", pos1+1);
        menu_pic[i].category = str.substr(pos1 + 1, pos2 - (pos1+1));

        if(menu_pic[i].category == "head_wear" || menu_pic[i].category == "drinks")
        {
             menu_pic[i].w_scr = menu_pic[i].w / 3;
             menu_pic[i].h_scr = menu_pic[i].h / 3;
        }
        if(menu_pic[i].category == "T-Shirt")
        {
             menu_pic[i].w_scr = menu_pic[i].w / 9;
             menu_pic[i].h_scr = menu_pic[i].h / 9;
        }
    }




    picture centr_pic[100];

    while(!txGetAsyncKeyState(VK_ESCAPE))
    {
        txSetColor (TX_YELLOW);
        txSetFillColor (TX_YELLOW);
        txClear();

        txBegin();

        Shablon();


        for(int i=0; i<count_btn; i++)
        {
            btn[i].draw();
        }

        for(int i=0; i<count_pic; i++)
        {
            menu_pic[i].draw();
        }
        for(int i=0; i<ncentr_pic; i++)
        {
            centr_pic[i].draw();
        }
        for(int ib=0;ib<count_btn; ib++)
        {
            if(btn[ib].click())
            {
                for(int ip=0; ip <count_pic; ip++)
                {
                    menu_pic[ip].visible = false;
                    if(menu_pic[ip].category == btn[ib].category)
                    {
                        menu_pic[ip].visible = true;
                    }
                }
            }
        }
        for(int i=0;i<count_pic; i++)
        {
            if(menu_pic[i].click() && menu_pic[i].visible)
            {
                while(txMouseButtons() == 1) txSleep(10);
                centr_pic[ncentr_pic] = {250, 130,menu_pic[i].w - menu_pic[i].w * 0.2,menu_pic[i].h - menu_pic[i].h * 0.2, menu_pic[i].adress, menu_pic[i].pic, menu_pic[i].w,menu_pic[i].h, true,"��� ������"};
                ncentr_pic++;
            }
        }

        for(int npic=0;npic<ncentr_pic; npic++)
        {
            if(centr_pic[npic].click() && centr_pic[npic].visible)
            {
                quest = npic;
                mousefree = false;
            }
        }

        if(quest >= 0 && txMouseButtons() == 1 && !mousefree)
        {
            centr_pic[quest].x = txMouseX() - centr_pic[quest].w_scr/2;
            centr_pic[quest].y = txMouseY() - centr_pic[quest].h_scr/2;;

        }
        if(quest >= 0 && !txMouseButtons() == 1 && !mousefree)
        {
            mousefree = true;
        }
        if(quest >=0)
        {
            if(txGetAsyncKeyState(VK_UP))
            {
                centr_pic[quest].y -=7;
            }
            if(txGetAsyncKeyState(VK_DOWN))
            {
                centr_pic[quest].y +=7;
            }
            if(txGetAsyncKeyState(VK_RIGHT))
            {
                centr_pic[quest].x +=7;
            }
            if(txGetAsyncKeyState(VK_LEFT))
            {
                centr_pic[quest].x -=7;
            }

            if(txGetAsyncKeyState(VK_ADD) || txGetAsyncKeyState(VK_OEM_PLUS))
            {
                centr_pic[quest].h_scr = centr_pic[quest].h_scr*1.02;
                centr_pic[quest].w_scr = centr_pic[quest].w_scr*1.02;
            }
            if(txGetAsyncKeyState(VK_SUBTRACT) || txGetAsyncKeyState(VK_OEM_MINUS))
            {
                centr_pic[quest].h_scr = centr_pic[quest].h_scr*0.98;
                centr_pic[quest].w_scr = centr_pic[quest].w_scr*0.98;
            }

        }
        if(quest >=0 && txGetAsyncKeyState(VK_DELETE))
        {
            centr_pic[quest] = centr_pic[ncentr_pic - 1];
            ncentr_pic--;
            quest = -1;
        }
        txEnd();

        txSleep(50);
    }
  /* for(int i = 0;i<count_pic;i++)
   {
        txDeleteDC();
   }*/

    txTextCursor (false);
    return 0;
}

