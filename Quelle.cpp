#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;


//Aufgabenteil 0.4
//Funktion für Compiler anmelden
string readFile(const string& filename); 
//Aufgabenteil 0.4
//Stringsplitter anmelden
vector<string> splitter(string s, char trennzeichen); 

int main()
{
	
	

	//Test Konsolenausgabe
	cout << " Programm gestartet!" << endl;
	
	//----------------------------------------------------------------------

	//Anfang Aufgabenteil 0.3
	//Bild wird geladen
	Mat pic;
	pic = imread("Lenna.png");
	//Bild wird beschriftet
	putText(/*Datei*/pic,/*Text*/ "Hallo Welt",/*Position*/ cvPoint(90, 90),/*Schrift*/
		FONT_ITALIC,/*Schriftgröße*/ 2.0, /*Farbe*/ cvScalar(200, 200, 250), /*Linienstärke*/5, CV_AA);
	//Bild wird geöffnet
	imshow("Bild Beschriftet", pic);
	waitKey();
	//Ende Aufgabenteil 0.3

	//----------------------------------------------------------------------

	//Anfang Aufgabenteil 0.4

	//String der mit Pfadangaben aus .txt Datei von Funktion "readFile()" beschrieben wurde
	//!!! Bitte den Speicherpfad von Datei Path.txt entsprechend anpassen !!!
	string name = readFile("C:/Users/SeBro/Documents/GitHub/MMGProjekt/Path.txt");

	/*String name wird nun in Teilstrings aufgeteilt*/
	vector<string> separiert = splitter(name, '\n');


	//Ausgabe von Vektor separiert über String teil in Kommandozeile
	for (string teil : separiert)
	{
		cout << teil << " .... wird geoeffnet!" << endl;
	}

	//Übergabe der Pfade von separiert und anschließendes öffnen
	for (string teil : separiert)
	{
		Mat buf;
		buf = imread(teil);
		imshow(teil, buf);
		cout << teil << " ---- wurde geoeffnet!" << endl;
	}

	//----------------------------------------------------------------------

	waitKey();

	return 0;



}




//Aufgabenteil 0.4
//Funktion zum einlesen einer Textdatei und deren zeilenweise Kopie in den Stringbuffer
string readFile(const string& filename)
{

	ifstream quelle(filename.c_str());

	if (quelle.is_open()) {
		stringbuf buffer;

		if (quelle >> &buffer) {
			return buffer.str();
		}
		else {
			// I/O Fehler
			cout << "Konnte nicht gelesen werden!" << endl;
			waitKey();
			destroyAllWindows;
		}
	}
	else {
		cout << "Datei existiert nicht oder kein Zugriff möglich" << endl;
		waitKey();
		destroyAllWindows;
	}
}

//----------------------------------------------------------------------

//Aufgabenteil 0.4
//Methode zum Splitten des Strings in einen Vektor
vector<string> splitter(string s, char trennzeichen)
{
	vector<string> internal;
	stringstream sstream(s); // Turn the string into a stream.
	string tok;

	while (getline(sstream, tok, trennzeichen)) {
		internal.push_back(tok);
	}

	return internal;
}
