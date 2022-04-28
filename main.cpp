#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
//#include <Vector2.hpp>

using namespace std;

#define NRSOL 3
#define NODES 50

int a[NODES][NODES] = {0}, sol[NRSOL][NODES];

int x[NODES], n, k, nrsol=0, nrtari, coord[2][NODES];

void init()
{
    x[k]=0;
}

void citire(){
    ifstream f("iran.in");
    int i, j, x, y, nrvecini, tara, t;
    f >> nrtari;
    n = nrtari;

    for ( i = 0; i < nrtari ; i++){
        f >> tara;
        f >> x;
        f >> y;
        coord[0][i + 1] = x;
        coord[1][i + 1] = y;
       //coord[1][i + 1] = y + 5;
        f >> nrvecini;
        for (j = 0; j < nrvecini; j++){
            f >> t;
            a[i + 1][t + 1] = 1;
        }
    }

}
void afisare_matr(){
    int i,j;
    for(i = 1; i <= nrtari; i++){
         for(j = 1; j <= nrtari; j++){
            cout << a[i][j] << " ";
        }
    cout << endl;
    }

}

int succesor()
{
    if (x[k] < 4)
    {
        x[k]++;
        return 1;
    }
    else return 0;
}

int valid()
{
    for (int i = 1; i < k; i++)
        if (a[i][k] == 1)
            if (x[i] == x[k])
                return 0;
    return 1;
}


int solutie()
{
    return (k == n);
}

void tipar()
{
    int i;
    for (i = 1; i <= n; i++){
        cout << x[i] << " ";
        sol[nrsol][i] = x[i];
    }
    cout << endl;
    nrsol++;
}

void back()
{
    int as;
    k = 1;
    init();
    while (k > 0)
    {
        do
        {
            as = succesor();
        } while (as &&! valid());
        if (as)
            if (solutie() && nrsol < NRSOL)
                tipar();
            else
            {
                k++;
                init();
            }
        else k--;
    }
}

sf::Image image;
sf::Vector2u marime;

void galeata(int x, int y, const sf::Color& c)
{

    sf::Color vechi = image.getPixel(x, y);
    image.setPixel(x, y, c);
    if(x > 0 && image.getPixel(x - 1, y) == vechi)
        galeata(x - 1, y, c);

    if(y > 0 && image.getPixel(x, y - 1) == vechi)
        galeata(x, y - 1, c);

    if(x < marime.x - 1 && image.getPixel(x + 1, y) == vechi)
        galeata(x + 1, y, c);

    if(y < marime.y - 1 && image.getPixel(x, y + 1) == vechi)
        galeata(x, y + 1, c);
}

sf::Color culori[4];

void geneare_culori(){
    srand(time(NULL));
    for(int i = 1; i <= 4; i++){
        culori[i].r = rand() % 256;
        culori[i].g = rand() % 256;
        culori[i].b = rand() % 256;
        culori[i].a = 255;
    }
}

int main()
{
    citire();

    cout << "numarul de tari este: " << nrtari << endl;
    back();
    cout << endl;
    cout << nrsol << endl;

    afisare_matr();
    printf("asdasadssda\n");
    fflush(stdout);


    if (!(image.loadFromFile("iran1.png")))
            std::cout << "Cannot load image";   //Load Image

    marime = image.getSize();

    sf::RenderWindow window(sf::VideoMode(marime.x, marime.y), "party in iran!");

    geneare_culori();

    for (int i = 1; i <= nrtari; i++){
        galeata(coord[0][i], coord[1][i], culori[sol[rand() % 3][i]]);
    }
//
//    for (int i = 1; i <= nrtari; i++){
//        for (int ii = 1; ii <= 10; ii++){
//            for (int jj = 1; jj <= 20; jj++){
//                image.setPixel(coord[0][i] + ii, coord[1][i] + jj, sf::Color().Black);
//            }
//        }
//    }

    sf::Texture texture;
    texture.loadFromImage(image);  //Load Texture from image

    sf::Sprite sprite;
    sprite.setTexture(texture);


    window.draw(sprite);
    window.display();

    while (window.isOpen())
    {
            sf::Event event;

            while (window.pollEvent(event))
            {
                    if (event.type == sf::Event::Closed)
                            window.close();

                    }//Event handling done

            texture.loadFromImage(image);

            //window.clear();
            window.draw(sprite);
            window.display();
        }

    return 0;
}
