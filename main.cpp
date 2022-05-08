#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <string.h>


using namespace std;

#define NRSOL 4
#define NODES 50
int a[NODES][NODES] = {0}, sol[NRSOL][NODES];
int x[NODES], n, k, nrsol=0, nrtari, coord[2][NODES];

char nume_imagini[3][20] =
{
    {"tehran1.png"},
    {"usa1.png"},
    {"iran1.png"}
};

char nume_date[3][20] =
{
    {"tehran.in"},
    {"usa.in"},
    {"iran.in"}
};

int nr_harta;

void init()
{
    x[k]=0;
}

void citire(){
    ifstream f(nume_date[nr_harta]);
    int i, j, x, y, nrvecini, tara, t;
    f >> nrtari;
    n = nrtari;

    for ( i = 0; i < nrtari ; i++){
        f >> tara;
        f >> x;
        f >> y;
        coord[0][i + 1] = x;
        coord[1][i + 1] = y;
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
    int pc = x[1]; // culoarea primei tari
    if(sol[pc - 1][1] != 0){
        return;
    }
    for (i = 1; i <= n; i++){
        cout << x[i] << " ";
        sol[pc - 1][i] = x[i];
    }
    cout << " !" << endl;
    nrsol++;
}

void print_sol_partiala()
{
  int i;
  for(i = 1; i <= n; i++){
    if(i <=k)
      cout << x[i] << " ";
    else
      cout << "*" << " ";
  }
  cout << endl; 
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
        } while (as && !valid());
	//print_sol_partiala(); 
        if (as)
            if (solutie() && nrsol < NRSOL) {
                tipar();
                k = 1; //sare direct
            } else {
                k++;
                init();
            }
        else k--;
    }
}

sf::Image image;

sf::Vector2u marime;


void galeatat(int x, int y, const sf::Color& c)
{

    sf::Color vechi = image.getPixel(x, y);
    image.setPixel(x, y, c);

    if(x > 0 && image.getPixel(x - 1, y) == vechi)
      galeatat(x - 1, y, c);

    if(y > 0 && image.getPixel(x, y - 1) == vechi)
        galeatat(x, y - 1, c);

    if(x < marime.x - 1 && image.getPixel(x + 1, y) == vechi)
        galeatat(x + 1, y, c);

    if(y < marime.y - 1 && image.getPixel(x, y + 1) == vechi)
        galeatat(x, y + 1, c);
}



void galeata(int x, int y, const sf::Color& c, const sf::Color& bk)
{

    sf::Color vechi = image.getPixel(x, y);
    if(c == vechi ||c == bk)
      return;
    
    image.setPixel(x, y, c);

    if(x > 0 && image.getPixel(x - 1, y) == vechi)
      galeata(x - 1, y, c, bk);

    if(y > 0 && image.getPixel(x, y - 1) == vechi)
      galeata(x, y - 1, c, bk);

    if(x < marime.x - 1 && image.getPixel(x + 1, y) == vechi)
      galeata(x + 1, y, c, bk);

    if(y < marime.y - 1 && image.getPixel(x, y + 1) == vechi)
      galeata(x, y + 1, c, bk);
}



sf::Color culori[5], bkgnd = {255, 255, 255, 255};

void geneare_culori(){
  culori[0].r = 0; 
  culori[0].g = 0; 
  culori[0].b = 0; 
  culori[0].a = 255;

    for(int i = 1; i <= 4; i++){
      culori[i].r = 100* (i%2); // rand() % 256;
      culori[i].g = 100* (i%3); //rand() % 256;
      culori[i].b = 100* (i%4); //rand() % 256;
      culori[i].a = 255;
    }
}

int main()
{
    srand(time(NULL));

    cout << "Alegeti o harta din urmatoarele: " << endl;
    cout << "[1] Tehran\n" << "[2] USA\n" << "[3] Iran\n";

    cin >> nr_harta;
    if(nr_harta > 3 || nr_harta < 1){
        cout << "\nAvem doar trei harti la dispozitie, asadar te rog alege din acelea !! \n";
        return 0;
    }
    nr_harta--;

    citire();

    cout << "numarul de tari: " << nrtari << endl;

    int sol_aleasa = rand() % NRSOL;
    cout <<"solutia aplicata: "<< sol_aleasa + 1 << endl;

    cout << "patru dintre solutiile generate sunt: \n";



//    cout << "\nmatricea de adiacenta: \n";
//    afisare_matr();

    if (!(image.loadFromFile(nume_imagini[nr_harta])))
            std::cout << "Cannot load image";   //Load Image

    marime = image.getSize();
    sf::RenderWindow window(sf::VideoMode(marime.x, marime.y), "Harta");
    bkgnd = image.getPixel(0, 0);
    printf("Bkgnd: %d %d %d %d\n", bkgnd.r, bkgnd.g,bkgnd.b,bkgnd.a);  
      
    geneare_culori();

    

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    window.draw(sprite);
    window.display();

    // L E G E N D A
    sf::Image limage;
    sf::Texture ltexture;
    sf::Sprite lsprite;
    sf::Vector2u lmarime;

    if (!(limage.loadFromFile("pergament.png")))
            std::cout << "Cannot load image";   //Load Image

    ltexture.loadFromImage(limage);
    lsprite.setTexture(ltexture);
    lmarime = limage.getSize();

    //    for (int i = 1; i <= nrtari; i++)
    //  galeata(coord[0][i], coord[1][i], culori[0], bkgnd);

    
    sf::RenderWindow winLegenda(sf::VideoMode(lmarime.x, lmarime.y), "Legenda");
    winLegenda.draw(lsprite);

    for(int r = 1; r <= 4; r++){
        sf::RectangleShape rectangle(sf::Vector2f(25, 25));
        rectangle.setPosition(25, r*(25+10));
        rectangle.setFillColor(culori[r]);
        winLegenda.draw(rectangle);

        sf::Text Text;
        sf::Font font;
        std::string stxt = std::to_string(r);
        Text.setString(stxt);

        if (!font.loadFromFile("Laksaman-Bold.ttf")){
            cout << "No font.\n";
            return 1;
        }
        Text.setFont(font);
        Text.setPosition(65, r*(22+10));
        Text.setCharacterSize(30);
        Text.setFillColor(sf::Color::Black);
        winLegenda.draw(Text);
        }

    winLegenda.display();

    // back();

    if(1){ 
      int as;
      k = 1;
      init();
      while (k > 0){
	do
	  {
	    as = succesor();
	  } while (as && !valid());


	
	for (int i = 1; i <= nrtari; i++){
	  if(i <= k){ 
	    galeata(coord[0][i], coord[1][i], culori[x[i]], bkgnd);
	  } else { 
	    galeata(coord[0][i], coord[1][i], culori[0], bkgnd); // negru
	  }
	}
	cout << endl; 
	print_sol_partiala();

 	int pressed = 0; 
	//cin >> aha;
	while (!pressed && window.isOpen()){
	  sf::Event event; 
	  while (!pressed && window.pollEvent(event)){
	    if (event.type == sf::Event::KeyPressed ||
		event.type == sf::Event::MouseButtonPressed
		)
	      pressed = 1;
	    usleep(10000);
	  }
	  texture.loadFromImage(image);
	  window.draw(sprite);
	  window.display();
	}
	
	if (as)
	  if (solutie() && nrsol < NRSOL) {
	    tipar();
	    k = 1; //sare direct
	  } else {
	    k++;
	    init();
	  }
	else k--;
      }
    } else {
      back();
    }
	

    for (int i = 1; i <= nrtari; i++){
      //   galeata(coord[0][i], coord[1][i], culori[sol[sol_aleasa][i]], bkgnd);
    }

    
    
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
      usleep(100000);
    }

    return 0;
}
